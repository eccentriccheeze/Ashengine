#include "Window.hpp"

int main()
{
    PixelColourMap::Init();
    Window W("Hi there!", 50, 50, 8, 8);
    W.SetTitle("New Title!");
    W.SetBuffer(Colour::BLUE);
    W.Draw();
}