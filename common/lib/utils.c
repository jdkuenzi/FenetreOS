/**
 * @file utils.c
 * @brief utility functions library
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#include "utils.h"

// Inspiré d'un code internet
// Ref : http://www.firmcodes.com/write-printf-function-c/
/**
 * Convert number to string 
 * @param num number to convert
 * @param base base to convert (ex: 10 (Dec), 16 (Hex))
 * @param buffer buffer to fill
 * @param size size of the buffer
 * @return char* string buffer after conversion
 */
char *convert(unsigned int num, int base, char *buffer, unsigned int size) { 
	static char representation[]= "0123456789ABCDEF";
	
    buffer = &buffer[size-1];
	*buffer = '\0';
	
    do 
	{ 
		*--buffer = representation[num%base];
		num /= base;
	} while (num);

	return buffer;
}

int atoi(char *s) {
	bool sign = false;
	int res = 0;

	if (*s == '-')
	{
		sign = true;
		s++;
	} else if (*s == '+') {
		s++;
	}

	while (*s)
	{
		uint_t d = *s - '0';
		res = 10 * res + d;
		s++; 
	}

	if (sign)
	{
		res = -res;
	}

	return res;
}

/**
 * Build formatted string
 * @param s buffer to store the string
 * @param fmt wished string to print
 * @param ... arguments to be printed
 */
char *sn_printf(char *s, const char *fmt, ...) {
	char *ptr = s;
	int *args = (int*)&fmt;
	while (*fmt) {
		if (*fmt == '%') {	// Format
			fmt++;
			args++;
			switch (*fmt) {
				case 'c': {		// Character
					int n = (int)*args;
					*ptr++ = (char)n;
					break;
				}
				case 's': {		// String
					ptr = sn_printf(ptr, (char*)*args);
					break;
				}
				case 'd': {		// Number
					char buffer[XS_BUFFER];
					int n = (int)*args;

					if (n < 0) {
						n = -n;
						*ptr++ = '-';
					}

					ptr = sn_printf(ptr, convert((unsigned int)n, 10, buffer, XS_BUFFER));
					
					break;
				}
				case 'x': {		// Hex value
					char buffer[XS_BUFFER];
					unsigned int n = (unsigned int)*args;
					ptr = sn_printf(ptr, "0x%s", convert(n, 16, buffer, XS_BUFFER));
					break;
				}
				default: {
					break;
				}
			}
		} else {
			*ptr++ = *fmt;
		}
		fmt++;
	}
	*ptr = '\0';
	return ptr;
}