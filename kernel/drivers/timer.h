/**
 * @file timer.h
 * @brief Initialisation and handling of the timer
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "../../common/types.h"

#define MIN_FREQ_HZ 19
#define MAX_FREQ_HZ 1193180
#define NB_RANDOM 255
#define LOOP_LOGO_SIZE 16

#define CMD_REG 0x43
#define CHAN_0 0x40
#define CHAN_1 0x41
#define CHAN_2 0x42
#define REPEAT_MODE 0x36

extern void timer_init(uint32_t freq_hz);
extern uint_t get_ticks();
uint_t random(uint_t max);
extern void sleep(uint_t ms);

#endif
