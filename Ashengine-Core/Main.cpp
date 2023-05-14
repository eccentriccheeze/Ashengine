#include "Window.hpp"
#include "Input.hpp"

struct GameCode
{
	int RectangleX = 0;
	int RectangleY = 0;
	int RectangleWidth = 4;
	int RectangleHeight = 4;
	int ButtonX = 20;
	int ButtonY = 20;
	int ButtonWidth = 20;
	int ButtonHeight = 10;
	Pixel ButtonNormal = Colour::RED;
	Pixel ButtonHover = Colour::DARK_GREEN;
	Pixel ButtonClicked = Colour::WHITE;
	Pixel ButtonCurrent = ButtonNormal;

	void OnTick(Window& a_GameWindow)
	{
		if (Input::IsKeyDown(KeyCode::Right) && RectangleX < a_GameWindow.GetWidth() - RectangleWidth)
		{
			RectangleX += 1;
		}

		if (Input::IsKeyDown(KeyCode::Left) && RectangleX > 0)
		{
			RectangleX -= 1;
		}

		if (Input::IsKeyDown(KeyCode::Up) && RectangleY > 0)
		{
			RectangleY -= 1;
		}

		if (Input::IsKeyDown(KeyCode::Down) && RectangleY < a_GameWindow.GetHeight() - RectangleHeight)
		{
			RectangleY += 1;
		}

		// Make the rectangle that changes colour when you hover over it.
		float MouseX, MouseY;
		Input::GetMousePosition(MouseX, MouseY);

		// If mouse is over the button
		if (
			(MouseX >= ButtonX) &&
			(MouseX < ButtonX + ButtonWidth) &&
			(MouseY >= ButtonY) &&
			(MouseY < ButtonY + ButtonWidth))
		{
			// If left clicking, change it's colour to the clicked colour;
			if (Input::IsMouseDown(MouseCode::LeftMouse))
			{
				ButtonCurrent = ButtonClicked;
			}
			else
			{
				ButtonCurrent = ButtonHover;
			}
		}
		else
		{
			ButtonCurrent = ButtonNormal;
		}
	}

	void OnDraw(Window& a_GameWindow)
	{
		a_GameWindow.SetRect(RectangleX, RectangleY, RectangleWidth, RectangleHeight, Colour::LIGHT_BLUE);
		a_GameWindow.SetRect(ButtonX, ButtonY, ButtonWidth, ButtonHeight, ButtonCurrent);
	}
};

int main()
{
	// Create a window to draw to.
	Window GameWindow("New window!", 128, 128, 4, 4);
	
	// Check if the new window creation was successful. If not, return error code.
	if (!GameWindow.IsValid())
	{
		return 1;
	}

	// Set the main window for this application to the one we just made.
	const Window* GameWindowPointer = &GameWindow;
	Window::SetMainWindow(GameWindowPointer);

	// Do things that need to happen once at the start, like setting up input.
	Input::Init();
	Pixel BackgroundPixel = Colour::BLACK;

	// Do things here to setup game code.
	GameCode SomeRandomGameCode;

	// Run a game loop.
	while (true)
	{
		// Tick things that need to happen each frame, like input.
		Input::Tick();


		// Clear the background ready for things to be drawn over the top of it.
		GameWindow.SetBuffer(BackgroundPixel);


		// Run game code
		// ...
		SomeRandomGameCode.OnTick(GameWindow);


		// Run graphics code
		// ...
		SomeRandomGameCode.OnDraw(GameWindow);


		// Draw to the window.
		GameWindow.Draw();


		// For now, we don't have any sort of frame rate system, so just pad it with some time.
		Sleep(33);
	}
}