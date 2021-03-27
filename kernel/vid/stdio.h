/**
 * @file stdio.h
 * @brief library for printing on the screen 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#ifndef _STDIO_H_
#define _STDIO_H_

#include "./vid.h"
#include "../../common/types.h"
#include "../../common/lib/utils.h"

#define eprintf(fmt, color, buffer, ...) sn_printf(buffer, fmt, ##__VA_ARGS__); puts_error(buffer, color)

void printChar(char c);
void my_printf(const char *fmt, ...);
void puts_error(char *s, uint8_t color);

#endif