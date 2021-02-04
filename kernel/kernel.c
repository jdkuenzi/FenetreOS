#include "../common/types.h"
#include "boot/multiboot.h"
#include "pmio/pmio.h"

#define COLONNES 80
#define LIGNES 25
#define VGA 0xB8000
#define CMD_REGISTER 0x3D4
#define DATA_REGISTER 0x3D5

typedef struct vga
{
	uint16_t *vidptr;
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
	outb(CMD_REGISTER, 0xE); 
	outb(DATA_REGISTER, (uint8_t)(pos >> 8)); 
	outb(CMD_REGISTER, 0xF); 
	outb(DATA_REGISTER, (uint8_t)(pos & 0xff));
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
	uint16_t pos = (y * COLONNES + x) % (COLONNES * LIGNES);
	return pos;
}

void set_x_y_from_cursor(uint16_t *x, uint16_t *y) {
	uint16_t pos = get_cursor();
	*y = pos / COLONNES % LIGNES;
	*x = (pos - (COLONNES * *y)) % COLONNES;
}

void set_cursor_from_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = get_cursor_from_x_y(x, y);
	set_cursor_from_pos(pos);
}

void set_font_background_color(vga_t *vga, uint8_t fc, uint8_t bc) {
	vga->font_color = fc;
	vga->background_color = bc;
}

void init_vga_struct(vga_t *vga, uint8_t fc, uint8_t bc) {
	memset(vga, 0, sizeof(vga_t));
	vga->vidptr = (uint16_t*)VGA;
	set_font_background_color(vga, fc, bc);
}

void write_to_pos(vga_t *vga, uint16_t pos, char c) {
	vga->vidptr[pos] =  (vga->background_color << 12) | (vga->font_color << 8) | c;
}

void write_to_x_y(vga_t *vga, uint16_t x, uint16_t y, char c) {
	uint16_t pos = get_cursor_from_x_y(x, y);
	write_to_pos(vga, pos, c);
}

void clean_vga(vga_t *vga) {
	uint16_t pos = 0;
	while(pos < COLONNES * LIGNES) {
		write_to_pos(vga, pos, ' ');		
		pos++;
	}
}

void init_vga(vga_t *vga, uint8_t fc, uint8_t bc) {
	init_vga_struct(vga, fc, bc);
	disable_cursor();
	init_rec_cursor();
	enable_cursor();
	clean_vga(vga);
	set_cursor_from_pos(0);
}

void printChar(vga_t *vga, char c) {
	uint16_t x;
	uint16_t y;
	set_x_y_from_cursor(&x, &y);

	if (c == '\t') { // Tab
		x += 4;
	} else if (c == '\n') {	// Return
		x = 0;
		y++;
	} else if (c >= ' ') {
		write_to_x_y(vga, x, y, c);
		x++;
	}

	if (x >= COLONNES) {
		y += x / COLONNES;
		x = x % COLONNES;
	}

	set_cursor_from_x_y(x, y);
}

// Permutation de deux valeurs
void swap(char *x, char *y) {
	char tmp = *x;
	*x = *y;
	*y = tmp;
}

// Inversion d'un buffer
void reverseBuffer(char *buffer)
{
	int i = 0;
	int j = strlen(buffer) - 1;
    while (i < j) {
		swap(&buffer[i], &buffer[j]);
		i++;
		j--;
	}
}

void convert(unsigned int num, int base, char *buffer) { 
	static char representation[]= "0123456789ABCDEF";
	int i = 0;
	do 
	{ 
		num /= base;
		buffer[i] = representation[num%base];
		i++;
	} while (num);

	// reverseBuffer(buffer);
	buffer[i] = '\0';
}	
	

void my_printf(vga_t *vga, const char *fmt, ...) {
	int *args = (int*)&fmt;

	while (*fmt) {
		if (*fmt == '%') {
			// int *ptr;
			fmt++;
			args++;
			switch (*fmt) {
				case 'c': {
					/* code */
					break;
				}
				case 's': {
					my_printf(vga, (char*)*args);
					break;
				}
				case 'd': {
					char buffer[50];
					int n = (int)*args;

					if (n < 0) {
						n = -n;
						printChar(vga, '-');
					}

					convert(n, 10, buffer);
					my_printf(vga, buffer);
					
					break;
				}
				case 'x': {
					char buffer[50];
					unsigned int n = (unsigned int)*args;

					convert(n, 16, buffer);
					my_printf(vga, "0x%s", buffer);

					break;
				}
				default: {
					break;
				}
			}
		} else {
			printChar(vga, *fmt);
		}
		fmt++;
	}
}

void entry(multiboot_info_t* info)
{
	const char *str = "Je suis la fenetre :)";
	const char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	vga_t vga;
	init_vga(&vga, 0x2, 0x0);
	my_printf(&vga, str);
	set_cursor_from_x_y(0, 2);
	my_printf(&vga, lorem);
	my_printf(&vga, "\n\n");
	my_printf(&vga, "Test 180		j'ai fait 2 tab !\n");
	my_printf(&vga, "	\nTest 190");
	my_printf(&vga, "\n%s -> %s", "0xFAFA", "64250");
	my_printf(&vga, "\n%d", 10);
	my_printf(&vga, "\n%d", -10);
	my_printf(&vga, "\n%x = %d", 0xFAFA, 0xFAFA);
	my_printf(&vga, "\n%x", 64250);
	my_printf(&vga, "\n%d", 0xFAFA);
	return;
}