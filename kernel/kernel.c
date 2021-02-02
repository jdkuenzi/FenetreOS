#include "../common/types.h"
#include "boot/multiboot.h"
#include "pmio/pmio.h"

#define COLONNES 80
#define LIGNES 25
#define VGA 0xB8000

int cursor_x = 10;
int cursor_y = 10;

uint16_t *screen;

// These are defined in the linker script: kernel.ld
extern void ld_kernel_start();
extern void ld_kernel_end();
uint_t kernel_start = (uint_t)&ld_kernel_start;
uint_t kernel_end = (uint_t)&ld_kernel_end;

void clean_vga(char *vidptr) {
	int i = 0;
	while(i < 80 * 25 * 2) {
		vidptr[i] = ' ';
		vidptr[i+1] = 0x07; 		
		i += 2;
	}
}

void write_string(char *vidptr, const char *str) {
	int j = 0;
	while(*str) {
		vidptr[j] = *str;
		vidptr[j+1] = 0x07;
		str++;
		j += 2;
	}
}

void printChar(uint8_t c) {

	uint16_t *addr;

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
		char *vidptr = (char*)VGA;
		clean_vga(vidptr);
		write_string(vidptr, "JE SUIS");
		addr = screen + (cursor_y * COLONNES + cursor_x);
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

void my_printf(char * fmt, ...) {
	char **args = (char**) &fmt;
	int c;
	char buffer[50];

	while ((c = *fmt++) != 0)
	{
		if (c != '%')
		{
			printChar(c);
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
					printChar(*p);
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
					printChar(*newBuff);
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


void entry(void)
{
	screen = (unsigned short *)VGA;
	const char *str = "Je suis la fenetre :)";
	char *vidptr = (char*)VGA;
	clean_vga(vidptr);
	write_string(vidptr, str);
	printChar('P');
	return;
}

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