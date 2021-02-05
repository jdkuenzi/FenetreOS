
#ifndef _VID_H_
#define _VID_H_

#include "../pmio/pmio.h"
#include "../lib/string.h"

#define CMD_REGISTER 0x3D4
#define DATA_REGISTER 0x3D5
#define COLONNES 80
#define LIGNES 25
#define VGA 0xB8000

typedef struct vid
{
	uint16_t *vidptr;
	uint8_t font_color;
	uint8_t background_color;
} vid_t;

extern vid_t vid;

void init_rec_cursor(void);
void enable_cursor(void);
void disable_cursor(void);
void set_cursor_from_pos(uint16_t pos);
void set_cursor_from_x_y(uint16_t x, uint16_t y);
uint16_t get_cursor(void);
uint16_t get_cursor_from_x_y(uint16_t x, uint16_t y);
uint16_t get_y_from_cursor();
uint16_t get_x_from_cursor();

void set_font_background_color(uint8_t fc, uint8_t bc);
void init_vid_struct(uint8_t fc, uint8_t bc);
void write_to_pos(uint16_t pos, char c);
void write_to_x_y(uint16_t x, uint16_t y, char c);
void clean_vid();
void init_vid(uint8_t fc, uint8_t bc);

#endif