/**
 * @file string.h
 * @brief library to use strings
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#ifndef _STRING_H_
#define _STRING_H_

#include "../types.h"

// Ref : Ce code source vient de la librairie string (https://code.woboq.org/linux/linux/lib/string.c.html)
// +------------------------------------------------------------+
void *memset(void *dst, int value, uint_t count);
void *memcpy(void *dst, void *src, uint_t count);
char *strncpy(char *dest, const char *src, uint_t count);
int strcmp(const char *p, const char *q);
int strncmp(const char *p, const char *q, uint_t count);
int strlen(const char *s);
// +------------------------------------------------------------+

#endif