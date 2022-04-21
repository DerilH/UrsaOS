#include "Screen.h"

unsigned int *pixPtr;

Screen::Screen(){}

Screen::Screen(Framebuffer *buffer){
    this->buffer = buffer;
    this->HorizontalResolution = buffer->Width;
    this->VerticalResolution = buffer->Height;
    pixPtr = (unsigned int *)this->buffer->BaseAddress;
}

bool Screen::PutPixel(unsigned int X, unsigned int Y, unsigned int color){
    if(!PointExist(X, Y)) return false;

    *(unsigned int *)(pixPtr + X + (Y * this->buffer->PixelsPerScanLine)) = color;
    return true;
}

bool Screen::PointExist(unsigned int X, unsigned int Y){
    if (X >= this->HorizontalResolution)
		return false;
	if (Y >= this->VerticalResolution)
		return false;
    return true;
}

