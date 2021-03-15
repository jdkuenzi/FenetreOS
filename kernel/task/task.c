#include "task.h"
#include "tss.h"

// TSS of the task
static tss_t task_tss;

// Define a LDT for the task; this LDT defines 2 segments
static gdt_entry_t task_ldt[2];

// Allocate 16KB of address space for the task.
// The address space MUST be aligned to 4096 bytes (4KB)!
static uint8_t task_addr_space[16834] __attribute__((aligned(4096)));

// Allocate 4KB of kernel stack for the task
static uint8_t task_kernel_stack[4096];

// Setup the memory space and context for a task
// gdt[4] stores the task's TSS descriptor
// gdt[5] stores the task's LDT descriptor
void task_setup() {
	memset(&task_tss, 0, sizeof(tss_t));

	// Add the task's TSS and LDT to the GDT
	gdt[4] = gdt_make_tss(&task_tss, DPL_KERNEL);
	gdt[5] = gdt_make_ldt((uint32_t)task_ldt, sizeof(task_ldt)-1, DPL_KERNEL);
	int gdt_tss_sel = gdt_entry_to_selector(&gdt[4]);
	int gdt_ldt_sel = gdt_entry_to_selector(&gdt[5]);

	// Define code and data segments in the LDT; both segments are overlapping
	int ldt_code_idx = 0;  // Index of code segment descriptor in the LDT
	int ldt_data_idx = 1;  // Index of data segment descriptor in the LDT
	uint limit = sizeof(task_addr_space);  // Limit for both code and data segments
	task_ldt[ldt_code_idx] = gdt_make_code_segment(task_addr_space, limit / 4096, DPL_USER);
	task_ldt[ldt_data_idx] = gdt_make_data_segment(task_addr_space, limit / 4096, DPL_USER);

	// Initialize the TSS fields
	// The LDT selector must point to the task's LDT
	task_tss.ldt_selector = gdt_ldt_sel;

	// Setup code and stack pointers
	task_tss.eip = 0;
	task_tss.esp = task_tss.ebp = limit;  // stack pointers

	// Code and data segment selectors are in the LDT
	task_tss.cs = GDT_INDEX_TO_SELECTOR(ldt_code_idx) | DPL_USER | LDT_SELECTOR;
	task_tss.ds = task_tss.es = task_tss.fs = task_tss.gs = task_tss.ss = GDT_INDEX_TO_SELECTOR(ldt_data_idx) | DPL_USER | LDT_SELECTOR;

	// Activate hardware interrupts (bit 9)
	// NOTE: bits 12-13 (IOPL) control ports access:
	//       - if set to 0 => only ring 0 can access IO ports
	//       - if set to 3 => rings up to 3 can access IO ports
	task_tss.eflags = (1 << 9);

	// Task's kernel stack
	task_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	task_tss.esp0 = (uint32_t)(task_kernel_stack) + sizeof(task_kernel_stack);
}
