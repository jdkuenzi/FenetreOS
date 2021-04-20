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

#define ASCII_SPACE 0x20
#define ASCII_A 0x41
#define ASCII_Z 0x5A
#define ASCII_TO_LOWER 0x20

// Ref : Ce code source vient de la librairie string (https://code.woboq.org/linux/linux/lib/string.c.html)
// +------------------------------------------------------------+
void *memset(void *dst, int value, uint_t count);
void *memcpy(void *dst, void *src, uint_t count);
char *strncpy(char *dest, const char *src, uint_t count);
int strcmp(const char *p, const char *q);
int strncmp(const char *p, const char *q, uint_t count);
int strlen(const char *s);
char *strcat(char *dest, const char *src);
char *strpbrk(const char *cs, const char *ct);
char *strsep(char **s, const char *ct);
// +------------------------------------------------------------+
bool start_with(const char *p, const char *q);
char *to_lower(char *s);
char *trim(char *s);
#endif