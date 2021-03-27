/**
 * @file keyboard.h
 * @brief Manage keyboard interruptions
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "../../common/types.h"
#include "../keyboard_conf/keyboard_ch.h"

#define KEYBOARD_BUFFER_SIZE 8
#define KEYBOARD_DATA_REGISTER 0x60
#define KEYBOARD_STATE_REGISTER 0x64

extern void keyboard_init();
extern uint8_t getc();
void read_string(char *buf, uint16_t size);

#endif
