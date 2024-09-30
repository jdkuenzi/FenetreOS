/**
 * @file keyboard.c
 * @brief Manage keyboard interruptions
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#include "keyboard.h"
#include "../interrupt/irq.h"
#include "../pmio/pmio.h"
#include "../vid/stdio.h"
#include "../../common/lib/string.h"

static bool shift_is_pressed = false;
static uint8_t scan_code_buffer[KEYBOARD_BUFFER_SIZE];
static uint8_t ptr_insert_buffer;
static uint8_t ptr_read_buffer;

/**
 * Reset the keyboard buffer
 */
static void reset_scan_code_buffer()
{
	ptr_insert_buffer = 0;
	ptr_read_buffer = 0;
	memset(scan_code_buffer, 0, KEYBOARD_BUFFER_SIZE * sizeof(uint8_t));
}

/**
 * Handle press on keyboard
 */
static void keyboard_handler()
{
	if (inb(KEYBOARD_STATE_REGISTER) & 0x1)
	{
		uint8_t scan_code = inb(KEYBOARD_DATA_REGISTER);
		if (!(scan_code & 0x80)) // make code
		{
			if (scan_code == L_SHIFT || scan_code == R_SHIFT)
			{
				shift_is_pressed = true;
			}
			else if (ptr_insert_buffer < KEYBOARD_BUFFER_SIZE)
			{
				scan_code_buffer[ptr_insert_buffer] = scan_code;
				ptr_insert_buffer++;
			}
			else
			{
				puts_error("Exception : Keyboard buffer is full !", COLOR_LIGHT_RED);
			}
		}
		else
		{ // break code
			if ((scan_code & 0x7F) == L_SHIFT || (scan_code & 0x7F) == R_SHIFT)
			{
				shift_is_pressed = false;
			}
			scan_code = 0;
		}
	}
}

/**
 * Initialise keyboard buffer and interruption handler
 */
void keyboard_init()
{
	reset_scan_code_buffer();
	keyboard_conf_init();

	// Install the keyboard interrupt handler
	handler_t handler = {keyboard_handler, "keyboard"};
	install_irq_handler(1, handler);
}

/**
 * Get the character pressed on the keyboard or 0 if no character is present in the internal buffer
 * This function never blocks.
 * @return uint8_t pressed character code or 0
 */
uint8_t getc()
{
	uint8_t c = 0;
	uint8_t scan_code = scan_code_buffer[ptr_read_buffer];
	if (scan_code)
	{
		if (shift_is_pressed)
		{
			c = shift_keyboard_conf[scan_code];
		}
		else
		{
			c = keyboard_conf[scan_code];
		}
		ptr_read_buffer++;
		if (ptr_read_buffer == ptr_insert_buffer)
		{
			reset_scan_code_buffer();
		}
	}
	return c;
}

/**
 * Allows you to manipulate the keyboard by filling a buffer with the keys entered
 * @param buf write buffer
 * @param size size of buffer
 */
void read_string(char *buf, uint16_t size)
{
	uint16_t i = 0;
	uint8_t c;
	memset(buf, '\0', size * sizeof(char));
	while (1)
	{
		if ((c = getc()))
		{
			if (c == ASCII_ESC)
			{
				/* Nothing to do */
			}
			else if (c == ASCII_TAB)
			{
				if (i + 3 < size - 1)
				{
					printChar('\t');
					buf[i] = ' ';
					buf[i++] = ' ';
					buf[i++] = ' ';
					buf[i++] = ' ';
				}
			}
			else if (c == ASCII_SHIFT_IN)
			{
				/* Nothing to do */
			}
			else if (c == ASCII_SPACE)
			{
				printChar(' ');
				buf[i++] = ' ';
			}
			else if (c == ASCII_ENTER)
			{
				buf[i] = '\0';
				break;
			}
			else if (c == ASCII_BACKSPACE)
			{
				if (i > 0)
				{
					backspace();
					buf[--i] = '\0';
				}
			}
			else if (c == ASCII_UP_ARROW)
			{
				/* Nothing to do */
			}
			else if (c == ASCII_LEFT_ARROW)
			{
				if (i > 0)
				{
					move_cursor_left();
					i--;
				}
			}
			else if (c == ASCII_RIGHT_ARROW)
			{
				if (buf[i] > '\0')
				{
					move_cursor_right();
					i++;
				}
			}
			else if (c == ASCII_DOWN_ARROW)
			{
				/* Nothing to do */
			}
			else
			{
				printChar(c);
				buf[i++] = c;
			}

			if (i >= size - 1)
			{
				buf[i] = '\0';
				break;
			}
		}
	}
}