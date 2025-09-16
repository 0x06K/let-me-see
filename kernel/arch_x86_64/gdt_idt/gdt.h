#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// Structure for a single GDT entry
typedef struct __attribute__((packed)) gdt_entry {
    uint16_t limit_low;      // Lower 16 bits of the segment limit
    uint16_t base_low;       // Lower 16 bits of the base
    uint8_t  base_middle;    // Next 8 bits of the base
    uint8_t  access;         // Access flags
    uint8_t  granularity;    // Granularity flags + upper 4 bits of limit
    uint8_t  base_high;      // Last 8 bits of the base
} gdt_entry_t;

// Pointer structure for LGDT instruction
struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint64_t base;
}gdt_ptr_t;

// Initialize the GDT
void gdt_init(void);

// Set an individual GDT entry
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

// Assembly function to flush/load the GDT
extern void gdt_flush(uint32_t gdt_ptr_address);

#endif
