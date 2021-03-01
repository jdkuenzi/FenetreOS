#include "../../common/types.h"
#include "timer.h"
#include "../interrupt/irq.h"
#include "../pmio/pmio.h"
#include "../x86.h"
#include "../lib/stdio.h"

static uint_t ticks;
static uint32_t f_hz;

// Timer interrupt handler
// Update ticks count and display a logo (animated)
static void timer_handler() {
	ticks++;
	if (ticks%50 == 0) {
		my_printf("\nFreq\n");
	}
}

void timer_init(uint32_t freq_hz) {
	ticks = 0;
	f_hz = freq_hz;

	uint16_t div = MAX_FREQ_HZ / freq_hz;

	outw(CHAN_0, REPEAT_MODE);
	outb(CHAN_0, div & 0xff);
	outb(CHAN_0, (div >> 8) & 0xff);
	
	// Install the timer interrupt handler
	handler_t handler = { timer_handler, "timer" };
	install_irq_handler(0, handler);
}

uint_t get_ticks() {
	return ticks;
}

void sleep(uint_t ms) {
	uint_t count = f_hz * ms / 1000 + get_ticks();
	while (get_ticks() < count)
	{
		/* nothing to do */
	}
}
