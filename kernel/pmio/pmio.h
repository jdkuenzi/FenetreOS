/*
 * Author: Florent Glück
 * Desc  : Code source venant du TP hyperviseur effectué dans le cadre du cours virtualisation à HEPIA
*/


#ifndef _PMIO_H_
#define _PMIO_H_

#include "../../common/types.h"

extern void outb(uint16_t port, uint8_t data);
extern uint8_t inb(uint16_t port);

extern void outw(uint16_t port, uint16_t data);
extern uint16_t inw(uint16_t port);

#endif
