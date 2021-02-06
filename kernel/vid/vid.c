#include "./vid.h"

static vid_t vid;

void init_rec_cursor(void) {
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x0); // Write to data register start value of cursor at 0
	outb(CMD_REGISTER, 0xB); // Write to command register 0xB
	outb(DATA_REGISTER, 0x1F); // Write to data register end value of cursor at 31
}

void enable_cursor(void) {
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x0);
}

void disable_cursor(void) {
	outb(CMD_REGISTER, 0xA); // Write to command register 0xA
	outb(DATA_REGISTER, 0x10);
}

void set_cursor_from_pos(uint16_t pos) {
	if (pos >= COLONNES * LIGNES) {
		pos = scroll(pos);
	}
	outb(CMD_REGISTER, 0xE);
	outb(DATA_REGISTER, (pos >> 8)); 
	outb(CMD_REGISTER, 0xF); 
	outb(DATA_REGISTER, (pos & 0xff));
}

uint16_t get_cursor(void) {
	outb(CMD_REGISTER, 0xE); 
	uint16_t pos = 0;
	pos = (inb(DATA_REGISTER) << 8) | pos; 
	outb(CMD_REGISTER, 0xF); 
	pos = inb(DATA_REGISTER) | pos;
	return pos;
}

uint16_t get_cursor_from_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = (y * COLONNES + x);
	return pos;
}

uint16_t scroll(uint16_t pos) {
	uint16_t *tmptr = NULL;
	memcpy(tmptr, vid.vidptr, COLONNES * LIGNES * 2);
	clean_vid();
	tmptr += COLONNES;
	memcpy(vid.vidptr, tmptr, COLONNES * LIGNES * 2 - COLONNES * 2);
	return pos - COLONNES;
}

uint16_t get_y_from_cursor() {
	uint16_t pos = get_cursor();
	return pos / COLONNES % LIGNES;
}

uint16_t get_x_from_cursor() {
	uint16_t pos = get_cursor();
	uint16_t y = get_y_from_cursor();
	return (pos - (COLONNES * y)) % COLONNES;
}

void set_cursor_from_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = get_cursor_from_x_y(x, y);
	set_cursor_from_pos(pos);
}

void set_font_background_color(uint8_t fc, uint8_t bc) {
	vid.font_color = fc;
	vid.background_color = bc;
}

void init_vid_struct(uint8_t fc, uint8_t bc) {
	vid.vidptr = (uint16_t*)VGA;
	set_font_background_color(fc, bc);
}

void write_to_pos(uint16_t pos, char c) {
	vid.vidptr[pos] = (vid.background_color << 12) | (vid.font_color << 8) | c;
}

void write_to_x_y(uint16_t x, uint16_t y, char c) {
	uint16_t pos = get_cursor_from_x_y(x, y);
	write_to_pos(pos, c);
}

void clean_vid() {
	uint16_t pos = 0;
	while(pos < COLONNES * LIGNES) {
		write_to_pos(pos, ' ');		
		pos++;
	}
}

void init_vid(uint8_t fc, uint8_t bc) {
	init_vid_struct(fc, bc);
	disable_cursor();
	init_rec_cursor();
	enable_cursor();
	clean_vid();
	set_cursor_from_pos(0);
}