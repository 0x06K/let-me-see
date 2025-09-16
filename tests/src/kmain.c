#include "vga.h"

void kmain(void) {
    // Initialize VGA
    vga_initialize();
    vga_clear();
    
    // Test basic text output
    vga_setcolor(RED_ON_BLACK);
    vga_writestring("KERNEL BOOT SUCCESSFUL\n");
    
    // Test positioned output
    vga_print_at("STATUS:", 2, 2, YELLOW_ON_BLACK);
    vga_print_at("[OK]", 70, 2, GREEN_ON_BLACK);
    
    // Test number output
    vga_setcolor(WHITE_ON_BLACK);
    vga_writestring("Memory: ");
    vga_print_int(4096);
    vga_writestring(" MB\n");
    
    vga_writestring("Kernel loaded at: ");
    vga_print_hex(0x10000);
    vga_writestring("\n\n");
    
    // Test fill area (progress bar)
    vga_print_at("Loading:", 5, 5, WHITE_ON_BLACK);
    vga_fill_area(15, 5, 20, 1, '#', GREEN_ON_BLACK);
    vga_print_at("[100%]", 37, 5, WHITE_ON_BLACK);
    
    // Test cursor positioning
    size_t x, y;
    vga_get_cursor(&x, &y);
    vga_writestring("Cursor at: ");
    vga_print_int(x);
    vga_writestring(", ");
    vga_print_int(y);
    vga_writestring("\n");
    
    // Test line clearing and updating
    vga_writestring("Initializing systems...\n");
    for (int i = 0; i < 1000000; i++); // delay
    
    vga_get_cursor(&x, &y);
    vga_clear_line(y-1);
    vga_set_cursor(0, y-1);
    vga_setcolor(GREEN_ON_BLACK);
    vga_writestring("All systems ready!\n");
    
    // Test special characters
    vga_setcolor(WHITE_ON_BLACK);
    vga_writestring("Tab test:\tAligned text\n");
    vga_writestring("Backspace test: ABC\b\bXY\n");
    
    // Create a simple box
    vga_fill_area(10, 15, 30, 5, ' ', WHITE_ON_BLUE);
    vga_print_at("SYSTEM INFORMATION", 15, 16, WHITE_ON_BLUE);
    vga_print_at("CPU: x86", 12, 17, WHITE_ON_BLUE);
    vga_print_at("RAM: 8GB", 12, 18, WHITE_ON_BLUE);
    
    // Bottom status line
    vga_print_at("Press any key to continue...", 25, 24, YELLOW_ON_BLACK);
    
    // Halt system
    while(1) {
        __asm__ volatile("hlt");
    }
}