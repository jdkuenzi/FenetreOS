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