#ifndef _ULIBC_H_
#define _ULIBC_H_

#include "../common/types.h"
#include "../common/syscall_nb.h"
<<<<<<< HEAD
#include "../common/lib/string.h"
#include "../common/lib/utils.h"

#define ULIBC_SM_BUFFER 128
#define ULIBC_MD_BUFFER 256
#define ULIBC_LG_BUFFER 512
#define ULIBC_XL_BUFFER 1024

#define printf(fmt, buffer, ...) sn_printf(buffer, fmt, ##__VA_ARGS__); puts(buffer)

int setcursor(uint32_t x, uint32_t y);
int puts(char *s);
int exec(char *filename);
int exec(char *filename);
char getc();
int read_string(char *buf, uint16_t size);
int filesread(char *filename, void *buf);
int filestat(char *filename, stat_t *stat);
uint_t random(uint_t max);
int cat(char *filename);
int sleep(uint_t ms);
void exit();
=======

extern int set_cursor_from_x_y(uint32_t x, uint32_t y);
extern int puts(char* s);
extern int my_printf(const char *fmt, ...);
extern int syscall(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
>>>>>>> cd5dcb4d628bac7fd9c7f1b8404d803de924ba65

#endif