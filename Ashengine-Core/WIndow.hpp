#pragma once
#include <Windows.h>

#include "Colour.hpp"
#include "PixelColourMap.hpp"

class Window
{
public:

    typedef HANDLE      ConsoleHandle;
    typedef HWND        WindowHandle;
    typedef SMALL_RECT  WindowRegion;

    class ScreenBuffer
    {
    public:

        ScreenBuffer(int a_Width, int a_Height)
        {
            m_Pixels = new Pixel[static_cast<size_t>(a_Width) * a_Height];
            m_Colours = new Colour[static_cast<size_t>(a_Width) * a_Height];
            m_Width = a_Width;
            m_Height = a_Height;
        }

        ~ScreenBuffer()
        {
            delete[] m_Pixels;
            delete[] m_Colours;
        }

        const Pixel* GetPixels() const
        {
            return m_Pixels;
        }

        const Colour* GetColours() const
        {
            return m_Colours;
        }

        int GetArea() const
        {
            return m_Width * m_Height;
        }

        int GetWidth() const
        {
            return m_Width;
        }

        int GetHeight() const
        {
            return m_Height;
        }

        void SetPixel(int a_X, int a_Y, Pixel a_Pixel)
        {
            m_Pixels[a_Y * m_Width + a_X] = a_Pixel;
        }

        void SetPixels(int a_Index, Pixel a_Pixel, int a_Count)
        {
            Pixel* PixelBegin = m_Pixels + a_Index;

            for (; a_Count > 0; --a_Count)
            {
                *PixelBegin = a_Pixel;
                ++PixelBegin;
            }
        }

        void SetPixels(int a_X, int a_Y, Pixel a_Pixel, int a_Count)
        {
            Pixel* PixelBegin = m_Pixels + (a_Y * m_Width + a_X);

            for (; a_Count > 0; --a_Count)
            {
                *PixelBegin = a_Pixel;
                ++PixelBegin;
            }
        }

        void SetColour(int a_X, int a_Y, Colour a_Colour)
        {
            int Index = a_Y * m_Width + a_X;
            m_Pixels[Index] = PixelColourMap::Get().ConvertColour(a_Colour);
            m_Colours[Index] = a_Colour;
        }

        void SetColours(int a_X, int a_Y, Colour a_Colour, int a_Count)
        {
            int Index = a_Y * m_Width + a_X;
            Pixel PixelToSet = PixelColourMap::Get().ConvertColour(a_Colour);
            Pixel* PixelBegin = m_Pixels + Index;
            Colour* ColourBegin = m_Colours + Index;

            for (; a_Count > 0; --a_Count)
            {
                *PixelBegin = PixelToSet;
                *ColourBegin = a_Colour;
                ++PixelBegin;
                ++ColourBegin;
            }
        }

        void SetColours(int a_Index, Colour a_Colour, int a_Count)
        {
            Pixel PixelToSet = PixelColourMap::Get().ConvertColour(a_Colour);
            Pixel* PixelBegin = m_Pixels + a_Index;
            Colour* ColourBegin = m_Colours + a_Index;

            for (; a_Count > 0; --a_Count)
            {
                *PixelBegin = PixelToSet;
                *ColourBegin = a_Colour;
                ++PixelBegin;
                ++ColourBegin;
            }
        }

        inline void SetBuffer(Pixel a_Pixel)
        {
            SetPixels(0, 0, a_Pixel, GetArea());
        }

        inline void SetBuffer(Colour a_Colour)
        {
            SetColours(0, 0, a_Colour, GetArea());
        }

        void SetRect(int a_X, int a_Y, int a_Width, int a_Height, Pixel a_Pixel)
        {
            int Index = a_Y * m_Width + a_X;

            for (int y = 0; y < a_Height; ++y)
            {
                SetPixels(Index, a_Pixel, a_Width);
                Index += m_Width;
            }
        }

        void SetRect(int a_X, int a_Y, int a_Width, int a_Height, Colour a_Colour)
        {
            int Index = a_Y * m_Width + a_X;

            for (int y = 0; y < a_Height; ++y)
            {
                SetColours(Index, a_Colour, a_Width);
                Index += m_Width;
            }
        }

    private:

        friend class ConsoleWindow;

        Pixel* m_Pixels;
        Colour* m_Colours;
        int m_Width;
        int m_Height;
    };

    Window(const char* a_Title, int a_Width, int a_Height, int a_PixelWidth = 4, int a_PixelHeight = 4)
        : m_ScreenBuffer(a_Width, a_Height)
        , m_TitleBuffer()
        , m_WindowRegion()
        , m_WindowHandle()
        , m_RawWidth( 0 )
        , m_RawHeight( 0 )
        , m_PixelWidth( 0 )
        , m_PixelHeight( 0 )
    {
        PixelColourMap::Init();

        // Retrieve handles for console window.
        m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (m_ConsoleHandle == INVALID_HANDLE_VALUE)
        {
            if (AllocConsole())
            {
                m_IsValid = false;
                return;
            }

            m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        m_WindowHandle = GetConsoleWindow();

        // Set console font.
        a_PixelWidth = min(a_PixelWidth, static_cast<short>(8));
        a_PixelHeight = min(a_PixelHeight, static_cast<short>(8));
        CONSOLE_FONT_INFOEX FontInfo;
        FontInfo.cbSize = sizeof(FontInfo);
        FontInfo.nFont = 0;
        FontInfo.dwFontSize = { (short)a_PixelWidth, (short)a_PixelHeight };
        FontInfo.FontFamily = FF_DONTCARE;
        FontInfo.FontWeight = FW_NORMAL;
        wcscpy_s(FontInfo.FaceName, L"Terminal");
        SetCurrentConsoleFontEx(m_ConsoleHandle, false, &FontInfo);

        // Get screen buffer info object.
        CONSOLE_SCREEN_BUFFER_INFOEX ScreenBufferInfo;
        ScreenBufferInfo.cbSize = sizeof(ScreenBufferInfo);
        GetConsoleScreenBufferInfoEx(m_ConsoleHandle, &ScreenBufferInfo);

        for (int i = 0; i < 16; ++i)
        {
            COLORREF& ColourRef = ScreenBufferInfo.ColorTable[i];
            Colour SeedColour = PixelColourMap::SeedColours[i];
            ColourRef =
                SeedColour.B << 16 |
                SeedColour.G << 8 |
                SeedColour.R;
        }

        SetConsoleScreenBufferInfoEx(m_ConsoleHandle, &ScreenBufferInfo);

        // Get largest possible window size that can fit on screen.
        COORD LargestWindow = GetLargestConsoleWindowSize(m_ConsoleHandle);

        // If smaller than requested size, exit.
        if (LargestWindow.X < a_Width ||
            LargestWindow.Y < a_Height)
        {
            m_IsValid = false;
            return;
        }

        // Set screen buffer.
        COORD WindowSize = { (short)GetWidth(), (short)GetHeight() };

        // Set window region rect.
        m_WindowRegion.Left = 0;
        m_WindowRegion.Top = 0;
        m_WindowRegion.Right = WindowSize.X - 1;
        m_WindowRegion.Bottom = WindowSize.Y - 1;

        // Set console attributes.
        SetConsoleScreenBufferSize(m_ConsoleHandle, { (short)a_Width, (short)a_Height });
        SetConsoleWindowInfo(m_ConsoleHandle, true, &m_WindowRegion);
        GetConsoleScreenBufferInfoEx(m_ConsoleHandle, &ScreenBufferInfo);
        SetConsoleScreenBufferSize(m_ConsoleHandle, { (short)a_Width, (short)a_Height });

        // Set cursor attributes.
        CONSOLE_CURSOR_INFO CursorInfo;
        GetConsoleCursorInfo(m_ConsoleHandle, &CursorInfo);
        CursorInfo.bVisible = false;
        SetConsoleCursorInfo(m_ConsoleHandle, &CursorInfo);

        // Set window attributes.
        SetWindowLong(m_WindowHandle, GWL_STYLE, WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
        SetWindowPos(m_WindowHandle, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
        SetTitle(a_Title);

        RECT WindowRect;
        GetWindowRect(m_WindowHandle, &WindowRect);

        m_RawWidth = WindowRect.right - WindowRect.left;
        m_RawHeight = WindowRect.bottom - WindowRect.top;
        m_PixelWidth = m_RawWidth / m_ScreenBuffer.GetWidth();
        m_PixelHeight = m_RawHeight / m_ScreenBuffer.GetHeight();
        m_IsValid = true;
    }

    static void SetMainWindow(const Window* a_Window) { s_MainWindow = a_Window; }

    static const Window* GetMainWindow() { return s_MainWindow; }

    bool IsValid() const { return m_IsValid; }

    void SetTitle(const char* a_Title)
    {
        size_t Length = strlen(a_Title) + 1;
        Length = Length > 64 ? 64 : Length;
        mbstowcs_s(nullptr, m_TitleBuffer, Length, a_Title, Length);
        SetConsoleTitleW(m_TitleBuffer);
    }

    const char* GetTitle() const { return m_Title.c_str(); }

    int GetWidth() const
    {
        return m_ScreenBuffer.GetWidth();
    }

    int GetHeight() const
    {
        return m_ScreenBuffer.GetHeight();
    }

    int GetArea() const
    {
        return m_ScreenBuffer.GetArea();
    }

    const Pixel* GetPixels() const
    {
        return m_ScreenBuffer.GetPixels();
    }

    const Colour* GetColours() const
    {
        return m_ScreenBuffer.GetColours();
    }

    int GetRawWidth() const { return m_RawWidth; }

    int GetRawHeight() const { return m_RawHeight; }

    int GetPixelWidth() const
    {
        return m_PixelWidth;
    }

    int GetPixelHeight() const
    {
        return m_PixelHeight;
    }

    void SetPixel(int a_X, int a_Y, Pixel a_Pixel)
    {
        m_ScreenBuffer.SetPixel(a_X, a_Y, a_Pixel);
    }

    void SetPixels(int a_Index, Pixel a_Pixel, int a_Count)
    {
        m_ScreenBuffer.SetPixels(a_Index, a_Pixel, a_Count);
    }

    void SetPixels(int a_X, int a_Y, Pixel a_Pixel, int a_Count)
    {
        m_ScreenBuffer.SetPixels(a_X, a_Y, a_Pixel, a_Count);
    }

    void SetColour(int a_X, int a_Y, Colour a_Colour)
    {
        m_ScreenBuffer.SetColour(a_X, a_Y, a_Colour);
    }

    void SetColours(int a_X, int a_Y, Colour a_Colour, int a_Count)
    {
        m_ScreenBuffer.SetColours(a_X, a_Y, a_Colour, a_Count);
    }

    void SetColours(int a_Index, Colour a_Colour, int a_Count)
    {
        m_ScreenBuffer.SetColours(a_Index, a_Colour, a_Count);
    }

    inline void SetBuffer(Pixel a_Pixel)
    {
        m_ScreenBuffer.SetBuffer(a_Pixel);
    }

    inline void SetBuffer(Colour a_Colour)
    {
        m_ScreenBuffer.SetBuffer(a_Colour);
    }

    void SetRect(int a_X, int a_Y, int a_Width, int a_Height, Pixel a_Pixel)
    {
        m_ScreenBuffer.SetRect(a_X, a_Y, a_Width, a_Height, a_Pixel);
    }

    void SetRect(int a_X, int a_Y, int a_Width, int a_Height, Colour a_Colour)
    {
        m_ScreenBuffer.SetRect(a_X, a_Y, a_Width, a_Height, a_Colour);
    }

    void Draw() const
    {
        WriteConsoleOutput(
            m_ConsoleHandle,
            ( CHAR_INFO* )m_ScreenBuffer.GetPixels(),
            { ( short )GetWidth(), ( short )GetHeight() },
            { 0, 0 },
            const_cast< WindowRegion* >(&m_WindowRegion));
    }

    ConsoleHandle GetConsoleHandle() const
    {
        return m_ConsoleHandle;
    }

    WindowHandle GetWindowHandle() const
    {
        return m_WindowHandle;
    }

private:

    bool                         m_IsValid;
    ConsoleHandle                m_ConsoleHandle;
    WindowHandle                 m_WindowHandle;
    WindowRegion                 m_WindowRegion;
    int                          m_RawWidth;
    int                          m_RawHeight;
    int                          m_PixelWidth;
    int                          m_PixelHeight;
    wchar_t                      m_TitleBuffer[64];
    std::string                  m_Title;
    ScreenBuffer                 m_ScreenBuffer;
    static const Window*         s_MainWindow;
};