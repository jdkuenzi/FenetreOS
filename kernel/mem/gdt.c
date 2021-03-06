#include "gdt.h"
#include "../descriptors.h"
#include "../../common/lib/string.h"

// Descriptor system bit (S)
// For code or data segments
#define S_CODE_OR_DATA   1
// For TSS segment, LDT, call gate, interrupt gate, trap gate, task gate
#define S_SYSTEM   0

// D/B bit
#define DB_SEG  1
#define DB_SYS  0

// Declare the GDT table
static gdt_entry_t gdt[3];
static gdt_ptr_t gdt_ptr;

// Build and return a GDT entry.
// base is the base of the segment
// limit is the limit of the segment (NOTE: it's a 20-bit value)
// type is the type of segment
// s indicates whether it's a system segment
// db indicates whether it's a code/data segment or TSS, LDT or gate
// granularity indicates 1 byte or 4KB granularity
// dpl is the privilege level
static gdt_entry_t build_entry(uint32_t base, uint32_t limit, uint8_t type, uint8_t s, uint8_t db, uint8_t granularity, uint8_t dpl) {
	gdt_entry_t entry;
	// For a TSS and LDT, base is the addresse of the TSS/LDT structure
	// and limit is the size of the structure.
	entry.lim15_0 = limit & 0xffff;
	entry.base15_0 = base & 0xffff;
	entry.base23_16 = (base >> 16) & 0xff;
	entry.type = type;  // See TYPE_xxx flags
	entry.s = s;        // 1 for segments; 0 for system (TSS, LDT, gates)
	entry.dpl = dpl;    // privilege level
	entry.present = 1;  // present in memory
	entry.lim19_16 = (limit >> 16) & 0xf;
	entry.avl = 0;      // available for use
	entry.l = 0;        // should be 0 (64-bit code segment)
	entry.db = db;      // 1 for 32-bit code and data segments; 0 for system (TSS, LDT, gate)
	entry.granularity = granularity;  // granularity of the limit value: 0 = 1 byte; 1 = 4096 bytes
	entry.base31_24 = (base >> 24) & 0xff;
	return entry;
}

// Return a NULL entry.
gdt_entry_t gdt_make_null_segment() {
	gdt_entry_t entry;
	memset(&entry, 0, sizeof(gdt_entry_t));
	return entry;
}

// Return a code segment specified by the base, limit and privilege level passed in arguments.
gdt_entry_t gdt_make_code_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_CODE_EXECONLY, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return a read-write data segment specified by the base, limit and privilege level passed in arguments.
gdt_entry_t gdt_make_data_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_DATA_RW, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return the selector of an entry in the GDT
uint_t gdt_entry_to_selector(gdt_entry_t *entry) {
	return GDT_INDEX_TO_SELECTOR(entry - gdt);
}

// Initialize the GDT
void gdt_init(uint_t RAM_in_KB) {
	// Initialize 3 segment descriptors: NULL, code segment, data segment.
	// Code and data segments must have kernel privilege (see "Privilege levels" in descriptors.h)
	// Segments limit must be the available RAM.
	gdt[0] = gdt_make_null_segment();
	gdt[1] = gdt_make_code_segment(0, RAM_in_KB, DPL_KERNEL);
	gdt[2] = gdt_make_data_segment(0, RAM_in_KB, DPL_KERNEL);


	// Setup gdt_ptr so it points to the GDT and ensure it has the right limit.
	gdt_ptr.base = (uint32_t)gdt;
	gdt_ptr.limit = sizeof(gdt)-1;

	// Load the GDT using the gdt_load assembly function
	gdt_load(&gdt_ptr);
}
