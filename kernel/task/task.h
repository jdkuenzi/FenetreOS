#ifndef _TASK_H_
#define _TASK_H_
#include "../../common/types.h"
#include "../mem/gdt.h"
#include "tss.h"

#define TASKS_SIZE 8
#define TASK_ADDR_SPACE 1048576
#define TASK_KERNEL_STACK 65536

typedef struct task {
    // TSS of the task
    tss_t task_tss;
    
    // Define a LDT for the task; this LDT defines 2 segments
    gdt_entry_t task_ldt[2];

    // Allocate 1MB of address space for the task.
    // The address space MUST be aligned to 4096 bytes (4KB)!
    uint8_t task_addr_space[TASK_ADDR_SPACE] __attribute__((aligned(4096)));

    // Allocate 64KB of kernel stack for the task
    uint8_t task_kernel_stack[TASK_KERNEL_STACK];

    int gdt_tss_sel;
    int limit;
    bool is_available;
} task_t;

extern void task_ltr(uint16_t tss_selector);     // Implemented in task_asm.s
extern void task_switch(uint16_t tss_selector);  // implemented in task_asm.s
void init_tasks(int n, int start_i);
void clean_task(task_t *task);

task_t tasks[TASKS_SIZE];

#endif