#include "irq.h"
#include "idt.h"

handler_t irq_handlers[IRQ_COUNT];

void install_irq_handler(uint_t number, handler_t handler) {
	irq_handlers[number] = handler;
}
