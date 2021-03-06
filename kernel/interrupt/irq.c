/**
 * @file irq.c
 * @brief handle exceptions
 *
 * @author Florent Glück
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#include "irq.h"
#include "idt.h"

handler_t irq_handlers[IRQ_COUNT];

void install_irq_handler(uint_t number, handler_t handler) {
	irq_handlers[number] = handler;
}
