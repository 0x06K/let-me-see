#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

// VGA text mode dimensions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// VGA color codes
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
};

// Initialize VGA text mode
void vga_initialize(void);

// Set text color (foreground + background)
void vga_setcolor(uint8_t color);

// Put a single character at (x, y)
void vga_putentryat(char c, uint8_t color, size_t x, size_t y);

// Print a single character at current cursor
void vga_putchar(char c);

// Print a string starting at current cursor
void vga_writestring(const char* str);

// Clear the screen
void vga_clear(void);

#endif // VGA_H
