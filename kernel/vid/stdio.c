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

#include "./stdio.h"

/**
 * Print a character on the screen and set cursor position
 * @param c character to print
 */
void printChar(char c) {
	if (c == '\t') { // Tab
		write_tab();
	} else if (c == '\n') {	// Return
		write_line_break();
	} else if (c >= ' ') {
		write_to_current_pos(c);
	}
}

void puts_error(char *s, uint8_t color) {
	disable_cursor();
	uint8_t old_color = get_font_color();
	set_font_color(color);
	printChar('\n');
	my_printf(s);
	set_font_color(old_color);
	printChar('\n');
	enable_cursor();
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
					char buffer[XS_BUFFER];
					int n = (int)*args;

					if (n < 0) {
						n = -n;
						printChar('-');
					}

					my_printf(convert((unsigned int)n, 10, buffer, XS_BUFFER));
					
					break;
				}
				case 'x': {		// Hex value
					char buffer[XS_BUFFER];
					unsigned int n = (unsigned int)*args;
					my_printf("0x%s", convert(n, 16, buffer, XS_BUFFER));
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