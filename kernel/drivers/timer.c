/**
 * @file timer.c
 * @brief Initialisation and handling of the timer
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#include "timer.h"
#include "../vid/stdio.h"
#include "../pmio/pmio.h"
#include "../interrupt/irq.h"
#include "../../common/lib/string.h"

static uint_t ticks;
static uint32_t f_hz;
static uint8_t loop_logo_ptr = 0;
static uint8_t loop_logo_size;
static char loop_logo[] = "---Fenetre_OS---";

/**
 * Displays animated logo on the top right corner
 */
static void loop_logo_display() {
	disable_cursor();

	uint8_t tmp_ptr = loop_logo_ptr;
	uint8_t x = COLONNES - loop_logo_size - 2; // -2 for '[' and ']' in my_prinft
	uint8_t old_font_color = get_font_color();
	
	set_font_color(COLOR_MAGENTA);
	write_to_x_y(x++, 0, '[');
	for (uint8_t i = 0; i < loop_logo_size; i++, tmp_ptr++)
	{
		write_to_x_y(x++, 0, loop_logo[tmp_ptr % loop_logo_size]);
	}
	write_to_x_y(x, 0, ']');
	set_font_color(old_font_color);
	enable_cursor();

	loop_logo_ptr = loop_logo_ptr + 1 % loop_logo_size;
}

/**
 * Timer interrupt handler
 * Update ticks count and display a logo (animated)
 */
static void timer_handler() {
	ticks++;
	if (!(ticks % (f_hz/5))) // display all 0.20s
	{
		loop_logo_display();
	}
}

/**
 * Initialisation of the timer with a frequency
 * 
 * @param freq_hz wished frequency in Hz
 */
void timer_init(uint32_t freq_hz) {
	ticks = 0;
	f_hz = freq_hz;
	loop_logo_size = strlen(loop_logo);

	uint16_t div = 11932;

	outw(CMD_REG, REPEAT_MODE);
	outb(CHAN_0, div & 0xff);
	outb(CHAN_0, (div >> 8) & 0xff);
	
	// Install the timer interrupt handler
	handler_t handler = { timer_handler, "timer" };
	install_irq_handler(0, handler);
	loop_logo_display();
}

/**
 * Get the ticks of the timers
 * 
 * @return uint_t ticks of the timer
 */
uint_t get_ticks() {
	return ticks;
}

uint_t random(uint_t max) {
	return ((get_ticks() * 1103515245 + 12345) / 65536) % max;
}

/**
 * Stop the system during some ms
 * 
 * @param ms time of sleep
 */
void sleep(uint_t ms) {
	uint_t count = f_hz * ms / 1000;
	count += get_ticks();
	while (get_ticks() <= count) { /* nothing to do */ }
}
