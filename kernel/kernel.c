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