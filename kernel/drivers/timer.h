#ifndef _TIMER_H_
#define _TIMER_H_

#define MIN_FREQ_HZ 19
#define MAX_FREQ_HZ 1193180

#define CMD_REG 0x43
#define CHAN_0 0x40
#define CHAN_1 0x41
#define CHAN_2 0x42 
#define REPEAT_MODE 0x36

extern void timer_init(uint32_t freq_hz);
extern uint_t get_ticks();
extern void sleep(uint_t ms);

#endif
