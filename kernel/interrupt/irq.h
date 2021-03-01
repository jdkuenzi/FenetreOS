#ifndef _IRQ_H_
#define _IRQ_H_

#include "../../common/types.h"

typedef struct handler_st {
	void (*func)(void);
	char name[64];
} handler_t;

void install_irq_handler(uint_t number, handler_t handler);

#endif
