#pragma once

#define MEM_FREE 0
#define MEM_USED 1
#define MEM_RESERVED 2

#include "efiMemoryType.h"
#include <stdint.h>
#include "../Graphics/Bitmap.h"
#include "memory.h"

class PageFrameAllocator {
public:
    Bitmap PageBitmap;
    void ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize);
    uint64_t GetRAM(int8_t type = MEM_FREE);

    void* RequestPage();
    
    void LockPage(void* address);
    void LockPages(void* address, uint64_t count);
    
    void FreePage(void* address);
    void FreePages(void* address, uint64_t count);

private:
    void InitBitmap(size_t bitmapSize, void* bufferAddress);

    void ReservePage(void* address);
    void ReservePages(void* address, uint64_t count);

    void UnreservePage(void* address);
    void UnreservePages(void* address, uint64_t count);
};

extern PageFrameAllocator GlobalAllocator;
