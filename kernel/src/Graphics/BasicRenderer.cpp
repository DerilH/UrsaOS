#include "BasicRenderer.h"

BasicRenderer::BasicRenderer(Screen screen, PSF1_FONT *stdFont, unsigned int stdColor)
{
	this->screen = screen;
	this->stdFont = stdFont;
	this->stdColor = stdColor;
}

bool BasicRenderer::PrintChar(PSF1_FONT *font, unsigned int color, char chr, unsigned int xOff, unsigned yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return false;

	char *fontPtr = (char *)font->glyphBuffer + (chr * font->psf1_header->charSize);

	for (unsigned long y = yOff; y < yOff + 16; y++)
	{
		for (unsigned long x = xOff; x < xOff + 8; x++)
		{
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
			{
				this->screen.PutPixel(x, y, color);
			}
		}
		fontPtr++;
	}
	return true;
}
bool BasicRenderer::PrintChar(unsigned int color, char chr, unsigned int xOff, unsigned yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return false;

	char *fontPtr = (char *)this->stdFont->glyphBuffer + (chr * this->stdFont->psf1_header->charSize);

	for (unsigned long y = yOff; y < yOff + 16; y++)
	{
		for (unsigned long x = xOff; x < xOff + 8; x++)
		{
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
			{
				this->screen.PutPixel(x, y, color);
			}
		}
		fontPtr++;
	}
	return true;
}
bool BasicRenderer::PrintChar(char chr, unsigned int xOff, unsigned yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return false;

	unsigned int *pixPtr = (unsigned int *)this->screen.buffer->BaseAddress;
	char *fontPtr = (char *)this->stdFont->glyphBuffer + (chr * this->stdFont->psf1_header->charSize);

	for (unsigned long y = yOff; y < yOff + 16; y++)
	{
		for (unsigned long x = xOff; x < xOff + 8; x++)
		{
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
			{
				this->screen.PutPixel(x, y, this->stdColor);
			}
		}
		fontPtr++;
	}
	return true;
}

void BasicRenderer::Print(PSF1_FONT *font, unsigned int color, const char *line, unsigned int xOff, unsigned yOff)
{
	int x = 0;
	char *chr = (char *)line;
	while (*chr != '\0')
	{
		if(!PrintChar(font, color, *chr, x + xOff, yOff)) return;
		x += 8;
		chr++;
	}
}
void BasicRenderer::Print(unsigned int color, const char *line, unsigned int xOff, unsigned yOff)
{
	int x = 0;
	char *chr = (char *)line;
	while (*chr != '\0')
	{
		if(!PrintChar(color, *chr, x + xOff, yOff)) return;
		x += 8;
		chr++;
	}
}
void BasicRenderer::Print(const char *line, unsigned int xOff, unsigned yOff)
{
	int x = 0;
	char *chr = (char *)line;
	while (*chr != '\0')
	{
		if(!PrintChar(*chr, x + xOff, yOff)) return;
		x += 8;
		chr++;
	}
}

void BasicRenderer::DrawUnderLine(unsigned int charSize, unsigned int color, unsigned int xOff, unsigned int yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return;

	for (int y = yOff + (charSize * 1.5) + 2; y < yOff + (charSize * 2); y++)
	{
		for (int x = xOff; x < xOff + charSize; x++)
		{
			this->screen.PutPixel(x, y, color);
		}
	}
}
void BasicRenderer::DrawUnderLine(unsigned int charSize, unsigned int xOff, unsigned int yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return;

	for (int y = yOff + (charSize * 1.5) + 2; y < yOff + (charSize * 2); y++)
	{
		for (int x = xOff; x < xOff + charSize; x++)
		{
			this->screen.PutPixel(x, y, this->stdColor);
		}
	}
}

void BasicRenderer::Clear(unsigned int Width, unsigned int Height, unsigned int xOff, unsigned int yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return;

	for (int y = yOff; y < yOff + Height * 2; y++)
	{
		for (int x = xOff; x < xOff + Width; x++)
		{
			if(!this->screen.PutPixel(x, y, 0)) return;
		}
	}
}
void BasicRenderer::Clear(unsigned int color, unsigned int Width, unsigned int Height, unsigned int xOff, unsigned int yOff)
{
	if(!this->screen.PointExist(xOff, yOff)) return;

	for (int y = yOff; y < yOff + Height; y++)
	{
		for (int x = xOff; x < xOff + Width; x++)
		{
			if(!this->screen.PutPixel(x, y, color)) return;
		}
	}
}

void BasicRenderer::ClearBuffer()
{
	unsigned int *pixPtr = (unsigned int *)this->screen.buffer->BaseAddress;
	for (int i = 0; i < this->screen.HorizontalResolution * this->screen.VerticalResolution; i++)
	{
		*(unsigned int *)(pixPtr + i) = 0;
	}
}

void BasicRenderer::ClearBuffer(unsigned int color)
{
	unsigned int *pixPtr = (unsigned int *)this->screen.buffer->BaseAddress;
	for (int i = 0; i < this->screen.HorizontalResolution * this->screen.VerticalResolution; i++)
	{
		*(unsigned int *)(pixPtr + i) = color;
	}
}

BasicRenderer GlobalRenderer;