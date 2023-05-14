#include "Pixel.hpp"
#include "PixelColourMap.hpp"

Pixel::Pixel(Colour a_Colour)
{
	*this = PixelColourMap::Get().ConvertColour(a_Colour);
}