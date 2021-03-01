/**
 * @file kernel.c
 * @brief Initial kernel in C
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date February 7, 2021
 * @version 0.1
 */

#include "boot/multiboot.h"
#include "lib/stdio.h"
#include "mem/gdt.h"
#include "interrupt/idt.h"
#include "drivers/timer.h"
#include "drivers/pic.h"

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
	uint_t RAM_in_KB = info->mem_upper * 1000 / 4096;
	gdt_init(RAM_in_KB);
	idt_init();
	pic_init();
	init_vid(COLOR_GREEN, COLOR_BLACK);
	timer_init(50);
	my_printf("ticks=%d\n", get_ticks());


	// Print of modules (Logo and image)
	multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;
	// for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
	// 	multiboot_uint32_t size = mods_addr->mod_end - mods_addr->mod_start + 1;
	// 	char buffer[size];
	// 	memcpy(buffer, (void*)mods_addr->mod_start, size);
	// 	my_printf(buffer);
	// 	mods_addr++;
	// }

	// Print of kernel infos
	my_printf("Kernel loaded\n");
	my_printf("	- addr  : %x\n", kernel_start);
	my_printf("	- size  : %d [KB]\n", (kernel_end - kernel_start)/1000);
	my_printf("	- RAM   : %d [KB]\n", info->mem_upper);
	my_printf("%d module(s) loaded\n", info->mods_count);

	// Print of modules infos
	mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		my_printf(
			"	- M%d    : addr=%x, size=%d [B]\n", 
			i, 
			mods_addr->mod_start,
			mods_addr->mod_end - mods_addr->mod_start
		);
		mods_addr++;
	}

	gdt_entry_t* gdt = get_gdt();
	gdt_ptr_t gdt_ptr = get_gdt_ptr();

	my_printf("GDT\n");
	my_printf("	- addr  : %x\n", gdt);
	my_printf("GDT_ptr\n");
	my_printf("	- base  : %x\n", gdt_ptr.base);
	my_printf("	- limit : %d * 4096 [B]\n", gdt_ptr.limit);
	my_printf("ticks=%d\n", get_ticks());

	// uint_t count = 10;
	// while (count--) {
	// 	my_printf("J'attends\n");
	// 	sleep(100);
	// }
	
	return;
}