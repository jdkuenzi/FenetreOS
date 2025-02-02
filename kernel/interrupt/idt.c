/**
 * @file idt.c
 * @brief handle interrupts and exceptions
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#include "idt.h"
#include "irq.h"
#include "../x86.h"
#include "../mem/gdt.h"
#include "../vid/stdio.h"
#include "../drivers/pic.h"
#include "../descriptors.h"
#include "../task/task.h"
#include "../../common/lib/string.h"

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

static idt_entry_t idt[INTERRUPT_COUNT];
static idt_ptr_t idt_ptr;

/**
 * Build and return an IDT entry.
 * @param selector the code segment selector to access the ISR
 * @param offset the address of the ISR (for task gates, offset must be 0)
 * @param type indicates the IDT entry type
 * @param dpl the privilege level required to call the associated ISR
 * @return IDT entry
 */
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

extern void _exception_0();
extern void _exception_1();
extern void _exception_2();
extern void _exception_3();
extern void _exception_4();
extern void _exception_5();
extern void _exception_6();
extern void _exception_7();
extern void _exception_8();
extern void _exception_9();
extern void _exception_10();
extern void _exception_11();
extern void _exception_12();
extern void _exception_13();
extern void _exception_14();
extern void _exception_15();
extern void _exception_16();
extern void _exception_17();
extern void _exception_18();
extern void _exception_19();
extern void _exception_20();

static void (*exceptions[])() = {
	_exception_0,
	_exception_1,
	_exception_2,
	_exception_3,
	_exception_4,
	_exception_5,
	_exception_6,
	_exception_7,
	_exception_8,
	_exception_9,
	_exception_10,
	_exception_11,
	_exception_12,
	_exception_13,
	_exception_14,
	_exception_15,
	_exception_16,
	_exception_17,
	_exception_18,
	_exception_19,
	_exception_20	
};

static char* excep_desc[] = {
	"Divide Error",
	"RESERVED",
	"NMI Interrupt",
	"Breakpoint",
	"Overflow",
	"BOUND Range Exceeded",
	"Invalid Opcode (Undefined Opcode)",
	"Device Not Available (No Math Coprocessor)",
	"Double Fault",
	"Coprocessor Segment Overrun (reserved)",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection",
	"Page Fault",
	"(Intel reserved. Do not use)",
	"x87 FPU Floating-Point Error (Math Fault)",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception"
};

extern void _irq_0();
extern void _irq_1();
extern void _irq_2();
extern void _irq_3();
extern void _irq_4();
extern void _irq_5();
extern void _irq_6();
extern void _irq_7();
extern void _irq_8();
extern void _irq_9();
extern void _irq_10();
extern void _irq_11();
extern void _irq_12();
extern void _irq_13();
extern void _irq_14();
extern void _irq_15();

static void (*irqs[])() = {
	_irq_0,
	_irq_1,
	_irq_2,
	_irq_3,
	_irq_4,
	_irq_5,
	_irq_6,
	_irq_7,
	_irq_8,
	_irq_9,
	_irq_10,
	_irq_11,
	_irq_12,
	_irq_13,
	_irq_14,
	_irq_15
};

/**
 * Prints a message with the exception type as well as its number and stops the kernel.
 */
void exception_handler(regs_t *regs) {
	char buffer[XL_BUFFER];
	eprintf("Exception : %s\nError code : %d", COLOR_RED, buffer, excep_desc[regs->number], regs->error_code);
	uint32_t task_i = ((regs->tss_selector >> 3) - 4) / 2;
	task_t *t = &tasks[task_i];
	t->task_addr_space[0] = 0xCF;
	regs->eax = -1;
	regs->eip = 0;
}

/**
 * Call the function mapped to the interruption
 */
void irq_handler(regs_t *regs) {
	uint_t irq = regs->number;
	pic_eoi(irq);

	handler_t handler = irq_handlers[irq];
	if (handler.func)
		handler.func();
}

/**
 * init the idt
 */
void idt_init() {
	memset(irq_handlers, 0, sizeof(irq_handlers));
	memset(idt, 0, sizeof(idt));
	idt_ptr.base = (uint32_t)idt;
	idt_ptr.limit = sizeof(idt)-1;
	
	for (int i = 0; i <= 20; i++)
	{
		idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)exceptions[i], TYPE_INTERRUPT_GATE, DPL_KERNEL);
	}

	for (int i = 32; i <= 47; i++)
	{
		idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)irqs[i-32], TYPE_INTERRUPT_GATE, DPL_KERNEL);
	}

	// IDT entry 64: used for system calls
	extern void _syscall_handler();  // Implemented in interrupt_asm.s
	idt[64] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_syscall_handler, TYPE_TRAP_GATE, DPL_USER);
	
	idt_load(&idt_ptr);
}