#pragma once
#include <stddef.h>
#include <stdlib.h>

#include "Terminal.h"
#include "BootInfo.h"
#include "cstr.h"

#include "Utility/Point.h"
#include "Graphics/BasicRenderer.h"
#include "Memory/PageFrameAllocator.h"

#include "IDT/InterruptHandler.h"

class Kernel{
private:
	void InitModules(BootInfo *bootInfo);
	void InitMemory(BootInfo *bootInfo);
	void LogInit();
public:
    Kernel(BootInfo *bootInfo);
    static Kernel *instance_;

    Terminal terminal;
	BasicRenderer renderer;
	Screen screen;
	InterruptHandler interruptHandler;

	static Kernel* getKernel();
};