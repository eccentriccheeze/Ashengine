#pragma once
#include "Pixel.hpp"
#include "Colour.hpp"

// This is a forward-declaration. It's a way to tell this file that there WILL be a class by this name and to just expect it for later.
// This is allowed since we're using only Window references (Window&) below, and not the full class.
// In the CPP however, when we define the actual stuff that will happen for Window, we do #include "Window.hpp" because we need to know how it operates.
class Window;

struct TestGameCode
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

	void OnTick( Window& a_GameWindow, float a_DeltaTime );
	void OnDraw( Window& a_GameWindow );
};