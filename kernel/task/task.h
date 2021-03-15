#ifndef _TASK_H_
#define _TASK_H_

extern void task_ltr(uint16_t tss_selector);     // Implemented in task_asm.s
extern void task_switch(uint16_t tss_selector);  // implemented in task_asm.s

#endif
