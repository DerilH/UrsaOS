#include "kernel.h"
#include "Gdt/gdt.h"
#include "Memory/efiMemoryType.h"
#include "Memory/memory.h"
#include "Memory/PageMapIndexer.h"
#include "Memory/PageTableManager.h"
#include "Memory/paging.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void _start(BootInfo *bootInfo)
{
	Kernel kernel = Kernel(bootInfo);
	for(;;){asm("hlt");}
}

Kernel *Kernel::instance_ = nullptr;

Kernel::Kernel(BootInfo *bootInfo)
{
	this->InitMemory(bootInfo);
	this->InitModules(bootInfo);
	this->LogInit();

	instance_ = this;
}

Kernel *Kernel::getKernel()
{
	return instance_;
}

void Kernel::InitModules(BootInfo *bootInfo)
{
	this->screen = Screen(bootInfo->framebuffer);
	this->renderer = BasicRenderer(screen, bootInfo->font, 0xFFFFFF);
	GlobalRenderer = this->renderer;
	this->renderer.ClearBuffer();
	this->terminal = Terminal(this->screen.HorizontalResolution / 2, this->screen.VerticalResolution / 2, Point(this->screen.HorizontalResolution / 2, this->screen.VerticalResolution / 2), bootInfo->font, 0xFFFFFF, &renderer);
	this->interruptHandler = InterruptHandler(&this->terminal);
	this->interruptHandler.PrepareInterrupts();
}

void Kernel::InitMemory(BootInfo *bootInfo)
{
	// Create and init global allocator
	GlobalAllocator = PageFrameAllocator();
	GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);
	// Create GDT
	GDTDescriptor descriptor;
	descriptor.Size = sizeof(GDT) - 1;
	descriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&descriptor);
	// Lock memory for kernel
	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
	GlobalAllocator.LockPages(&_KernelStart, kernelPages);
	// Map memory
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	PageTable *PML4 = (PageTable *)GlobalAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	PageTableManager pageTableManager = PageTableManager(PML4);

	for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000)
	{
		pageTableManager.MapMemory((void *)t, (void *)t);
	}

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
	GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);
	for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096)
	{
		pageTableManager.MapMemory((void *)t, (void *)t);
	}

	asm("mov %0, %%cr3"
		:
		: "r"(PML4));
}

void Kernel::LogInit()
{
	this->terminal.Print("Kernel succesfully initialized\n");

	this->terminal.Print("Free memory: ");
	this->terminal.Print(to_string(GlobalAllocator.GetRAM(MEM_FREE)));
	this->terminal.Print(" KB\n");
}