#include "boot/multiboot.h"
#include "lib/stdio.h"
vid_t vid; // Extern in lib/stdio.h

// These are defined in the linker script: kernel.ld
extern void ld_kernel_start();
extern void ld_kernel_end();
uint_t kernel_start = (uint_t)&ld_kernel_start;
uint_t kernel_end = (uint_t)&ld_kernel_end;

// void scroll(vid_t *vid) {
// 	int dist = get_x_cursor() - LIGNES + 1;
// 	if (dist > 0)
// 	{
// 		uint8_t *newStart = ((uint8_t *) *vid->vidptr) + dist * COLONNES * 2;
// 		int bytesToCopy = (LIGNES - dist) * COLONNES * 2;
// 		uint8_t *newBlankStart = vid->vidptr + (LIGNES - dist) * COLONNES;
// 		int byteToBlank = dist + COLONNES * 2;
// 		// memcpy((uint8_t*) *vid->vidptr, newStart, bytesToCopy);
// 		// memset((uint8_t*) newBlankStart, (vid->background_color << 12) | 0x20, byteToBlank);
// 	}
	
// }

void entry(multiboot_info_t* info)
{
	// uint16_t *vga = (uint16_t*)0xb8000;
	// const char *str = "Je suis la fenetre :)";
	// while (*str) {
	// 	*vga++ = (0x0 << 12) | (0x2 << 8) | *str;
	// 	str++;
	// }
	// const char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	init_vid(&vid, 0x2, 0x0);
	my_printf(
		"Kernel loaded\n	- ADDR: %x\n	- SIZE: %d [KB]\n	- RAM : %d [KB]\n", 
		kernel_start, 
		(kernel_end - kernel_start)/1000,
		info->mem_upper
	);
	my_printf("%d module(s) loaded\n", info->mods_count);

	multiboot_uint32_t *mods_addr = &info->mods_addr;
	
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		my_printf(
			"	- Module %d: addr=%x, size=%d [B]\n", 
			i, 
			mods_addr,
			0
		);
		mods_addr++;
	}

	// while (true)
	// {
	// 	/* code */
	// }
	

	// my_printf(&vid, str);
	// set_cursor_from_x_y(0, 2);
	// my_printf(&vid, lorem);
	// my_printf(&vid, "\n\n");
	// my_printf(&vid, "Test 180		j'ai fait 2 tab !\n");
	// my_printf(&vid, "	\nTest 190");
	// my_printf(&vid, "\n%s -> %s", "0xFAFA", "64250");
	// my_printf(&vid, "\n%d", 10);
	// my_printf(&vid, "\n%d", -10);
	// my_printf(&vid, "\n%x = %d", 0xBA, 0xBA);
	// my_printf(&vid, "\n%x", 64250);
	// my_printf(&vid, "\n%d", 0xFAFA);
	// my_printf(&vid, "\n%d = %c", 65, 65);
	// my_printf(&vid, "\n%c", 66);
	// my_printf(&vid, "\n%c", 75);
	return;
}