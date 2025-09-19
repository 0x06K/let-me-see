#include "gdt.h"
#include <stdint.h>

// Assembly function to flush/load the GDT
extern void gdt_flush(uint32_t gdt_ptr_address);

// Our GDT will hold 3 entries: null, code, data
static gdt_entry_t gdt_entries[3];
static gdt_ptr_t   gdt_ptr;

// Helper to set a single GDT entry
static void gdt_set_entry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[idx].limit_low    =  limit        & 0xFFFF;
    gdt_entries[idx].base_low     =  base         & 0xFFFF;
    gdt_entries[idx].base_middle  = (base  >> 16) & 0xFF;
    gdt_entries[idx].access       =  access;
    gdt_entries[idx].granularity  = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt_entries[idx].base_high    = (base  >> 24) & 0xFF;
}

// === FIRST FUNCTION ===
// Initialize the GDT and load it with lgdt
void gdt_init(void)
{
    // Set up the pointer for lgdt
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // Null descriptor (required)
    gdt_set_entry(0, 0, 0, 0, 0);

    // Kernel code/data
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA0);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xA0);

    // User code/data (Ring 3)
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xA0);
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xA0);


    // Load the new GDT (assembly routine you’ll implement in gdt.asm)
    gdt_flush((uint32_t)&gdt_ptr);
}
