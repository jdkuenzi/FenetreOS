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
	char *vidptr;
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

void set_cursor(uint16_t pos) {
	outb(CMD_REGISTER, 0xE); 
	outb(DATA_REGISTER, pos >> 8); 
	outb(CMD_REGISTER, 0xF); 
	outb(DATA_REGISTER, pos & 0xff);
}

uint16_t get_cursor(void) {
	outb(CMD_REGISTER, 0xE); 
	uint16_t pos = 0;
	pos = (inb(DATA_REGISTER) << 8) | pos; 
	outb(CMD_REGISTER, 0xF); 
	pos = inb(DATA_REGISTER) | pos;
	return pos;
}

void set_cursor_x_y(uint16_t x, uint16_t y) {
	uint16_t pos = y * 80 + x % (80 * 25);
	set_cursor(pos);
}

void clean_vga(vga_t *vga) {
	int i = 0;
	while(i < COLONNES * LIGNES * 2) {
		vga->vidptr[i] = ' ';
		vga->vidptr[i+1] = (vga->background_color << 4) | vga->font_color; 		
		i += 2;
	}
}

void set_font_background_color(vga_t *vga, uint8_t fc, uint8_t bc) {
	vga->font_color = fc;
	vga->background_color = bc;
}

void init_vga_struct(vga_t *vga, char *vidptr, uint8_t fc, uint8_t bc) {
	memset(vga, 0, sizeof(vga_t));
	vga->vidptr = vidptr;
	set_font_background_color(vga, fc, bc);
}

void init_vga(vga_t *vga, char *vidptr, uint8_t fc, uint8_t bc) {
	init_vga_struct(vga, vidptr, fc, bc);
	disable_cursor();
	init_rec_cursor();
	enable_cursor();
	clean_vga(vga);
	set_cursor(0);
}

void write_string(vga_t *vga, const char *str) {
	uint16_t pos = get_cursor();
	while(*str) {
		vga->vidptr[pos] = *str;
		vga->vidptr[pos+1] = (vga->background_color << 4) | vga->font_color;
		str++;
		pos += 2;
	}
	set_cursor(pos);
}

void printChar(vga_t *vga, uint8_t c) {

	int cursor_x = 10;
	int cursor_y = 10;
	char *addr;

	if (c == '\t')		// Tab
	{
		cursor_x += 4;
	} else if (c == '\n')	// Return
	{
		cursor_x = 0;
		cursor_y++;
	} else if (c == 0x80)	// Backspace
	{
		cursor_x--;
	} else if (c >= ' ')
	{
		// clean_vga(vga);
		write_string(vga, "JE SUIS");
		addr = vga->vidptr + (cursor_y * COLONNES + cursor_x);
		//*addr = (0xF0<<8) | c;
		cursor_x++;
	}
	
	if (cursor_x >= COLONNES)	// Fin de ligne
	{
		cursor_x = 0;
		cursor_y++;
	}
}

// Permutation de deux valeurs
void swap(char *x, char *y) {
	char tmp = *x;
	*x = *y;
	*y = tmp;
}

// Inversion d'un buffer
char* reverseBuffer(char *buffer, int i, int j)
{
    while (i < j) {
		i++;
		j--;
        swap(&buffer[i], &buffer[j]);
	}
    return buffer;
}

void my_printf(vga_t *vga, char * fmt, ...) {
	char **args = (char**) &fmt;
	int c;
	char buffer[50];

	while ((c = *fmt++) != 0)
	{
		if (c != '%')
		{
			printChar(vga, c);
		} else 
		{
			char *p;
			c = *fmt++;
			switch (c)
			{
			case 'c':
				/* code */
				break;
			case 's': {
				p = *args++;
				while (*p != NULL)
				{
					printChar(vga, *p);
					*p++;
				}
				
				break;
			}
			case 'd':
				/* code */
				break;
			case 'x': {
				int n = *((int *) args++);
				int i = 0;
				while (n)
				{
					int res = n % 16;		// Base 16 -> HEX
					if (res >= 10) 
						buffer[i++] = 'a' + (res - 10);
					else
						buffer[i++] = '0' + res;
					n = n / 16;
				}
				if (i == 0)
        			buffer[i++] = '0';
				buffer[i] = '\0';
				char *newBuff = reverseBuffer(buffer, 0, i - 1);
				while (*newBuff != NULL)
				{
					printChar(vga, *newBuff);
					*newBuff++;
				}
				break;
			}
			
			default:
				break;
			}
		}
		
	}
	
}

void entry(multiboot_info_t* info)
{
	const char *str = "Je suis la fenetre :)";
	const char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	vga_t vga;
	init_vga(&vga, (char*)VGA, 0x0, 0x2);
	write_string(&vga, str);
	set_cursor_x_y(0, 13);
	write_string(&vga, lorem);
	return;
}