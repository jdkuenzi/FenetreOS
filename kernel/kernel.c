#include "boot/multiboot.h"
#include "lib/stdio.h"

// These are defined in the linker script: kernel.ld
extern void ld_kernel_start();
extern void ld_kernel_end();
uint_t kernel_start = (uint_t)&ld_kernel_start;
uint_t kernel_end = (uint_t)&ld_kernel_end;

/**
 * Program's entry point. 
 * @param info multiboot information.
 * @return program exit code.
 */
void entry(multiboot_info_t* info)
{
	init_vid(0x2, 0x0);

	multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		multiboot_uint32_t size = mods_addr->mod_end - mods_addr->mod_start + 1;
		char buffer[size];
		memcpy(buffer, (void*)mods_addr->mod_start, size);
		my_printf(buffer);
		mods_addr++;
	}

	my_printf("Kernel loaded\n");
	my_printf("	- addr: %x\n", kernel_start);
	my_printf("	- size: %d [KB]\n", (kernel_end - kernel_start)/1000);
	my_printf("	- RAM : %d [KB]\n", info->mem_upper);

	my_printf("%d module(s) loaded\n", info->mods_count);

	mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		my_printf(
			"	- M%d: addr=%x, size=%d [B]\n", 
			i, 
			mods_addr->mod_start,
			mods_addr->mod_end - mods_addr->mod_start
		);
		mods_addr++;
	}
	
	return;
}