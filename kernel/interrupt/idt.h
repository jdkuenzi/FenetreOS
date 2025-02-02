/**
 * @file idt.h
 * @brief functions signatures to handle interruptions and exceptions 
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#ifndef _IDT_H_
#define _IDT_H_

#include "../../common/types.h"

#define INTERRUPT_COUNT     256

#define FIRST_EXCEPTION     0
#define LAST_EXCEPTION      20
#define EXCEPTION_COUNT     (LAST_EXCEPTION-FIRST_EXCEPTION+1)

#define FIRST_IRQ           0
#define LAST_IRQ            15
#define IRQ_COUNT           (LAST_IRQ-FIRST_IRQ+1)

// Structure of an IDT descriptor. There are 3 types of descriptors:
// a task-gate, an interrupt-gate, and a trap-gate.
// See 5.11 of Intel 64 & IA32 architectures software developer's manual for more details.
// For task gates, offset must be 0.
typedef struct idt_entry_st {
	uint16_t offset15_0;   // only used by trap and interrupt gates
	uint16_t selector;     // segment selector for trap and interrupt gates; TSS segment selector for task gates
	uint16_t reserved : 8;
	uint16_t type : 5;
	uint16_t dpl : 2;
	uint16_t p : 1;

	uint16_t offset31_16;  // only used by trap and interrupt gates
} __attribute__((packed)) idt_entry_t;

// Structure describing a pointer to the IDT gate table.
// This format is required by the lidt instruction.
typedef struct idt_ptr_st {
	uint16_t limit;   // Limit of the table (ie. its size)
	uint32_t base;    // Address of the first entry
} __attribute__((packed)) idt_ptr_t;

extern void idt_init();
extern void idt_load(idt_ptr_t *idt_ptr);

#endif
