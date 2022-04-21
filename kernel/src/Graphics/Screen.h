#pragma once
#include "Framebuffer.h"

class Screen
{
public:
    Framebuffer *buffer;
    int HorizontalResolution, VerticalResolution;

    Screen();
    Screen(Framebuffer *buffer);

    bool PutPixel(unsigned int X, unsigned int Y, unsigned int color);
    bool PointExist(unsigned int X, unsigned int Y);
};