#include "ulibc.h"

// Entry point of a user application
void main() {
	set_cursor_from_x_y(0, 24);
	puts("\tJe vais tester des choses !\n");
	// my_printf("%d = %x = %c\n", 65, 65, 65);
}