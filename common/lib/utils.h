/**
 * @file utils.h
 * @brief utility functions library
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "../types.h"

#define UTILS_BUFFER_SIZE 50

extern char *convert(unsigned int num, int base, char *buffer, unsigned int size);
int atoi(char *s);
char *sn_printf(char *s, const char *fmt, ...);

#endif