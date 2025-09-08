// Read a byte from a port
unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void kernel_main() {
    volatile unsigned short* video = (unsigned short*)0xB8000;
    int width = 80;
    int height = 25;

    // Clear screen
    for (int row = 0; row < height; row++)
        for (int col = 0; col < width; col++)
            video[row * width + col] = ' ' | 0x0200;

    int cursor_row = 0;
    int cursor_col = 0;

    // Print prompt
    void print_char(char c) {
        if (c == '\n') {
            cursor_row++;
            cursor_col = 0;
        } else {
            video[cursor_row * width + cursor_col] = c | 0x0200;
            cursor_col++;
            if (cursor_col >= width) {
                cursor_col = 0;
                cursor_row++;
            }
        }
    }

    void print_string(const char* s) {
        int i = 0;
        while (s[i] != '\0') {
            print_char(s[i]);
            i++;
        }
    }

    char input[80];
    int input_len = 0;

    print_string("mini-kernel> ");

    while (1) {
        for (volatile int i = 0; i < 22000000; i++) {
        // do nothing, just waste time
        }
        unsigned char scancode = inb(0x60); // read keyboard port

        // Only handle key press, ignore release (high bit)
        if (scancode & 0x80) continue;

        // Wipe last character if Backspace
        if (scancode == 0x0E && input_len > 0) { // Backspace
            input_len--;
            cursor_col--;
            video[cursor_row * width + cursor_col] = ' ' | 0x0200;
            continue;
        }

        // Enter key
        if (scancode == 0x1C) { // Enter
            print_char('\n');
            input[input_len] = '\0';

            // Compare commands manually
            int match = 1;
            char cmd_hello[] = "hello";
            int j = 0;
            while (cmd_hello[j] != '\0') {
                if (input[j] != cmd_hello[j]) { match = 0; break; }
                j++;
            }

            if (match && input_len == j) {
                print_string("Hello, User!\n");
            } else {
                print_string("Unknown command\n");
            }

            // Reset input buffer
            input_len = 0;
            print_string("mini-kernel> ");
            continue;
        }

        // Map simple A-Z, 0-9 (scan code to ASCII)
        char c = 0;
        if (scancode >= 0x02 && scancode <= 0x0B) c = '1' + (scancode - 0x02);
        else if (scancode >= 0x10 && scancode <= 0x19) c = 'q' + (scancode - 0x10);
        else if (scancode >= 0x1E && scancode <= 0x26) c = 'a' + (scancode - 0x1E);
        else if (scancode >= 0x2C && scancode <= 0x32) c = 'z' + (scancode - 0x2C);

        if (c != 0 && input_len < 79) {
            input[input_len++] = c;
            print_char(c);
        }
    }
}

