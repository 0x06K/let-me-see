// Multiboot header (8-byte alignment, must be in first 8KB)
#include <stdint.h>
__attribute__((section(".multiboot"))) 
const uint32_t multiboot_header[] = {
    0x1BADB002,       // Magic number
    0x00010003,       // Flags (e.g., align modules, memory info)
    -(0x1BADB002 + 0x00010003) // Checksum (must make sum=0)
};

#include "vga.h"

void kmain(void) {
    vga_initialize();
    vga_writestring_rainbow("Welcome to My Hacker OS!\n");
    vga_typewriter("Initializing systems...\n", 1);
    vga_writehex(0xDEADBEEF);
    vga_putchar('\n');
    vga_writebin(42);
    vga_putchar('\n');
    vga_matrix_effect();
    while(1) { } // idle
}
