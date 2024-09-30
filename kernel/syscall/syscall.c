
#include "../mem/gdt.h"
#include "../task/exec.h"
#include "../task/task.h"
#include "../vid/stdio.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "../file_system/file_system.h"
#include "../../common/types.h"
#include "../../common/syscall_nb.h"
#include "../../common/lib/string.h"

static int syscall_getc(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	return getc();
}

static int syscall_exec(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg4);
	char **argv = (char **)(base + arg2);
	for (uint32_t i = 0; i < arg3; i++)
	{
		argv[i] = (char *)(base + argv[i]);
	}
	return exec((char *)(base + arg1), argv, arg3);
}

static int syscall_set_cursor(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	set_cursor_from_x_y(arg1 & 0xffff, arg2 & 0xffff);
	return 0;
}

static int syscall_puts(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	my_printf("%s", (char *)(base + arg1));
	return 0;
}

static int syscall_read_string(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg3);
	UNUSED(arg4);
	read_string((char *)(base + arg1), arg2 & 0xffff);
	return 0;
}

static int syscall_file_stat(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg3);
	UNUSED(arg4);
	multiboot_module_t addr;
	char *filename = (char *)(base + arg1);
	if (find_file(filename, &addr))
	{
		file_stat(&addr, (stat_t *)(base + arg2));
		return 0;
	}
	else
	{
		char buf[XL_BUFFER];
		eprintf("Exception : File read, file \"%s\" not found !", COLOR_LIGHT_RED, buf, filename);
		return -1;
	}
}

static int syscall_file_read(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg3);
	UNUSED(arg4);
	multiboot_module_t addr;
	char *filename = (char *)(base + arg1);
	if (find_file(filename, &addr))
	{
		file_read(&addr, (void *)(base + arg2));
		return 0;
	}
	else
	{
		char buf[XL_BUFFER];
		eprintf("Exception : File read, file \"%s\" not found !", COLOR_LIGHT_RED, buf, filename);
		return -1;
	}
}

static int syscall_sleep(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	sleep((uint_t)arg1);
	return 0;
}

static int syscall_random(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	return (int)random((uint_t)arg1);
}

static int syscall_puts_x_y(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg4);
	puts_x_y((char *)(base + arg1), arg2 & 0xffff, arg3 & 0xffff);
	return 0;
}

static int syscall_clean_vid_x_y(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	clean_from_x_y(arg1, arg2);
	return 0;
}

static int syscall_ls(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	ls();
	return 0;
}

static int syscall_enable_cursor(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	enable_cursor();
	return 0;
}

static int syscall_disable_cursor(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base)
{
	UNUSED(arg1);
	UNUSED(arg2);
	UNUSED(arg3);
	UNUSED(arg4);
	UNUSED(base);
	disable_cursor();
	return 0;
}

// As an example, we can map syscall numbers to functions
static int (*syscall_func[])(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t base) = {
	syscall_getc,
	syscall_exec,
	syscall_set_cursor,
	syscall_puts,
	syscall_file_stat,
	syscall_file_read,
	syscall_read_string,
	syscall_sleep,
	syscall_random,
	syscall_puts_x_y,
	syscall_clean_vid_x_y,
	syscall_ls,
	syscall_enable_cursor,
	syscall_disable_cursor};

// System call handler: call the appropriate system call according to the nb argument.
// This function is called by the assembly code _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t caller_tss_selector)
{
	// caller_tss_selector allows the kernel to identify the task that made the syscall
	if (nb < __SYSCALL_END__)
	{
		uint32_t task_i = ((caller_tss_selector >> 3) - 4) / 2;
		task_t *t = &tasks[task_i];
		return (*syscall_func[nb])(arg1, arg2, arg3, arg4, (uint32_t)t->task_addr_space);
	}
	return -1;
	// Make sure to check the validity of the nb parameters.
	// Otherwise, the code might call an invalid syscall number and read a non-exististing index!
}
