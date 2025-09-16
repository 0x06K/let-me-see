#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

// VGA dimensions
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

// VGA colors
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
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Common color combinations
#define GREEN_ON_BLACK  ((VGA_COLOR_BLACK << 4) | VGA_COLOR_LIGHT_GREEN)
#define RED_ON_BLACK    ((VGA_COLOR_BLACK << 4) | VGA_COLOR_LIGHT_RED)
#define YELLOW_ON_BLACK ((VGA_COLOR_BLACK << 4) | VGA_COLOR_LIGHT_BROWN)
#define WHITE_ON_BLACK  ((VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE)
#define WHITE_ON_BLUE   ((VGA_COLOR_BLUE << 4) | VGA_COLOR_WHITE)

// Core VGA functions
void vga_initialize(void);
void vga_clear(void);
void vga_setcolor(uint8_t color);

// Character output
void vga_putchar(char c);
void vga_writestring(const char* str);

// Positioned output
void vga_putchar_at(char c, int x, int y, uint8_t color);
void vga_print_at(const char* str, int x, int y, uint8_t color);

// Cursor management
void vga_get_cursor(size_t* x, size_t* y);
void vga_set_cursor(size_t x, size_t y);

// Screen management
void vga_scroll(void);
void vga_clear_line(size_t line);
void vga_fill_area(size_t x, size_t y, size_t width, size_t height, char c, uint8_t color);

// Number output (essential for debugging)
void vga_print_int(int value);
void vga_print_hex(uint32_t value);

#endif // VGA_H