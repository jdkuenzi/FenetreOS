#include "task.h"
#include "../../common/lib/string.h"
#include "../descriptors.h"

task_t tasks[TASKS_SIZE];

static int task_idx = 0;
static int ldt_code_idx = 0; // Index of code segment descriptor in the LDT
static int ldt_data_idx = 1; // Index of data segment descriptor in the LDT

static void init_task(uint8_t tss_i, uint8_t ldt_i) {

	memset(tasks[task_idx].task_addr_space, 0, sizeof(tasks[task_idx].task_addr_space));
	tasks[task_idx].is_available = true;
	tasks[task_idx].argc = 0;

	tasks[task_idx].tss_i = tss_i;
	tasks[task_idx].ldt_i = ldt_i;
	memset(&tasks[task_idx].task_tss, 0, sizeof(tss_t));
	// Add the task's TSS and LDT to the GDT
	gdt[tss_i] = gdt_make_tss(&tasks[task_idx].task_tss, DPL_KERNEL);
	gdt[ldt_i] = gdt_make_ldt((uint32_t)tasks[task_idx].task_ldt, sizeof(tasks[task_idx].task_ldt) - 1, DPL_KERNEL);
	int gdt_tss_sel = gdt_entry_to_selector(&gdt[tss_i]);
	int gdt_ldt_sel = gdt_entry_to_selector(&gdt[ldt_i]);

	tasks[task_idx].gdt_tss_sel = gdt_tss_sel;

	// Define code and data segments in the LDT; both segments are overlapping
	tasks[task_idx].limit = sizeof(tasks[task_idx].task_addr_space); // Limit for both code and data segments
	tasks[task_idx].task_ldt[ldt_code_idx] = gdt_make_code_segment((uint32_t)tasks[task_idx].task_addr_space, tasks[task_idx].limit / 4096, DPL_USER);
	tasks[task_idx].task_ldt[ldt_data_idx] = gdt_make_data_segment((uint32_t)tasks[task_idx].task_addr_space, tasks[task_idx].limit / 4096, DPL_USER);

	// Initialize the TSS fields
	// The LDT selector must point to the task's LDT
	tasks[task_idx].task_tss.ldt_selector = gdt_ldt_sel;

	// Setup code and stack pointers
	tasks[task_idx].task_tss.eip = 0;
	tasks[task_idx].task_tss.esp = tasks[task_idx].task_tss.ebp = tasks[task_idx].limit; // stack pointers

	// Code and data segment selectors are in the LDT
	tasks[task_idx].task_tss.cs = GDT_INDEX_TO_SELECTOR(ldt_code_idx) | DPL_USER | LDT_SELECTOR;
	tasks[task_idx].task_tss.ds = tasks[task_idx].task_tss.es = tasks[task_idx].task_tss.fs = tasks[task_idx].task_tss.gs = tasks[task_idx].task_tss.ss = GDT_INDEX_TO_SELECTOR(ldt_data_idx) | DPL_USER | LDT_SELECTOR;

	// Activate hardware interrupts (bit 9)
	// NOTE: bits 12-13 (IOPL) control ports access:
	//       - if set to 0 => only ring 0 can access IO ports
	//       - if set to 3 => rings up to 3 can access IO ports
	tasks[task_idx].task_tss.eflags = (1 << 9);

	// Task's kernel stack
	tasks[task_idx].task_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	tasks[task_idx].task_tss.esp0 = (uint32_t)(tasks[task_idx].task_kernel_stack) + sizeof(tasks[task_idx].task_kernel_stack);
	
	task_idx++;
}

void clean_task(task_t *task) {
	memset(task->task_addr_space, 0, sizeof(task->task_addr_space));
	task->argc = 0;

	task->task_tss.eip = 0;
	task->task_tss.esp = task->task_tss.ebp = task->limit; // stack pointers

	task->task_tss.eflags = (1 << 9);
	task->task_tss.esp0 = (uint32_t)(task->task_kernel_stack) + sizeof(task->task_kernel_stack);

	task->is_available = true;
}

void init_tasks(int n, int start_i) {
	for (int i = start_i; i < (n * 2) + start_i; i += 2) 
	{
		init_task(i, i+1);
	}
}