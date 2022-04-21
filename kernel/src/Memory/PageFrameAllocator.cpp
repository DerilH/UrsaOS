#include "PageFrameAllocator.h"
#include "../kernel.h"

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;
PageFrameAllocator GlobalAllocator;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR *mMap, size_t mMapSize, size_t mMapDescSize)
{
    if (Initialized)
        return;
    Initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescSize;

    void *largestFreeMemSeg = NULL;
    size_t largestFreeMemSegSize = 0;

    for (int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == 7)
        {
            if (desc->numPages * 4096 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->numPages * 4096;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescSize);

    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    InitBitmap(bitmapSize, largestFreeMemSeg);

    LockPages(&this->PageBitmap.Buffer, this->PageBitmap.Size / 4096 + 1);
   
    for (int i = 0; i < mMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type != 7)
        {
            ReservePages(desc->physAddr, desc->numPages);
        }
    }
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void *bufferAddress)
{
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t *)bufferAddress;
    for (int i = 0; i < bitmapSize; i++)
    {
        *(uint8_t *)(PageBitmap.Buffer + i) = 0;
    }
}
uint64_t pageBitmapIndex = 0;
void *PageFrameAllocator::RequestPage()
{
    for (; pageBitmapIndex < this->PageBitmap.Size * 8; pageBitmapIndex++)
    {
        if (this->PageBitmap[pageBitmapIndex] == true)
            continue;
        LockPage((void *)(pageBitmapIndex * 4096));
        return (void *)(pageBitmapIndex * 4096);
    }

    return NULL;
}

void PageFrameAllocator::LockPage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true)
        return;
    if(!PageBitmap.Set(index, true)) return;
    freeMemory -= 4096;
    usedMemory += 4096;
}
void PageFrameAllocator::LockPages(void *address, uint64_t count)
{
    for (int i = 0; i < count; i++)
    {
        LockPage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::FreePage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false)
        return;
    if(!PageBitmap.Set(index, false)) return;

    freeMemory += 4096;
    usedMemory -= 4096;
    if(pageBitmapIndex > index) pageBitmapIndex = index;
}
void PageFrameAllocator::FreePages(void *address, uint64_t count)
{
    for (int i = 0; i < count; i++)
    {
        FreePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::ReservePage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == true)
        return;
    if(!PageBitmap.Set(index, true)) return;
    freeMemory -= 4096;
    reservedMemory += 4096;
}

void PageFrameAllocator::ReservePages(void *address, uint64_t count)
{
    for (int i = 0; i < count; i++)
    {
        ReservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

void PageFrameAllocator::UnreservePage(void *address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (PageBitmap[index] == false)
        return;
    if(!PageBitmap.Set(index, false)) return;
    freeMemory += 4096;
    reservedMemory -= 4096;
    if(pageBitmapIndex > index) pageBitmapIndex = index;
}

void PageFrameAllocator::UnreservePages(void *address, uint64_t count)
{
    for (int i = 0; i < count; i++)
    {
        UnreservePage((void*)((uint64_t)address + (i * 4096)));
    }
}

uint64_t PageFrameAllocator::GetRAM(int8_t type)
{
    switch (type)
    {
    case MEM_FREE:
        return freeMemory / 1024;
    case MEM_USED:
        return usedMemory / 1024;
    case MEM_RESERVED:
        return reservedMemory / 1024;
    default:
        return freeMemory / 1024;
    }
};
