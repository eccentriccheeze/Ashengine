#include "TestGameCode.hpp"

#include "Input.hpp"
#include "Window.hpp"

// In this CPP file, we are defining how these functions work. In the HPP, we only declare them. Here, we define what they do.

void TestGameCode::OnTick( Window& a_GameWindow, float a_DeltaTime )
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

static float i = 0;

void TestGameCode::OnDraw( Window& a_GameWindow )
{
	a_GameWindow.SetRect( RectangleX, RectangleY, RectangleWidth, RectangleHeight, Colour::LIGHT_BLUE );
	a_GameWindow.SetRect( ButtonX, ButtonY, ButtonWidth, ButtonHeight, ButtonCurrent );
}