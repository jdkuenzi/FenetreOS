/**
 * @file vid.c
 * @brief functions to display the cursor and initialise the screen 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.2
 */

#include "./vid.h"
#include "../pmio/pmio.h"
#include "../../common/lib/string.h"

static vid_t vid;

/**
 * Initialise the cursor with rectangle shape
 */
static void init_rec_cursor(void)
{
	outb(CMD_REGISTER, 0xA);   // Write to command register 0xA
	outb(DATA_REGISTER, 0x0);  // Write to data register start value of cursor at 0
	outb(CMD_REGISTER, 0xB);   // Write to command register 0xB
	outb(DATA_REGISTER, 0x1F); // Write to data register end value of cursor at 31
}

/**
 * Enable the cursor on the screen
 */
void enable_cursor(void)
{
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x0);
}

/**
 * Disable the cursor on the screen
 */
void disable_cursor(void)
{
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x10);
}

/**
 * Scroll the screen
 */
static void scroll()
{
	uint_t count = COLONNES * LIGNES - COLONNES * 2; // - COLONNES * 2 because we don't copy the first two rows
	uint16_t buffer[count];
	memcpy(buffer, vid.vidptr + COLONNES * 2, count * sizeof(uint16_t));
	clean_from_x_y(0, 1);
	memcpy(vid.vidptr + COLONNES, buffer, count * sizeof(uint16_t)); // + COLONNES because we never write on the first row
}

/**
 * Set the position of the cursor from a value and scroll if needed
 * @param pos wished position
 */
static void set_cursor_from_pos(uint16_t pos)
{
	if (pos >= COLONNES * LIGNES)
	{
		scroll();
		pos = COLONNES * LIGNES - COLONNES; // Cursor is set at the first column of the last row
		vid.y = LIGNES - 1;
		vid.x = 0;
	}
	outb(CMD_REGISTER, 0xE);
	outb(DATA_REGISTER, (pos >> 8));
	outb(CMD_REGISTER, 0xF);
	outb(DATA_REGISTER, (pos & 0xff));
}

/**
 * Get the cursor position
 * @return cursor current position between 0 and 2000, [0, 2000[
 */
static uint16_t get_cursor(void)
{
	outb(CMD_REGISTER, 0xE);
	uint16_t pos = 0;
	pos = (inb(DATA_REGISTER) << 8) | pos;
	outb(CMD_REGISTER, 0xF);
	pos = inb(DATA_REGISTER) | pos;
	return pos;
}

/**
 * Get the cursor position on the screen
 * @param x column coordinate
 * @param y row coordinate
 * @return position of the cursor between 0 and 2000, [0, 2000[
 */
static uint16_t get_cursor_from_x_y(uint16_t x, uint16_t y)
{
	uint16_t pos = (y * COLONNES + x);
	return pos;
}

/**
 * Get the Y position of the cursor
 * @return position of the cursor between 0 and ROWS
 */
uint16_t get_y_from_cursor()
{
	return vid.y;
}

/**
 * Get the X position of the cursor
 * @return position of the cursor between 0 and COLONNES
 */
uint16_t get_x_from_cursor()
{
	return vid.x;
}

/**
 * Set the position of the cursor from two coordinates
 * @param x wished column position
 * @param y wished rows position
 */
void set_cursor_from_x_y(uint16_t x, uint16_t y)
{
	vid.x = x;
	vid.y = y;
	if (vid.x >= COLONNES)
	{
		vid.x = 0;
		vid.y += 1;
	}
	uint16_t pos = get_cursor_from_x_y(x, y);
	set_cursor_from_pos(pos);
}

/**
 * Set the font and the background colors
 * @param fc wished color for the font
 */
void set_font_color(uint8_t fc)
{
	vid.font_color = fc;
}

/**
 * Set the font and the background colors
 * @param bc wished color for the background
 */
void set_background_color(uint8_t bc)
{
	vid.background_color = bc;
}

/**
 * Get the font colors
 * @return font's current color
 */
uint8_t get_font_color()
{
	return vid.font_color;
}

/**
 * Get the background colors
 * @return background's current color
 */
uint8_t get_background_color()
{
	return vid.background_color;
}

/**
 * Initialise video buffer at 0xB8000 position with the selected colors
 * @param fc wished color for the font
 * @param bc wished color for the background
 */
static void init_vid_struct(uint8_t fc, uint8_t bc)
{
	vid.vidptr = (uint16_t *)VGA;
	vid.x = 0;
	vid.y = 0;
	set_font_color(fc);
	set_background_color(bc);
}

/**
 * Write a character at the given position
 * @param pos wished position on the screen between 0 and 2000, [0, 2000[
 * @param c charater to write
 */
static void write_to_pos(uint16_t pos, char c)
{
	vid.vidptr[pos] = (vid.background_color << 12) | (vid.font_color << 8) | c;
}

/**
 * Write a character at given coordinates
 * @param x wished column
 * @param y wished row
 * @param c character to write
 */
void write_to_x_y(uint16_t x, uint16_t y, char c)
{
	uint16_t pos = get_cursor_from_x_y(x, y);
	write_to_pos(pos, c);
}

/**
 * Write a character at the current position of the cursor and increment it
 * @param c character to write
 */
void write_to_current_pos(char c)
{
	write_to_x_y(vid.x, vid.y, c);
	vid.x++;
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Write 4 spaces from the current position
 */
void write_tab()
{
	write_to_current_pos(' ');
	write_to_current_pos(' ');
	write_to_current_pos(' ');
	write_to_current_pos(' ');
}

/**
 * Place the cursor at the start of the next row
 */
void write_line_break()
{
	write_to_current_pos('\0');
	vid.x = 0;
	vid.y++;
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Clean the video buffer from a position x and y
 * @param x wished column
 * @param y wished row
 */
void clean_from_x_y(uint16_t x, uint16_t y)
{
	uint16_t pos = get_cursor_from_x_y(x, y);
	while (pos < COLONNES * LIGNES)
	{
		write_to_pos(pos, '\0');
		pos++;
	}
}

/**
 * Clean the video buffer
 */
void clean_vid()
{
	uint16_t pos = 0;
	while (pos < COLONNES * LIGNES)
	{
		write_to_pos(pos, '\0');
		pos++;
	}
}

/**
 * Deletes the character preceding the cursor
 */
void backspace()
{
	vid.x--;
	write_to_x_y(vid.x, vid.y, '\0');
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Move the cursor to the previous line, in case of overflow, perform a scroll
 */
void move_cursor_up()
{
	vid.y--;
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Move the cursor to the next line, in case of overflow, perform a scroll
 */
void move_cursor_down()
{
	vid.y++;
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Move the cursor to the previous columns, in case of overflow, go to the last columns of the previous line
 */
void move_cursor_left()
{
	vid.x--;
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Move the cursor to the next columns, in case of overflow, go to the first columns of the next line
 */
void move_cursor_right()
{
	vid.x++;
	set_cursor_from_x_y(vid.x, vid.y);
}

/**
 * Initialise the screen with font and background colors
 * @param fc wished color for the font
 * @param bc wished color for the background
 */
void init_vid(uint8_t fc, uint8_t bc)
{
	init_vid_struct(fc, bc);
	disable_cursor();
	init_rec_cursor();
	clean_vid();
	set_cursor_from_x_y(0, 1);
	enable_cursor();
}