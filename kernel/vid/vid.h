/**
 * @file vid.h
 * @brief functions signatures to display the cursor and initialise the screen 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.2
 */

#ifndef _VID_H_
#define _VID_H_

#include "../../common/types.h"

#define CMD_REGISTER 0x3D4
#define DATA_REGISTER 0x3D5
#define COLONNES 80
#define LIGNES 25
#define VGA 0xB8000

#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xa
#define COLOR_LIGHT_CYAN 0xb
#define COLOR_LIGHT_RED 0xc
#define COLOR_LIGHT_MAGENTA 0xd
#define COLOR_YELLOW 0xe
#define COLOR_WHITE 0xf

typedef struct vid
{
	uint16_t *vidptr;
	uint8_t font_color;
	uint8_t background_color;
	uint16_t x;
	uint16_t y;
} vid_t;

void enable_cursor(void);
void disable_cursor(void);
void set_cursor_from_x_y(uint16_t x, uint16_t y);
uint16_t get_y_from_cursor();
uint16_t get_x_from_cursor();
void move_cursor_up();
void move_cursor_down();
void move_cursor_left();
void move_cursor_right();

void backspace();
void set_font_color(uint8_t fc);
void set_background_color(uint8_t bc);
uint8_t get_font_color();
uint8_t get_background_color();
void write_to_current_pos(char c);
void write_tab();
void write_line_break();

void write_to_x_y(uint16_t x, uint16_t y, char c);
void clean_vid();
void init_vid(uint8_t fc, uint8_t bc);

#endif