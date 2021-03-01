/**
 * @file stdio.c
 * @brief library for printing on the screen 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#include "stdio.h"

/**
 * Print a character on the screen and set cursor position
 * @param c character to print
 */
void printChar(char c) {
	uint16_t x = get_x_from_cursor();
	uint16_t y = get_y_from_cursor();

	if (c == '\t') { // Tab
		x += 4;
	} else if (c == '\n') {	// Return
		x = 0;
		y++;
	} else if (c >= ' ') {
		write_to_x_y(x, y, c);
		x++;
	}

	if (x >= COLONNES) {	// Out of the screen
		y += x / COLONNES;
		x = x % COLONNES;
	}

	set_cursor_from_x_y(x, y);
}

/**
 * Print formatted string on the screen
 * @param fmt wished string to print
 * @param ... arguments to be printed
 */
void my_printf(const char *fmt, ...) {
	int *args = (int*)&fmt;
	while (*fmt) {
		if (*fmt == '%') {	// Format
			fmt++;
			args++;
			switch (*fmt) {
				case 'c': {		// Character
					int n = (int)*args;
					printChar((char)n);
					break;
				}
				case 's': {		// String
					my_printf((char*)*args);
					break;
				}
				case 'd': {		// Number
					char buffer[BUFFER_SIZE];
					int n = (int)*args;

					if (n < 0) {
						n = -n;
						printChar('-');
					}

					my_printf(convert((unsigned int)n, 10, buffer, BUFFER_SIZE));
					
					break;
				}
				case 'x': {		// Hex value
					char buffer[BUFFER_SIZE];
					unsigned int n = (unsigned int)*args;
					my_printf("0x%s", convert(n, 16, buffer, BUFFER_SIZE));
					break;
				}
				default: {
					break;
				}
			}
		} else {
			printChar(*fmt);
		}
		fmt++;
	}
}