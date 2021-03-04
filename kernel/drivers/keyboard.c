#include "../../common/types.h"
#include "keyboard.h"
#include "../interrupt/irq.h"

static void keyboard_handler() {
	// TODO
}

void keyboard_init() {
	// TODO

	// Install the keyboard interrupt handler
	// TODO
}

// Return the character that was pressed on the keyboard or 0 if no character is present in the internal buffer.
// This function never blocks.
int getc() {
	// TODO
	return 0;
}
