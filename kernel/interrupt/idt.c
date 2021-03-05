#include "../../common/types.h"
#include "idt.h"
#include "irq.h"
#include "../x86.h"
#include "../mem/gdt.h"
#include "../descriptors.h"
#include "../lib/string.h"
#include "../lib/stdio.h"
#include "../drivers/pic.h"

// Defined in irq.c
extern handler_t irq_handlers[IRQ_COUNT];

// CPU context used when saving/restoring context from an interrupt
typedef struct regs_st {
	uint32_t tss_selector;  // to know where we're coming from
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;

// TODO: setup the IDT table for CPU exceptions and hardware interrupts
static idt_entry_t idt[INTERRUPT_COUNT];
static idt_ptr_t idt_ptr;

// Build and return an IDT entry.
// selector is the code segment selector to access the ISR
// offset is the address of the ISR (for task gates, offset must be 0)
// type indicates the IDT entry type
// dpl is the privilege level required to call the associated ISR
static idt_entry_t idt_build_entry(uint16_t selector, uint32_t offset, uint8_t type, uint8_t dpl) {
	idt_entry_t entry;
	entry.offset15_0 = offset & 0xffff;
	entry.selector = selector;
	entry.reserved = 0;
	entry.type = type;
	entry.dpl = dpl;
	entry.p = 1;
	entry.offset31_16 = (offset >> 16) & 0xffff;
	return entry;
}

extern void _exception_3();
extern void _exception_8();
extern void _irq_5();

// Exception handler
void exception_handler(regs_t *regs) {
	// TODO: handle CPU exceptions:
	// Prints a message with the exception type as well as its number and stops the kernel.
	set_font_background_color(COLOR_RED, COLOR_BLACK);
	my_printf("\nException num=%d abort !\n", regs->number);
	halt();
}

// IRQ handler
void irq_handler(regs_t *regs) {
	uint_t irq = regs->number;
	pic_eoi(irq);

	handler_t handler = irq_handlers[irq];
	if (handler.func)
		handler.func();
}

void idt_init() {
	memset(irq_handlers, 0, sizeof(irq_handlers));
	memset(idt, 0, sizeof(idt));
	// TODO: setup idt_ptr, the IDT pointer
	idt_ptr.base = (uint32_t)idt;
	idt_ptr.limit = sizeof(idt)-1;
	
	// TODO: setup IDT entries for CPU exceptions and hardware interrupts (IRQs)
	// Example: idt[3] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)_exception_3, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	for (int i = 0; i <= 20; i++)
	{
		idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)_exception_3, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	}

	for (int i = 32; i <= 47; i++)
	{
		idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)_irq_5, TYPE_INTERRUPT_GATE, DPL_KERNEL);
	}
	
	
	// TODO: load the IDT using idt_ptr
	idt_load(&idt_ptr);
}