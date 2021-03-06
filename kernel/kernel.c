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

#include "x86.h"
#include "mem/gdt.h"
#include "boot/multiboot.h"
#include "interrupt/idt.h"
#include "drivers/timer.h"
#include "drivers/pic.h"
#include "drivers/keyboard.h"
#include "../common/lib/string.h"
#include "../common/lib/stdio.h"

#define TIMER_FREQ_HZ 100

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
	
	init_vid(COLOR_GREEN, COLOR_BLACK);
	my_printf("Init vid OK !\n");
	gdt_init(RAM_in_KB);
	my_printf("Init GDT OK !\n");
	idt_init();
	my_printf("Init IDT OK !\n");
	pic_init();
	my_printf("Init PIC OK !\n");
	timer_init(TIMER_FREQ_HZ);
	my_printf("Init Timer OK !\n");
	my_printf("\t- freq  : %d[Hz]\n", TIMER_FREQ_HZ);
	keyboard_init();
	my_printf("Init Keyboard OK !\n");
	sti(); // enable hardware interruptions
	my_printf("Enable hardware interruptions OK !\n");

	// Print of modules (Logo and image)
	multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		multiboot_uint32_t size = mods_addr->mod_end - mods_addr->mod_start + 1;
		char buffer[size];
		memcpy(buffer, (void*)mods_addr->mod_start, size);
		my_printf(buffer);
		mods_addr++;
	}

	// Print of kernel infos
	my_printf("Kernel loaded\n");
	my_printf("\t- addr  : %x\n", kernel_start);
	my_printf("\t- size  : %d [KB]\n", (kernel_end - kernel_start)/1000);
	my_printf("\t- RAM   : %d [KB]\n", info->mem_upper);
	my_printf("%d module(s) loaded\n", info->mods_count);

	// Print of modules infos
	mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		my_printf(
			"\t- M%d    : addr=%x, size=%d [B]\n", 
			i, 
			mods_addr->mod_start,
			mods_addr->mod_end - mods_addr->mod_start
		);
		mods_addr++;
	}

	while (1)
	{
		uint8_t c = getc();
		if (c) {
			if (c == ASCII_ESC) {
				break;
			} else if (c == ASCII_TAB) {
				my_printf("\t");
			} else if (c == ASCII_SHIFT_IN) {
				/* Nothing to do */	
			} else if (c == ASCII_SPACE) {
				my_printf(" ");
			} else if (c == ASCII_ENTER) {
				my_printf("\n");
			} else if (c == ASCII_BACKSPACE) {
				backspace();
			}else if (c == ASCII_UP_ARROW) {
				move_cursor_up();
			} else if (c == ASCII_LEFT_ARROW) {
				move_cursor_left();
			} else if (c == ASCII_RIGHT_ARROW) {
				move_cursor_right();
			} else if (c == ASCII_DOWN_ARROW) {
				move_cursor_down();
			} else {
				my_printf("%c", c);
			}
		}
	}
	
	
	return;
}