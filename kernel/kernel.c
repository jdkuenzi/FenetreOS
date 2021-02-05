#include "boot/multiboot.h"
#include "lib/stdio.h"
#include "lib/string.h"
vid_t vid; // Extern in vid/vid.h

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

void entry(multiboot_info_t* info)
{
	// uint16_t *vga = (uint16_t*)0xb8000;
	// const char *str = "Je suis la fenetre :)";
	// while (*str) {
	// 	*vga++ = (0x0 << 12) | (0x2 << 8) | *str;
	// 	str++;
	// }
	// const char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

	init_vid(0x2, 0x0);

	my_printf("Kernel loaded\n");
	my_printf("	- ADDR: %x\n", kernel_start);
	my_printf("	- SIZE: %d [KB]\n", (kernel_end - kernel_start)/1000);
	my_printf("	- RAM : %d [KB]\n", info->mem_upper);

	my_printf("%d module(s) loaded\n", info->mods_count);

	multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;

	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		my_printf(
			"	- Module %d: addr=%x, size=%d [B]\n", 
			i, 
			mods_addr->mod_start,
			mods_addr->mod_end - mods_addr->mod_start
		);
		mods_addr++;
	}

	mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		multiboot_uint32_t size = mods_addr->mod_end - mods_addr->mod_start;
		char buffer[size];
		memcpy(buffer, (void*)mods_addr->mod_start, size);
		my_printf(buffer);
		mods_addr++;
	}

	// while (true)
	// {
	// 	/* code */
	// }

	// my_printf(&vid, str);
	// set_cursor_from_x_y(0, 2);
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