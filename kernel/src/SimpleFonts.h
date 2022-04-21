#pragma once
#include <stdint.h>

struct PSF1_HEADER
{
	unsigned char magic[2];
	unsigned char mode;
	uint8_t charSize;
};

struct PSF1_FONT
{
	PSF1_HEADER *psf1_header;
	void *glyphBuffer;
};