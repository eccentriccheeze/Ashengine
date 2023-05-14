#pragma once
#include <Windows.h>
#include <bitset>

#include "Window.hpp"

enum class KeyCode : unsigned char
{
	Backspace,
	Tab,
	Enter,
	Shift,
	Ctrl,
	Alt,
	Caps,
	Esc,
	Space,
	PageUp,
	PageDown,
	End,
	Home,
	Left,
	Up,
	Right,
	Down,
	PrintScrn,
	Insert,
	Delete,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	NumLock,
	ScrollLock,
	LeftShift,
	RightShift,
	LeftCtrl,
	RightCtrl,
	LeftMenu,
	RightMenu,
	Semicolon,
	Plus,
	Comma,
	Minus,
	Period,
	Forwardslash,
	Tilda,
	LeftBracket,
	Backslash,
	RightBracket,
	InvertedComma
};

enum class MouseCode : unsigned char
{
	LeftMouse,
	RightMouse,
	MiddleMouse,
};

class Input
{
public:

	static void Init()
	{
		GetMousePosition(MousePositionX, MousePositionY);
	}

	inline static bool IsKeyDown(KeyCode a_KeyCode)
	{
		return GetKeyState(KeyCodes[static_cast<unsigned char>(a_KeyCode)]) & 0x8000;
	}

	inline static bool IsKeyUp(KeyCode a_KeyCode)
	{
		return !(GetKeyState(KeyCodes[static_cast<unsigned char>(a_KeyCode)]) & 0x8000);
	}

	inline static bool IsKeyPressed(KeyCode a_KeyCode)
	{
		return !KeyStates[static_cast<unsigned char>(a_KeyCode)] && IsKeyDown(a_KeyCode);
	}

	inline static bool IsKeyReleased(KeyCode a_KeyCode)
	{
		return KeyStates[static_cast<unsigned char>(a_KeyCode)] && IsKeyUp(a_KeyCode);
	}

	inline static bool IsMouseDown(MouseCode a_MouseCode)
	{
		return GetKeyState(MouseCodes[static_cast<unsigned char>(a_MouseCode)]) & 0x8000;
	}

	inline static bool IsMouseUp(MouseCode a_MouseCode)
	{
		return !(GetKeyState(MouseCodes[static_cast<unsigned char>(a_MouseCode)]) & 0x8000);
	}

	inline static bool IsMousePressed(MouseCode a_MouseCode)
	{
		return !MouseStates[static_cast<unsigned char>(a_MouseCode)] && IsMouseDown(a_MouseCode);
	}

	inline static bool IsMouseReleased(MouseCode a_MouseCode)
	{
		return MouseStates[static_cast<unsigned char>(a_MouseCode)] && IsMouseUp(a_MouseCode);
	}

	static void GetMousePosition(float& a_X, float& a_Y)
	{
		POINT Coordinates = { 0, 0 };

		if (!GetCursorPos(&Coordinates))
		{
			a_X = 0;
			a_Y = 0;
			return;
		}

		if (!ScreenToClient(Window::GetMainWindow()->GetWindowHandle(), &Coordinates))
		{
			a_X = 0;
			a_Y = 0;
			return;
		}

		const Window* MainWindow = Window::GetMainWindow();

		a_X = Coordinates.x;
		a_Y = Coordinates.y;

		a_X /= MainWindow->GetPixelWidth();
		a_Y /= MainWindow->GetPixelHeight();
	}

	static void GetMouseDelta(float& a_X, float& a_Y)
	{
		GetMousePosition(a_X, a_Y);
		a_X -= MousePositionX;
		a_Y -= MousePositionY;
	}

	static void Tick()
	{
		for (int i = 0; i < KeyStates.size(); ++i)
		{
			KeyStates[i] = GetKeyState(KeyCodes[i]) & 0x8000;
		}

		for (int i = 0; i < MouseStates.size(); ++i)
		{
			MouseStates[i] = GetKeyState(MouseCodes[i]) & 0x8000;
		}

		GetMousePosition(MousePositionX, MousePositionY);
	}

private:

	static unsigned char     KeyCodes[99];
	static unsigned char     MouseCodes[3];
	static std::bitset< 99 > KeyStates;
	static std::bitset< 3  > MouseStates;
	static float             MousePositionX;
	static float             MousePositionY;
};