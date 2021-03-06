/**
 * @file vid.h
 * @brief functions signatures to display the cursor and initialise the screen 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#ifndef _VID_H_
#define _VID_H_

#include "../types.h"

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
} vid_t;

extern void enable_cursor(void);
extern void disable_cursor(void);
extern void set_cursor_from_pos(uint16_t pos);
extern void set_cursor_from_x_y(uint16_t x, uint16_t y);
extern uint16_t get_cursor(void);
extern uint16_t get_cursor_from_x_y(uint16_t x, uint16_t y);
extern uint16_t get_y_from_cursor();
extern uint16_t get_x_from_cursor();
extern void move_cursor_up();
extern void move_cursor_down();
extern void move_cursor_left();
extern void move_cursor_right();

extern void backspace();
extern void set_font_color(uint8_t fc);
extern void set_background_color(uint8_t bc);
extern uint8_t get_font_color();
extern uint8_t get_background_color();
extern void init_vid_struct(uint8_t fc, uint8_t bc);
extern void write_to_pos(uint16_t pos, char c);
extern void write_to_x_y(uint16_t x, uint16_t y, char c);
extern void clean_vid();
extern void init_vid(uint8_t fc, uint8_t bc);

#endif