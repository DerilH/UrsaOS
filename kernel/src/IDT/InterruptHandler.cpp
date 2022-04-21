#include "InterruptHandler.h"
#include "../Memory/PageFrameAllocator.h"
#include "../Panic.h"
#include "../IO/IO.h"
#include "../IO/UserInput/keyboard.h"

Terminal *terminal;

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame *frame)
{
	Panic("Page fault detected");
	while (true)
		;
}

__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame *frame)
{
	Panic("Double Fault Detected");
	while (true)
		;
}

__attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame *frame)
{
	Panic("General Protection Fault Detected");
	while (true)
		;
}

__attribute__((interrupt)) void KeyboardInt_Handler(struct interrupt_frame *frame)
{
	uint8_t scancode = inb(0x60);
	HandleKeyboard(scancode, terminal);
	PIC_EndMaster();
}

InterruptHandler::InterruptHandler(Terminal *outputTerminal)
{
	terminal = outputTerminal;
}

IDTR idtr;
void InterruptHandler::PrepareInterrupts()
{
	idtr.Limit = 0x0FFF;
	idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

	IDTDescEntry *int_PageFault = (IDTDescEntry *)(idtr.Offset + 0xE * sizeof(IDTDescEntry));
	int_PageFault->SetOffset((uint64_t)PageFault_Handler);
	int_PageFault->TypeAttr = IDT_TA_InterruptGate;
	int_PageFault->SegSelector = 0x08;

	IDTDescEntry *int_DoubleFault = (IDTDescEntry *)(idtr.Offset + 0x8 * sizeof(IDTDescEntry));
	int_DoubleFault->SetOffset((uint64_t)DoubleFault_Handler);
	int_DoubleFault->TypeAttr = IDT_TA_InterruptGate;
	int_DoubleFault->SegSelector = 0x08;

	IDTDescEntry *int_GPFault = (IDTDescEntry *)(idtr.Offset + 0xD * sizeof(IDTDescEntry));
	int_GPFault->SetOffset((uint64_t)GPFault_Handler);
	int_GPFault->TypeAttr = IDT_TA_InterruptGate;
	int_GPFault->SegSelector = 0x08;

	IDTDescEntry *int_Keyboard = (IDTDescEntry *)(idtr.Offset + 0x21 * sizeof(IDTDescEntry));
	int_Keyboard->SetOffset((uint64_t)KeyboardInt_Handler);
	int_Keyboard->TypeAttr = IDT_TA_InterruptGate;
	int_Keyboard->SegSelector = 0x08;

	asm("lidt %0"
		:
		: "m"(idtr));

	RemapPIC();

	outb(PIC1_DATA, 0b11111101);
	outb(PIC2_DATA, 0b11111111);

	asm("sti");
}

void PIC_EndMaster()
{
	outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave()
{
	outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC()
{
	uint8_t a1, a2;

	a1 = inb(PIC1_DATA);
	io_wait();
	a2 = inb(PIC2_DATA);
	io_wait();

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA, 0x20);
	io_wait();
	outb(PIC2_DATA, 0x28);
	io_wait();

	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	io_wait();
	outb(PIC2_DATA, a2);
}