#include "stdio.h"

void printChar(char c) {
	uint16_t x = get_x_from_cursor();
	uint16_t y = get_y_from_cursor();

	if (c == '\t') { // Tab
		x += 4;
	} else if (c == '\n') {	// Return
		x = 0;
		y++;
	} else if (c >= ' ') {
		write_to_x_y(&vid, x, y, c);
		x++;
	}

	if (x >= COLONNES) {
		y += x / COLONNES;
		x = x % COLONNES;
	}

	set_cursor_from_x_y(x, y);
}

void my_printf(const char *fmt, ...) {
	int *args = (int*)&fmt;
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			args++;
			char buffer[50];
			switch (*fmt) {
				case 'c': {
					int n = (int)*args;
					printChar((char)n);
					break;
				}
				case 's': {
					my_printf((char*)*args);
					break;
				}
				case 'd': {
					int n = (int)*args;

					if (n < 0) {
						n = -n;
						printChar('-');
					}

					my_printf(convert((unsigned int)n, 10, buffer));
					
					break;
				}
				case 'x': {
					unsigned int n = (unsigned int)*args;
					my_printf("0x%s", convert(n, 16, buffer));
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