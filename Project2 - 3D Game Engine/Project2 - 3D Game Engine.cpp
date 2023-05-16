#include <iostream>

#include "Window.hpp"
#include "Input.hpp"

int main()
{
	// Create a window to draw to.
	Window GameWindow( "New window!", 128, 128, 8, 8 );

	// Check if the new window creation was successful. If not, return error code.
	if ( !GameWindow.IsValid() )
	{
		return 1;
	}

	// Set the main window for this application to the one we just made.
	const Window* GameWindowPointer = &GameWindow;
	Window::SetMainWindow( GameWindowPointer );

	// Do things that need to happen once at the start, like setting up input.
	Input::Init();



}
