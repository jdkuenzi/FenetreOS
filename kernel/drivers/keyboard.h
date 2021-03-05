#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../keyboard_conf/keyboard_ch.h"

#define KEYBOARD_DATA_REGISTER 0x60
#define KEYBOARD_STATE_REGISTER 0x64

extern void keyboard_init();
extern char getc();
#endif
