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
#include "vid/stdio.h"
#include "task/exec.h"
#include "file_system/file_system.h"
#include "../common/lib/string.h"
#include "../common/lib/utils.h"

#define TIMER_FREQ_HZ 100

// These are defined in the linker script: kernel.ld
extern void ld_kernel_start();
extern void ld_kernel_end();
uint_t kernel_start = (uint_t)&ld_kernel_start;
uint_t kernel_end = (uint_t)&ld_kernel_end;

multiboot_info_t *info;

/**
 * Program's entry point. 
 * @param info multiboot information.
 * @return program exit code.
 */
void entry(multiboot_info_t* mb_info)
{	
	info = mb_info;
	uint_t RAM_in_KB = info->mem_upper * 1024 / 4096;
	
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

	// Print of modules (Logo)
	multiboot_module_t addr;
	stat_t st;
	if (find_file("logo", &addr)) {
		file_stat(&addr, &st);
		char buf[st.size];
		file_read(&addr, buf);
		my_printf(buf);
	}

	// Print of kernel infos
	my_printf("Kernel loaded\n");
	my_printf("\t- addr  : %x\n", kernel_start);
	my_printf("\t- size  : %d [KB]\n", (kernel_end - kernel_start)/1000);
	my_printf("\t- RAM   : %d [KB]\n", info->mem_upper);
	my_printf("%d module(s) loaded\n", info->mods_count);

	// Print of modules infos
	multiboot_module_t *mods_addr = (multiboot_module_t*)info->mods_addr;
	for (multiboot_uint32_t i = 0; i < info->mods_count; i++) {
		file_stat(mods_addr, &st);
		my_printf(
			"\t- M%d    : addr=%x, size=%d [B] cmdline=%s\n", 
			i, 
			mods_addr->mod_start,
			st.size,
			st.filename
		);
		mods_addr++;
	}
	char *argv[2] = {"shell.bin", "Je suis l'argument"};
	exec("shell.bin", argv, 2);

	my_printf("\n+-------------------------+\n|    System shutdown !    |\n+-------------------------+\n");
	halt();
}