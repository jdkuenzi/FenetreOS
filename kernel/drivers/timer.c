#include "../../common/types.h"
#include "timer.h"
#include "../interrupt/irq.h"
#include "../pmio/pmio.h"
#include "../x86.h"
#include "../lib/stdio.h"

static uint_t ticks;
static uint32_t f_hz;
static uint8_t loop_logo_ptr = 0;
static char loop_logo[] = "---Fenetre_OS---";

static void loop_logo_display() {
	disable_cursor();

	uint8_t tmp_ptr = loop_logo_ptr;
	uint8_t size = strlen(loop_logo);
	uint8_t x = COLONNES - size - 2; // -2 for '[' and ']' in my_prinft
	uint8_t old_font_color = get_font_color();
	uint16_t old_pos = get_cursor();
	char reconstruct_loop_logo[size+1]; // size + 1 for \0
	
	set_font_color(COLOR_MAGENTA);
	set_cursor_from_x_y(x, 0);
	for (uint8_t i = 0; i < size; i++)
	{
		reconstruct_loop_logo[i] = loop_logo[tmp_ptr % size];
		tmp_ptr++;
	}
	reconstruct_loop_logo[size] = '\0';
	my_printf("[%s]", reconstruct_loop_logo);
	set_cursor_from_pos(old_pos);
	set_font_color(old_font_color);

	enable_cursor();
	loop_logo_ptr = loop_logo_ptr + 1 % size;
}

// Timer interrupt handler
// Update ticks count and display a logo (animated)
static void timer_handler() {
	ticks++;
	if (!(ticks % (f_hz/5))) // display all 0.20s
	{
		loop_logo_display();
	}
}

void timer_init(uint32_t freq_hz) {
	ticks = 0;
	f_hz = freq_hz;

	uint16_t div = MAX_FREQ_HZ / freq_hz;

	outw(CMD_REG, REPEAT_MODE);
	outb(CHAN_0, div & 0xff);
	outb(CHAN_0, (div >> 8) & 0xff);
	
	// Install the timer interrupt handler
	handler_t handler = { timer_handler, "timer" };
	install_irq_handler(0, handler);
	loop_logo_display();
}

uint_t get_ticks() {
	return ticks;
}

void sleep(uint_t ms) {
	uint_t count = f_hz * ms / 1000;
	count += get_ticks();
	while (get_ticks() <= count) { /* nothing to do */ }
}
