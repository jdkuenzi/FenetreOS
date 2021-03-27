#ifndef _ULIBC_H_
#define _ULIBC_H_

#include "../common/types.h"
#include "../common/syscall_nb.h"
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

#endif