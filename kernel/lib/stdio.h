#ifndef _STDIO_H_
#define _STDIO_H_

#include "../vid/vid.h"
#include "utils.h"

extern vid_t vid;

void printChar(char c);
void my_printf(const char *fmt, ...);

#endif