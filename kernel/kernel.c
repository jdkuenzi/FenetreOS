#include "../common/types.h"
#include "boot/multiboot.h"
#include "pmio/pmio.h"

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

void write_string(char *vidptr, char *str) {
	int j = 0;
	while(*str) {
		vidptr[j] = *str;
		vidptr[j+1] = 0x07;
		str++;
		j += 2;
	}
}

void my_printf() {
	// TO DO
}

void entry(void)
{
	const char *str = "Je suis la fenetre :)";
	char *vidptr = (char*)0xb8000;
	unsigned int i = 0;
	unsigned int j = 0;
	clean_vga(vidptr);
	write_string(vidptr, str);
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