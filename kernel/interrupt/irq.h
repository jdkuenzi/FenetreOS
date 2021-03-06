/**
 * @file irq.h
 * @brief functions signatures to handle exceptions
 *
 * @author Florent Glück
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#ifndef _IRQ_H_
#define _IRQ_H_

#include "../../common/types.h"

typedef struct handler_st {
	void (*func)(void);
	char name[64];
} handler_t;

void install_irq_handler(uint_t number, handler_t handler);

#endif
