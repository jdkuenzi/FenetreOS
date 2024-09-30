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
static uint8_t seed = 0;
static uint16_t random_list[NB_RANDOM] = {53721, 12805, 44258, 39641, 35332, 39403, 65510, 27102, 25786, 43034,
										  40662, 30109, 11681, 1676, 57698, 32015, 36449, 59347, 27551, 13560, 50458, 58227, 13255, 54339, 7822,
										  13792, 13793, 28642, 7112, 23088, 707, 47767, 44145, 36056, 21858, 2943, 51709, 31584, 39803, 54789,
										  14151, 29329, 1774, 36883, 56958, 27437, 25851, 2412, 19195, 44233, 5698, 16039, 7332, 64522, 18680,
										  53120, 60, 63808, 62420, 56288, 6165, 42270, 30430, 30658, 12007, 8721, 51157, 43251, 21762, 59712,
										  48056, 11889, 16967, 35703, 37000, 46209, 6607, 49410, 26445, 58476, 45992, 4071, 32140, 53524, 51051,
										  7701, 37541, 3264, 13246, 37302, 43712, 15660, 44594, 60453, 65457, 45284, 26759, 7377, 16952, 40861,
										  35793, 35254, 37950, 18633, 49299, 21315, 37834, 51259, 28004, 34464, 22957, 47181, 15209, 31496, 4006,
										  47216, 2570, 39467, 5713, 43615, 48618, 65270, 2987, 39435, 64722, 11291, 27271, 56703, 62032, 39177,
										  54707, 11209, 44318, 58802, 55050, 4045, 12450, 54400, 39804, 2970, 17751, 41792, 33130, 34648, 27516,
										  55427, 32325, 13688, 38217, 6369, 50556, 17082, 46572, 45894, 44601, 21162, 52788, 1304, 36453, 44229,
										  21469, 46866, 19784, 62182, 64712, 12121, 48542, 52910, 32976, 31650, 39906, 39672, 57815, 7369, 19448,
										  18418, 41169, 33302, 49052, 39772, 53801, 48412, 63210, 54535, 52853, 32312, 47323, 46984, 45490, 57017,
										  12900, 30659, 40847, 37244, 30000, 18620, 16205, 49579, 51852, 39269, 23614, 64491, 17183, 48571, 65081,
										  25135, 16966, 41211, 55466, 13563, 55285, 31961, 57703, 26801, 48726, 634, 15338, 37325, 43873, 154,
										  44013, 58651, 52421, 6296, 12135, 1533, 31472, 60466, 31593, 39349, 5131, 10075, 4395, 40974, 21162,
										  39219, 15981, 36464, 17644, 33520, 61791, 34787, 43609, 10122, 30766, 22273, 34008, 50123, 8172, 33876,
										  57959, 46008, 8037, 7781, 31659};
static uint32_t f_hz;
static uint8_t loop_logo_ptr = 0;
static char loop_logo[] = "---Fenetre_OS---";
static uint8_t loop_logo_size = 1;
static uint8_t loop_logo_x_start = COLONNES - LOOP_LOGO_SIZE - 2; // -2 for '[' and ']' in my_printf

/**
 * Displays animated logo on the top right corner
 */
static void loop_logo_display()
{
	uint8_t tmp_ptr = loop_logo_ptr;
	uint8_t old_font_color = get_font_color();
	uint8_t x = loop_logo_x_start;

	set_font_color(COLOR_MAGENTA);
	write_to_x_y(x++, 0, '[');
	for (uint8_t i = 0; i < loop_logo_size; i++, tmp_ptr++)
	{
		write_to_x_y(x++, 0, loop_logo[tmp_ptr % loop_logo_size]);
	}
	write_to_x_y(x, 0, ']');
	set_font_color(old_font_color);

	loop_logo_ptr = loop_logo_ptr + 1 % loop_logo_size;
}

/**
 * Timer interrupt handler
 * Update ticks count and display a logo (animated)
 */
static void timer_handler()
{
	ticks++;
	if (!(ticks % (f_hz / 5))) // display all 0.20s
	{
		loop_logo_display();
	}
}

/**
 * Initialisation of the timer with a frequency
 * 
 * @param freq_hz wished frequency in Hz
 */
void timer_init(uint32_t freq_hz)
{
	ticks = 0;
	f_hz = freq_hz;
	loop_logo_size = strlen(loop_logo);

	uint16_t div = MAX_FREQ_HZ / freq_hz;

	outw(CMD_REG, REPEAT_MODE);
	outb(CHAN_0, div & 0xff);
	outb(CHAN_0, (div >> 8) & 0xff);

	// Install the timer interrupt handler
	handler_t handler = {timer_handler, "timer"};
	install_irq_handler(0, handler);
	loop_logo_display();
}

/**
 * Get the ticks of the timers
 * 
 * @return uint_t ticks of the timer
 */
uint_t get_ticks()
{
	return ticks;
}

uint_t random(uint_t max)
{
	uint_t temp = (random_list[seed] + get_ticks()) % max;
	seed++;
	return temp;
}

/**
 * Stop the system during some ms
 * 
 * @param ms time of sleep
 */
void sleep(uint_t ms)
{
	uint_t count = f_hz * ms / 1000;
	count += get_ticks();
	while (get_ticks() <= count)
	{ /* nothing to do */
	}
}
