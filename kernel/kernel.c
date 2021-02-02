#include "../common/types.h"
#include "boot/multiboot.h"
#include "pmio/pmio.h"

#define CMD_REGISTER 0x3D4
#define DATA_REGISTER 0x3D5

typedef struct vga
{
	uint8_t font_color;
	uint8_t background_color;
} vga_t;

// These are defined in the linker script: kernel.ld
extern void ld_kernel_start();
extern void ld_kernel_end();
uint_t kernel_start = (uint_t)&ld_kernel_start;
uint_t kernel_end = (uint_t)&ld_kernel_end;

// Ref : Ce code source vient de la librairie string.c (https://code.woboq.org/linux/linux/lib/string.c.html)
// +------------------------------------------------------------+
void *memset(void *dst, int value, uint_t count) {
	char *d = dst;
	while (count--)
	{
		*d++ = value;
	}
	return dst;
}

void *memcpy(void *dst, void *src, uint_t count) {
	char *copySrc = src;
	char *copyDst = dst;
	while (count--)
	{
		*copyDst++ = *copySrc++;
	}
	return dst;
}

char *strncpy(char *dest, const char *src, uint_t count) {
	char *tmp = dest;
	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

int strcmp(const char *p, const char *q) {
	unsigned char c1, c2;
	while (1) {
		c1 = *p++;
		c2 = *q++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
	
}

int strncmp(const char *p, const char *q, uint_t count) {
	unsigned char c1, c2;
	while (count) {
		c1 = *p++;
		c2 = *q++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

int strlen(const char *s) {
	const char *sc;
	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
// +------------------------------------------------------------+

void my_printf() {
	// TO DO
}

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

void set_cursor(uint16_t pos) {
	outb(CMD_REGISTER, 0xE); 
	outb(DATA_REGISTER, pos >> 8); 
	outb(CMD_REGISTER, 0xF); 
	outb(DATA_REGISTER, pos & 0xff);
}

uint16_t get_cursor(void) {
	outb(CMD_REGISTER, 0xE); 
	uint16_t pos = (inb(DATA_REGISTER) << 8) | pos; 
	outb(CMD_REGISTER, 0xF); 
	pos = inb(DATA_REGISTER) | pos;
	return pos;
}

void set_cursor_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = y * 80 + x % (80 * 25);
	set_cursor(pos);
}

void set_font_background_color(vga_t *vga, uint8_t fc, uint8_t bc) {
	vga->font_color = fc;
	vga->background_color = bc;
}

vga_t *init_vga_struct(uint8_t fc, uint8_t bc) {
	vga_t *vga = (vga_t*)memset(vga, 0, sizeof(vga_t));
	set_font_background_color(vga, fc, bc);
	return vga;
}

vga_t *init_vga(uint8_t fc, uint8_t bc, char *vidptr) {
	vga_t *vga = init_vga_struct(fc, bc);
	disable_cursor();
	init_rec_cursor();
	enable_cursor();
	clean_vga(vga, vidptr);
	set_cursor(0);
	return vga;
}

void clean_vga(vga_t *vga, char *vidptr) {
	int i = 0;
	while(i < 80 * 25 * 2) {
		vidptr[i] = ' ';
		vidptr[i+1] = (vga->background_color << 4) | vga->font_color; 		
		i += 2;
	}
}

void write_string(vga_t *vga, char *vidptr, const char *str) {
	uint16_t pos = get_cursor();
	while(*str) {
		vidptr[pos] = *str;
		vidptr[pos+1] = (vga->background_color << 4) | vga->font_color;
		str++;
		pos += 2;
	}
	set_cursor(pos);
}

void entry(multiboot_info_t* info)
{
	const char *str = "Je suis la fenetre :)";
	const char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	char *vidptr = (char*)0xb8000;
	vga_t *vga = init_vga(0x2, 0x0, vidptr);
	write_string(vga, vidptr, str);
	set_cursor_x_y(0, 13);
	write_string(vga, vidptr, lorem);
	return;
}