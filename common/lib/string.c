/**
 * @file string.c
 * @brief library to use strings (array of char)
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#include "string.h"

// Ref : Ce code source vient de la librairie string (https://code.woboq.org/linux/linux/lib/string.c.html)
// +------------------------------------------------------------+

/**
 * Fill the memory with a costant
 * @param dst destination pointer of the memory
 * @param value costant for fill
 * @param count number of bytes to fill
 * @return pointer to the destination
 */
void *memset(void *dst, int value, uint_t count) {
	char *d = dst;
	while (count--)
	{
		*d++ = value;
	}
	return dst;
}

/**
 * Copy memory zone
 * @param dst destination zone
 * @param src zone to be copied
 * @param count number of bytes that needs to be copied
 * @return pointer to the destination zone
 */
void *memcpy(void *dst, void *src, uint_t count) {
	char *copySrc = src;
	char *copyDst = dst;
	while (count--)
	{
		*copyDst++ = *copySrc++;
	}
	return dst;
}

/**
 * Copy the first n bytes of a string
 * @param dest destination zone
 * @param src zone to be coiped
 * @param count number of bytes that have to be copied
 * @return pointer to the destination zone
 */
char *strncpy(char *dest, const char *src, uint_t count) {
	char *tmp = dest;
	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

/**
 * Compare two strings
 * @param p first string
 * @param q second string
 * @return int 0 if equal, -1 if first < second, 1 if first > second
 */
int strcmp(const char *p, const char *q) {
	unsigned char c1, c2;
	while (1) {
		c1 = *p++;
		c2 = *q++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}

/**
 * Compare n bytes of two strings
 * @param p first string
 * @param q second string
 * @param count number of bytes
 * @return int 0 if equal, -1 if first < second, 1 if first > second
 */
int strncmp(const char *p, const char *q, uint_t count) {
	unsigned char c1, c2;
	while (count) {
		c1 = *p++;
		c2 = *q++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

/**
 * strcat - Append one %NUL-terminated string to another
 * @dest: The string to be appended to
 * @src: The string to append to it
 */
char *strcat(char *dest, const char *src) {
	char *tmp = dest;
	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;
	return tmp;
}
// +------------------------------------------------------------+

/**
 * Compare if the string q starts well with the string p
 * @param p start string
 * @param q string to compare
 * @return int 0 if equal, -1 if first < second, 1 if first > second
 */
bool start_with(const char *p, const char *q) {
	return (strncmp(p, q, strlen(p)) == 0);
}

/**
 * Remove spaces at the start and end of a string
 * @param s string to trim
 * @return s
 */
char *trim(char *s) {
	char *start = s;
	char *end = s + strlen(s) - 1;

	while(*start == ASCII_SPACE) { 
		start++; 
		if (start == end) {
			return "";
		}	
	}

	while(*end == ASCII_SPACE) { end--; }

	uint_t size = end - start + 1;

	memcpy(s, start, size);
	s[size] = '\0';
	return s;
}

/**
 * Put the string in lowercase
 * @param s string to lower
 * @return s
 */
char *to_lower(char *s) {
	char *ptr = s;
	while (*ptr)
	{
		if (*ptr >= ASCII_A && *ptr <= ASCII_Z) {
			*ptr = *ptr + ASCII_TO_LOWER; 
		}
		ptr++;
	}
	return s;
}

/**
 * Calculate length of a string
 * @param s string for count
 * @return int length of the string
 */
int strlen(const char *s) {
	const char *sc;
	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}