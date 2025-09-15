#include <stdint.h>

#define VGA 0xB8000

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64


uint16_t* vga = (uint16_t*)VGA;
uint16_t cursor = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0":"=a"(ret):"Nd"(port));
    return ret;
}

void print_char(char c){
    if(c=='\n'){ cursor+=80-(cursor%80); return; }
    vga[cursor++]=(0x0F<<8)|c;
}

void print_string(const char* s){
    while(*s) print_char(*s++);
}



void kmain() {
    print_string("Keyboard Polling Test\n");

    while(1){
        // check if output buffer full (key pressed)
        if(inb(KEYBOARD_STATUS) & 1){
            uint8_t scancode = inb(KEYBOARD_DATA);
            char hex[3];
            const char* digits = "0123456789ABCDEF";
            hex[0] = digits[(scancode>>4)&0xF];
            hex[1] = digits[scancode & 0xF];
            hex[2] = 0;
            print_string("SC:");
            print_string(hex);
            print_char('\n');
        }
    }
}
