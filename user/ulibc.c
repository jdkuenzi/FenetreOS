#include "ulibc.h"

int set_cursor_from_x_y(uint32_t x, uint32_t y) {
    return syscall(SYSCALL_SET_CUSRSOR, x, y, 0, 0);
}

int puts(char* s) {
    return syscall(SYSCALL_PUTS, (uint32_t)s, 0, 0, 0);
}

int my_printf(const char *fmt, ...) {
    uint32_t *args = (uint32_t*)&fmt;
    return syscall(SYSCALL_PUTS, *args, 0, 0, 0);
}