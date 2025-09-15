#include "vga.h"
#include <stdint.h>
#include <stddef.h>

// VGA memory
static uint16_t* terminal_buffer = (uint16_t*)0xB8000;
static size_t terminal_row = 0;
static size_t terminal_col = 0;
static uint8_t terminal_color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE;

// ================= BASIC FUNCTIONS =================

void vga_setcolor(uint8_t color) {
    terminal_color = color;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = ((uint16_t)color << 8) | c;
}

void vga_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_putentryat(' ', terminal_color, x, y);
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}

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

    // Scroll if needed
    if (terminal_row >= VGA_HEIGHT) {
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                terminal_buffer[(y-1)*VGA_WIDTH+x] = terminal_buffer[y*VGA_WIDTH+x];
            }
        }
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(VGA_HEIGHT-1)*VGA_WIDTH + x] = ((uint16_t)terminal_color << 8) | ' ';
        }
        terminal_row = VGA_HEIGHT - 1;
    }
}

void vga_writestring(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

void vga_initialize(void) {
    vga_clear();
    terminal_color = (VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE;
    terminal_row = 0;
    terminal_col = 0;
}

// ================= HACKER-STYLE FUNCTIONS =================

// Simple typewriter effect
void vga_typewriter(const char* str, uint32_t delay_ms) {
    for (size_t i = 0; str[i]; i++) {
        vga_putchar(str[i]);
        for (volatile int j = 0; j < 500000 * delay_ms; j++); // crude delay
    }
}

// Rainbow text
void vga_writestring_rainbow(const char* str) {
    uint8_t colors[] = {VGA_COLOR_LIGHT_RED, VGA_COLOR_LIGHT_GREEN, VGA_COLOR_LIGHT_BLUE,
                        VGA_COLOR_LIGHT_CYAN, VGA_COLOR_LIGHT_MAGENTA, VGA_COLOR_LIGHT_BROWN};
    size_t n_colors = sizeof(colors)/sizeof(colors[0]);
    size_t idx = 0;
    for (size_t i = 0; str[i]; i++) {
        vga_setcolor((VGA_COLOR_BLACK << 4) | colors[idx]);
        vga_putchar(str[i]);
        idx = (idx + 1) % n_colors;
    }
}

// Print hex
void vga_writehex(uint32_t value) {
    const char* hex_chars = "0123456789ABCDEF";
    vga_writestring("0x");
    for (int i = 28; i >= 0; i -= 4) {
        vga_putchar(hex_chars[(value >> i) & 0xF]);
    }
}

// Print binary
void vga_writebin(uint32_t value) {
    vga_writestring("0b");
    for (int i = 31; i >= 0; i--) {
        vga_putchar((value & (1 << i)) ? '1' : '0');
    }
}

// Matrix effect
void vga_matrix_effect(void) {
    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        uint8_t color = (VGA_COLOR_BLACK << 4) | (i % 15 + 1);
        terminal_buffer[i] = ((uint16_t)color << 8) | (33 + i % 94); // printable ASCII
    }
}

// Clear single line
void vga_clear_line(size_t row) {
    if (row >= VGA_HEIGHT) return;
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_putentryat(' ', terminal_color, x, row);
    }
}

// Center text on current row
void vga_print_center(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    size_t start_col = (VGA_WIDTH - len) / 2;
    for (size_t i = 0; i < len; i++) {
        vga_putentryat(str[i], terminal_color, start_col + i, terminal_row);
    }
    terminal_row++;
    terminal_col = 0;
}
