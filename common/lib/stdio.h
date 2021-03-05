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

#include "../vid/vid.h"

#define BUFFER_SIZE 50

extern void printChar(char c);
extern void my_printf(const char *fmt, ...);

#endif