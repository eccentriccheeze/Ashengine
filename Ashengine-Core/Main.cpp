#include "Window.hpp"

int main()
{
	PixelColourMap::Init();

	Window New("Asher's new window!", 50, 50);
	
	for (int i = 0; i < 10; ++i)
	{
		Colour BackgroundColour = Colour::RED;
		Pixel BackgroundPixel(BackgroundColour);
		New.SetBuffer(BackgroundPixel);
		New.SetRect(i, 4, 15, 20, Colour::BLUE);

		New.Draw();
		Sleep(1000);
	}
}