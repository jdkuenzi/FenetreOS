#include "keyboard.h"
#include "../../common/pmio/pmio.h"
#include "../interrupt/irq.h"

static bool shift_is_pressed = false;
static uint8_t scan_code;

static void keyboard_handler() {
	if (inb(KEYBOARD_STATE_REGISTER) & 0x1)
	{
		scan_code = inb(KEYBOARD_DATA_REGISTER);
		if (!(scan_code & 0x80)) // make code
		{
			if (scan_code == L_SHIFT || scan_code == R_SHIFT) {
				shift_is_pressed = true;
			}	
		} else { // break code
			if ((scan_code & 0x7F) == L_SHIFT || (scan_code & 0x7F) == R_SHIFT) {
				shift_is_pressed = false;
			}
			scan_code = 0;
		}
	}
	
}

void keyboard_init() {
	// TODO
	keyboard_conf_init();

	// Install the keyboard interrupt handler
	handler_t handler = { keyboard_handler, "keyboard" };
	install_irq_handler(1, handler);
}

// Return the character that was pressed on the keyboard or 0 if no character is present in the internal buffer.
// This function never blocks.
char getc() {
	char c = 0;
	if (scan_code) {
		if (shift_is_pressed) {
			c = shift_keyboard_conf[scan_code];
		}
		else {
			c = keyboard_conf[scan_code];
		}
		scan_code = 0;
	}
	return c;
}
