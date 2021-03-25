#ifndef _ULIBC_H_
#define _ULIBC_H_

#include "../common/types.h"
#include "../common/syscall_nb.h"

extern int set_cursor_from_x_y(uint32_t x, uint32_t y);
extern int puts(char* s);
extern int my_printf(const char *fmt, ...);
extern int syscall(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

#endif