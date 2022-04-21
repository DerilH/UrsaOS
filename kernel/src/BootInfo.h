#pragma once
#include <stdint.h>
#include "Graphics/Framebuffer.h"
#include "SimpleFonts.h"
#include "Memory/efiMemoryType.h"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
};
