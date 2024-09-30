#ifndef _ULIBC_H_
#define _ULIBC_H_

#include "../common/types.h"
#include "../common/syscall_nb.h"
#include "../common/lib/string.h"
#include "../common/lib/utils.h"

#define printf(fmt, buffer, ...)           \
    sn_printf(buffer, fmt, ##__VA_ARGS__); \
    puts(buffer)

int setcursor(uint32_t x, uint32_t y);
int enable_cursor();
int disable_cursor();
int puts(char *s);
int clean_vid_x_y(uint32_t x, uint32_t y);
int puts_x_y(char *s, uint32_t x, uint32_t y);
int ls();
int exec(char *filename, char *argv[], int argc);
char getc();
int read_string(char *buf, uint16_t size);
int filesread(char *filename, void *buf);
int filestat(char *filename, stat_t *stat);
uint_t random(uint_t max);
int cat(char *filename);
int sleep(uint_t ms);
void exit();

#endif