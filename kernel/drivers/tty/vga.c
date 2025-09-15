#include "vga.h"
#include <stdint.h>
#include <stddef.h>

static size_t terminal_row;
static size_t terminal_col;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
static uint8_t terminal_color;
static uint16_t* terminal_buffer = (uint16_t*)0xB8000; // VGA memory

void vga_initialize(void) {
    // Point to VGA text mode memory
    terminal_buffer = (uint16_t*)0xB8000;

    // Set default color: white on black
    terminal_color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE;

    // Start at top-left
    terminal_row = 0;
    terminal_col = 0;

    // Clear screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
}

// Current text color for printing
void vga_setcolor(uint8_t color) {
    terminal_color = color;
}

// Places a single character c at coordinates (x, y) on the screen.
// Uses the specified color for foreground/background.
// Writes directly to VGA memory at 0xB8000.
void vga_putentryat(char c, uint8_t color, size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return; // ignore out-of-bounds coordinates

    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = ((uint16_t)color << 8) | c;
}


// Scroll screen up by one row
static void vga_scroll(void) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }
    // Clear the last row
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((uint16_t)terminal_color << 8) | ' ';
    }
    terminal_row = VGA_HEIGHT - 1;
    terminal_col = 0;
}

// Print a single character at current cursor
void vga_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
    } else if (c == '\r') {
        terminal_col = 0;
    } else {
        vga_putentryat(c, terminal_color, terminal_col, terminal_row);
        terminal_col++;
        if (terminal_col >= VGA_WIDTH) {
            terminal_col = 0;
            terminal_row++;
        }
    }

    if (terminal_row >= VGA_HEIGHT) {
        vga_scroll();
    }
}

// prints a string
void vga_writestring(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

// clears the screen
void vga_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}