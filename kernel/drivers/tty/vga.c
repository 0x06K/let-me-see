#include "vga.h"
#include <stdint.h>
#include <stddef.h>

static uint16_t* vga = (uint16_t*)0xB8000;
static size_t cursor_x = 0;
static size_t cursor_y = 0;
static uint8_t current_color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE;

void vga_initialize(void) {
    cursor_x = 0;
    cursor_y = 0;
    current_color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE;
}

void vga_setcolor(uint8_t color) {
    current_color = color;
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = ((uint16_t)current_color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_scroll(void) {
    // Move rows up by one
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga[(y-1) * VGA_WIDTH + x] = vga[y * VGA_WIDTH + x];
        }
    }
    // Clear bottom row
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga[(VGA_HEIGHT-1) * VGA_WIDTH + x] = ((uint16_t)current_color << 8) | ' ';
    }
    cursor_y = VGA_HEIGHT - 1;
    cursor_x = 0;
}

void vga_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0; cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~7; // align to 8
    } else if (c == '\b') {
        if (cursor_x > 0) cursor_x--;
    } else {
        vga[cursor_y * VGA_WIDTH + cursor_x] = ((uint16_t)current_color << 8) | c;
        cursor_x++;
    }
    
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0; cursor_y++;
    }
    if (cursor_y >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_writestring(const char* str) {
    while (*str) {
        vga_putchar(*str++);
    }
}

void vga_putchar_at(char c, int x, int y, uint8_t color) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        vga[y * VGA_WIDTH + x] = ((uint16_t)color << 8) | c;
    }
}

void vga_print_at(const char* str, int x, int y, uint8_t color) {
    for (int i = 0; str[i] && x + i < VGA_WIDTH; i++) {
        vga[y * VGA_WIDTH + x + i] = ((uint16_t)color << 8) | str[i];
    }
}

// Get cursor position (useful for kernel messages)
void vga_get_cursor(size_t* x, size_t* y) {
    *x = cursor_x;
    *y = cursor_y;
}
void vga_set_cursor(size_t x, size_t y) {
    if (x < VGA_WIDTH && y < VGA_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
    }
}

// Clear specific line (useful for status updates)
void vga_clear_line(size_t line) {
    if (line < VGA_HEIGHT) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga[line * VGA_WIDTH + x] = ((uint16_t)current_color << 8) | ' ';
        }
    }
}

// Fill area with character (useful for progress bars, borders)
void vga_fill_area(size_t x, size_t y, size_t width, size_t height, char c, uint8_t color) {
    for (size_t dy = 0; dy < height && y + dy < VGA_HEIGHT; dy++) {
        for (size_t dx = 0; dx < width && x + dx < VGA_WIDTH; dx++) {
            vga[(y + dy) * VGA_WIDTH + (x + dx)] = ((uint16_t)color << 8) | c;
        }
    }
}

// Print integer (essential for debugging/printf)
void vga_print_int(int value) {
    if (value < 0) {
        vga_putchar('-');
        value = -value;
    }
    
    if (value == 0) {
        vga_putchar('0');
        return;
    }
    
    char buffer[12];
    int i = 0;
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    // Print in reverse
    while (i > 0) {
        vga_putchar(buffer[--i]);
    }
}

// Print hex (essential for memory addresses, debugging)
void vga_print_hex(uint32_t value) {
    const char hex_chars[] = "0123456789ABCDEF";
    vga_writestring("0x");
    for (int i = 28; i >= 0; i -= 4) {
        vga_putchar(hex_chars[(value >> i) & 0xF]);
    }
}