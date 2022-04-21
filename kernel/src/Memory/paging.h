#pragma once
#include <stdint.h>

enum PT_Flag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrought = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 6,
    Custom0 = 7,
    Custom1 = 8,
    Custom2 = 9,
    NX = 63
};

struct PageDirectoryEntry{
    uint64_t Value;
    void SetFlag(PT_Flag flag, bool enabled);
    bool GetFlag(PT_Flag flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress();
};

struct PageTable { 
    PageDirectoryEntry entries[512];
}__attribute__((aligned(0x1000)));