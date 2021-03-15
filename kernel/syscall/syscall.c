#include "syscall.h"

// To avoid warnings when an argument is not used:
// void f(int arg) {
//     UNUSED(arg);
//     ...
// }
// Alternatively use gcc' syntax:
// void (__attribute__ ((unused)) int arg) { ... }

// As an example, we can map syscall numbers to functions
static int (*syscall_func[])(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base) = {
	syscall_puts,
	syscall_exec,
	...
}

// System call handler: call the appropriate system call according to the nb argument.
// This function is called by the assembly code _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t caller_tss_selector) {
	
	// caller_tss_selector allows the kernel to identify the task that made the syscall
	
	// Make sure to check the validity of the nb parameters.
    // Otherwise, the code might call an invalid syscall number and read a non-exististing index!
}
