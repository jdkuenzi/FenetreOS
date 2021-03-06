/**
 * @file vid.c
 * @brief functions to display the cursor and initialise the screen 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#include "./vid.h"
#include "../pmio/pmio.h"
#include "../lib/string.h"

static vid_t vid;

/**
 * Initialise the cursor with rectangle shape
 */
void init_rec_cursor(void) {
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x0); // Write to data register start value of cursor at 0
	outb(CMD_REGISTER, 0xB); // Write to command register 0xB
	outb(DATA_REGISTER, 0x1F); // Write to data register end value of cursor at 31
}

/**
 * Enable the cursor on the screen
 */
void enable_cursor(void) {
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x0);
}

/**
 * Disable the cursor on the screen
 */
void disable_cursor(void) {
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x10);
}

/**
 * Scroll the screen
 */
void scroll() {
	uint_t count = COLONNES * LIGNES * 2 - COLONNES * 2;
	uint16_t buffer[COLONNES * LIGNES - COLONNES];
	memcpy(buffer, vid.vidptr + COLONNES, count);
	clean_vid();
	memcpy(vid.vidptr, buffer, count);
}

/**
 * Set the position of the cursor from a value and scroll if needed
 * @param pos wished position
 */
void set_cursor_from_pos(uint16_t pos) {
	if (pos >= COLONNES * LIGNES) {
		scroll();
		pos = COLONNES * LIGNES - COLONNES; // Cursor is set at the first column of the last row
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
uint16_t get_cursor(void) {
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
uint16_t get_cursor_from_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = (y * COLONNES + x);
	return pos;
}

/**
 * Get the Y position of the cursor
 * @return position of the cursor between 0 and 2000, [0, 2000[
 */
uint16_t get_y_from_cursor() {
	uint16_t pos = get_cursor();
	return pos / COLONNES % LIGNES;
}

/**
 * Get the X position of the cursor
 * @return position of the cursor between 0 and 2000, [0, 2000[
 */
uint16_t get_x_from_cursor() {
	uint16_t pos = get_cursor();
	uint16_t y = get_y_from_cursor();
	return (pos - (COLONNES * y)) % COLONNES;
}

/**
 * Set the position of the cursor from two coordinates
 * @param x wished column position
 * @param y wished rows position
 */
void set_cursor_from_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = get_cursor_from_x_y(x, y);
	set_cursor_from_pos(pos);
}

/**
 * Set the font and the background colors
 * @param fc wished color for the font
 */
void set_font_color(uint8_t fc) {
	vid.font_color = fc;
}

/**
 * Set the font and the background colors
 * @param bc wished color for the background
 */
void set_background_color(uint8_t bc) {
	vid.background_color = bc;
}

uint8_t get_font_color() {
	return vid.font_color;
}

uint8_t get_background_color() {
	return vid.background_color;
}

/**
 * Initialise video buffer at 0xB8000 position with the selected colors
 * @param fc wished color for the font
 * @param bc wished color for the background
 */
void init_vid_struct(uint8_t fc, uint8_t bc) {
	vid.vidptr = (uint16_t*)VGA;
	set_font_color(fc);
	set_background_color(bc);
}

/**
 * Write a character at the given position
 * @param pos wished position on the screen between 0 and 2000, [0, 2000[
 * @param c charater to write
 */
void write_to_pos(uint16_t pos, char c) {
	vid.vidptr[pos] = (vid.background_color << 12) | (vid.font_color << 8) | c;
}

/**
 * Write a character at given coordinates
 * @param x wished column
 * @param y wished row
 * @param c character to write
 */
void write_to_x_y(uint16_t x, uint16_t y, char c) {
	uint16_t pos = get_cursor_from_x_y(x, y);
	write_to_pos(pos, c);
}

/**
 * Clean the video buffer
 */
void clean_vid() {
	uint16_t pos = 0;
	while(pos < COLONNES * LIGNES) {
		write_to_pos(pos, ' ');		
		pos++;
	}
}

void backspace() {
	uint16_t pos = get_cursor() - 1;
	write_to_pos(pos, ' ');
	set_cursor_from_pos(pos);
}

void move_cursor_up() {
	uint16_t y = get_y_from_cursor();
	uint16_t x = get_x_from_cursor();	
	y--;
	set_cursor_from_x_y(x, y);
}

void move_cursor_down() {
	uint16_t y = get_y_from_cursor();
	y++;
	uint16_t x = get_x_from_cursor();	
	set_cursor_from_x_y(x, y);
}

void move_cursor_left() {
	uint16_t x = get_x_from_cursor();	
	x--;
	uint16_t y = get_y_from_cursor();
	set_cursor_from_x_y(x, y);
}

void move_cursor_right() {
	uint16_t x = get_x_from_cursor();	
	uint16_t y = get_y_from_cursor();
	x++;
	set_cursor_from_x_y(x, y);
}

/**
 * Initialise the screen with font and background colors
 * @param fc wished color for the font
 * @param bc wished color for the background
 */
void init_vid(uint8_t fc, uint8_t bc) {
	init_vid_struct(fc, bc);
	disable_cursor();
	init_rec_cursor();
	enable_cursor();
	clean_vid();
	set_cursor_from_pos(0);
}