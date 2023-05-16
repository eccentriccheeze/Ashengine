#include "Window.hpp"
#include "Input.hpp"
#include "Time.hpp"

struct GameCode
{
	float RectangleMovementSpeed = 25.0f;
	float RectangleX = 0;
	float RectangleY = 0;
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

	void OnTick( Window& a_GameWindow, float a_DeltaTime )
	{
		if ( Input::IsKeyDown( KeyCode::Right ) && RectangleX < a_GameWindow.GetWidth() - RectangleWidth )
		{
			RectangleX += RectangleMovementSpeed * a_DeltaTime;
		}

		if ( Input::IsKeyDown( KeyCode::Left ) && RectangleX > 0 )
		{
			RectangleX -= RectangleMovementSpeed * a_DeltaTime;
		}

		if ( Input::IsKeyDown( KeyCode::Up ) && RectangleY > 0 )
		{
			RectangleY -= RectangleMovementSpeed * a_DeltaTime;
		}

		if ( Input::IsKeyDown( KeyCode::Down ) && RectangleY < a_GameWindow.GetHeight() - RectangleHeight )
		{
			RectangleY += RectangleMovementSpeed * a_DeltaTime;
		}

		// Make the rectangle that changes colour when you hover over it.
		float MouseX, MouseY;
		Input::GetMousePosition( MouseX, MouseY );

		// If mouse is over the button
		if (
			( MouseX >= ButtonX ) &&
			( MouseX < ButtonX + ButtonWidth ) &&
			( MouseY >= ButtonY ) &&
			( MouseY < ButtonY + ButtonHeight ) )
		{
			// If left clicking, change it's colour to the clicked colour;
			if ( Input::IsMouseDown( MouseCode::LeftMouse ) )
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

	void OnDraw( Window& a_GameWindow )
	{
		a_GameWindow.SetRect( RectangleX, RectangleY, RectangleWidth, RectangleHeight, Colour::LIGHT_BLUE );
		a_GameWindow.SetRect( ButtonX, ButtonY, ButtonWidth, ButtonHeight, ButtonCurrent );
	}
};

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
	Pixel BackgroundPixel = Colour::BLACK;

	// Do things here to setup game code.
	GameCode SomeRandomGameCode;

	// Store the time at the end of each loop
	TimePoint StartTime = Time::Now();

	// Store somewhere the deltatime of each frame for use when updating things.
	float DeltaTime = 0.0333;

	// Run a game loop.
	while ( true )
	{
		// Tick things that need to happen each frame, like input.
		Input::Tick();


		// Clear the background ready for things to be drawn over the top of it.
		GameWindow.SetBuffer( BackgroundPixel );


		// Run game code
		// ...
		SomeRandomGameCode.OnTick( GameWindow, DeltaTime );


		// Run graphics code
		// ...
		SomeRandomGameCode.OnDraw( GameWindow );


		// Draw to the window.
		GameWindow.Draw();


		// Let's figure out how long it took for this whole loop to run
		TimePoint EndTime = Time::Now();
		TimeSpan LoopDuration = EndTime - StartTime;
		StartTime = EndTime;

		// Let's now sleep for however long we need to to pad out the time to 30fps.
		// 30 fps means that each loop should take 33.333... milliseconds.
		// 30 frames per seconds means 1/30 seconds per frame = 33.333 milliseconds
		int64_t Milliseconds = LoopDuration.Milliseconds();
		DeltaTime = ( float )Milliseconds / 1000;

		// Note: This whole part here isn't necessary really since we're passing in the deltatime to the game code
		// and if the frame is faster, then it means that the deltatime will be smaller and the game code can
		// adjust how far to move the box for example in one frame.
		// The effect this has is pretty much the same as the VSync setting you see in some games, to try to lock
		// the fps by padding out the time if the frame was too quick. It however doesn't work if the frame too LONGER
		// than the target fps, since you'd have to pad it out negatively.
		if ( Milliseconds < 33.333 )
		{
			// Time left to pad it out to 30fps
			int64_t PadTime = 33.3333 - Milliseconds;

			// Let's sleep for that long to make the whole loop take 33.333 milliseconds
			Sleep( PadTime );
		}
	}
}