#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

// VGA screen dimensions
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
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Basic functions
void vga_initialize(void);
void vga_setcolor(uint8_t color);
void vga_putentryat(char c, uint8_t color, size_t x, size_t y);
void vga_putchar(char c);
void vga_writestring(const char* str);
void vga_clear(void);

// Advanced / Hacker style
void vga_typewriter(const char* str, uint32_t delay_ms);
void vga_writestring_rainbow(const char* str);
void vga_writehex(uint32_t value);
void vga_writebin(uint32_t value);
void vga_matrix_effect(void);
void vga_clear_line(size_t row);
void vga_print_center(const char* str);

#endif
