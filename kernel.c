/* Enhanced keyboard + hex dumper + tiny prompt kernel (PS/2 Set 1) */
/* compile as freestanding 32-bit (see notes below) */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

volatile uint16_t* const video = (uint16_t*)0xB8000;
int width = 80;
int height = 25;
int cursor_row = 0;
int cursor_col = 0;

/* I/O: read byte from port */
static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

/* Convert byte -> 2-char hex (null-terminated) */
void byte_to_hex(uint8_t v, char *buf) {
    const char *hex = "0123456789ABCDEF";
    buf[0] = hex[(v >> 4) & 0xF];
    buf[1] = hex[v & 0xF];
    buf[2] = '\0';
}

/* scroll screen up by one line */
void scroll_up() {
    int r, c;
    for (r = 1; r < height; r++) {
        for (c = 0; c < width; c++) {
            video[(r - 1) * width + c] = video[r * width + c];
        }
    }
    /* clear last row */
    for (c = 0; c < width; c++) video[(height - 1) * width + c] = ' ' | 0x0200;
    if (cursor_row > 0) cursor_row--;
}

/* print a char at cursor, handle newline and simple scrolling */
void print_char(char ch) {
    if (ch == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        if (cursor_row >= height) scroll_up();
        video[cursor_row * width + cursor_col] = (uint16_t)ch | 0x0200; // dark green
        cursor_col++;
        if (cursor_col >= width) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    if (cursor_row >= height) scroll_up();
}

/* print null-terminated string */
void print_string(const char* s) {
    int i = 0;
    while (s[i]) { print_char(s[i]); i++; }
}

/* Complete PS/2 Set 1 keymap - normal keys */
const char keymap_normal[128] = {
    0,    27,   '1',  '2',  '3',  '4',  '5',  '6',    // 0x00-0x07
    '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',   // 0x08-0x0F
    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',    // 0x10-0x17
    'o',  'p',  '[',  ']',  '\n', 0,    'a',  's',    // 0x18-0x1F
    'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',    // 0x20-0x27
    '\'', '`',  0,    '\\', 'z',  'x',  'c',  'v',    // 0x28-0x2F
    'b',  'n',  'm',  ',',  '.',  '/',  0,    '*',    // 0x30-0x37
    0,    ' ',  0,    0,    0,    0,    0,    0,      // 0x38-0x3F (F1-F5)
    0,    0,    0,    0,    0,    0,    0,    '7',    // 0x40-0x47 (F6-F10, NumLock, ScrollLock, KP7)
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',    // 0x48-0x4F (KP8-KP1)
    '2',  '3',  '0',  '.',  0,    0,    0,    0,      // 0x50-0x57 (KP2-KP0, KPDot, F11, F12)
    0     // 0x58-0x7F (rest zeros)
};

/* Complete PS/2 Set 1 keymap - shifted keys */
const char keymap_shifted[128] = {
    0,    27,   '!',  '@',  '#',  '$',  '%',  '^',    // 0x00-0x07
    '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',   // 0x08-0x0F
    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',    // 0x10-0x17
    'O',  'P',  '{',  '}',  '\n', 0,    'A',  'S',    // 0x18-0x1F
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',    // 0x20-0x27
    '"',  '~',  0,    '|',  'Z',  'X',  'C',  'V',    // 0x28-0x2F
    'B',  'N',  'M',  '<',  '>',  '?',  0,    '*',    // 0x30-0x37
    0,    ' ',  0,    0,    0,    0,    0,    0,      // 0x38-0x3F
    0,    0,    0,    0,    0,    0,    0,    '7',    // 0x40-0x47
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',    // 0x48-0x4F
    '2',  '3',  '0',  '.',  0,    0,    0,    0,      // 0x50-0x57
    0     // 0x58-0x7F
};

/* Scan code definitions */
#define SC_ESCAPE       0x01
#define SC_BACKSPACE    0x0E
#define SC_TAB          0x0F
#define SC_ENTER        0x1C
#define SC_LCTRL        0x1D
#define SC_LSHIFT       0x2A
#define SC_RSHIFT       0x36
#define SC_LALT         0x38
#define SC_SPACE        0x39
#define SC_CAPSLOCK     0x3A
#define SC_F1           0x3B
#define SC_F2           0x3C
#define SC_F3           0x3D
#define SC_F4           0x3E
#define SC_F5           0x3F
#define SC_F6           0x40
#define SC_F7           0x41
#define SC_F8           0x42
#define SC_F9           0x43
#define SC_F10          0x44
#define SC_NUMLOCK      0x45
#define SC_SCROLLLOCK   0x46
#define SC_F11          0x57
#define SC_F12          0x58

/* Keyboard state */
struct {
    int shift_pressed;
    int ctrl_pressed;
    int alt_pressed;
    int caps_lock;
    int num_lock;
    int extended_key;  /* for E0 prefix */
} kbd_state = {0, 0, 0, 0, 1, 0}; /* num_lock starts on */

/* small static input buffer */
char input[80];
int input_len = 0;

void clear_screen() {
    for (int r=0;r<height;r++)
        for (int c=0;c<width;c++)
            video[r*width + c] = ' ' | 0x0200;
    cursor_row = 0; cursor_col = 0;
}

/* Handle special keys and return ASCII char or 0 */
char process_scancode(uint8_t scancode) {
    /* Check if this is an extended key (E0 prefix) */
    if (scancode == 0xE0) {
        kbd_state.extended_key = 1;
        return 0;
    }
    
    /* Check for break code (key release) */
    if (scancode & 0x80) {
        uint8_t make_code = scancode & 0x7F;
        
        /* Handle modifier key releases */
        switch (make_code) {
            case SC_LSHIFT:
            case SC_RSHIFT:
                kbd_state.shift_pressed = 0;
                break;
            case SC_LCTRL:
                if (kbd_state.extended_key) {
                    /* Right Ctrl release */
                } else {
                    kbd_state.ctrl_pressed = 0;
                }
                break;
            case SC_LALT:
                kbd_state.alt_pressed = 0;
                break;
        }
        kbd_state.extended_key = 0;
        return 0;
    }
    
    /* Handle make codes (key press) */
    switch (scancode) {
        case SC_LSHIFT:
        case SC_RSHIFT:
            kbd_state.shift_pressed = 1;
            kbd_state.extended_key = 0;
            return 0;
            
        case SC_LCTRL:
            if (kbd_state.extended_key) {
                /* Right Ctrl press */
            } else {
                kbd_state.ctrl_pressed = 1;
            }
            kbd_state.extended_key = 0;
            return 0;
            
        case SC_LALT:
            kbd_state.alt_pressed = 1;
            kbd_state.extended_key = 0;
            return 0;
            
        case SC_CAPSLOCK:
            kbd_state.caps_lock = !kbd_state.caps_lock;
            kbd_state.extended_key = 0;
            return 0;
            
        case SC_NUMLOCK:
            kbd_state.num_lock = !kbd_state.num_lock;
            kbd_state.extended_key = 0;
            return 0;
            
        case SC_F1:
            print_string("\n[F1 pressed]\n");
            kbd_state.extended_key = 0;
            return 0;
            
        case SC_F2:
            clear_screen();
            print_string("mini-kernel> ");
            kbd_state.extended_key = 0;
            return 0;
    }
    
    /* Handle extended keys (navigation, etc.) */
    if (kbd_state.extended_key) {
        kbd_state.extended_key = 0;
        switch (scancode) {
            case 0x48: /* Up arrow */
                print_string("\n[UP]\n");
                return 0;
            case 0x50: /* Down arrow */
                print_string("\n[DOWN]\n");  
                return 0;
            case 0x4B: /* Left arrow */
                print_string("\n[LEFT]\n");
                return 0;
            case 0x4D: /* Right arrow */
                print_string("\n[RIGHT]\n");
                return 0;
            case 0x47: /* Home */
                print_string("\n[HOME]\n");
                return 0;
            case 0x4F: /* End */
                print_string("\n[END]\n");
                return 0;
            case 0x52: /* Insert */
                print_string("\n[INSERT]\n");
                return 0;
            case 0x53: /* Delete */
                print_string("\n[DELETE]\n");
                return 0;
        }
        return 0;
    }
    
    /* Get basic character */
    if (scancode >= 128) return 0;
    
    char ch;
    if (kbd_state.shift_pressed) {
        ch = keymap_shifted[scancode];
    } else {
        ch = keymap_normal[scancode];
    }
    
    /* Apply caps lock to letters */
    if (kbd_state.caps_lock && ch >= 'a' && ch <= 'z') {
        ch = ch - 'a' + 'A';
    } else if (kbd_state.caps_lock && ch >= 'A' && ch <= 'Z' && kbd_state.shift_pressed) {
        ch = ch - 'A' + 'a';
    }
    
    kbd_state.extended_key = 0;
    return ch;
}

void kernel_main() {
    clear_screen();
    print_string("Enhanced mini-kernel v1.1\n");
    print_string("Features: Complete keymap, Shift, Caps, F-keys, arrows\n");
    print_string("F1=Help, F2=Clear, Ctrl+Alt for special combos\n\n");
    print_string("mini-kernel> ");

    uint8_t last_scancode = 0;  /* Track last scancode to avoid duplicates */

    while (1) {
        for(int i = 0; i < 25000000; i++);
        uint8_t scancode = inb(0x60);

        //* Check both status and that scancode changed */

        // /* debug: print raw scancode in hex and a space */
        // char hexbuf[3];
        // byte_to_hex(scancode, hexbuf);
        // print_string(hexbuf);
        // print_char(' ');
        if (scancode == 0 || scancode == last_scancode) continue;

        /* Process the scancode */
        char ch = process_scancode(scancode);
        
        /* Handle special control characters */
        if (ch == '\b') { /* Backspace */
            if (input_len > 0) {
                input_len--;
                if (cursor_col > 0) {
                    cursor_col--;
                    video[cursor_row * width + cursor_col] = ' ' | 0x0200;
                }
            }
            continue;
        }

        if (ch == '\n') { /* Enter */
            print_char('\n');
            input[input_len] = '\0';
            
            /* Enhanced command processing */
            if (input_len == 0) {
                print_string("mini-kernel> ");
                continue;
            }
            
            /* Simple string comparison function */
            int str_equals(const char* a, const char* b) {
                int i = 0;
                while (a[i] && b[i]) {
                    if (a[i] != b[i]) return 0;
                    i++;
                }
                return a[i] == b[i];
            }
            
            /* Command processing */
            if (str_equals(input, "hello")) {
                print_string("Hello, User!\n");
            } else if (str_equals(input, "help")) {
                print_string("Available commands:\n");
                print_string("  hello  - Say hello\n");
                print_string("  help   - Show this help\n");
                print_string("  clear  - Clear screen\n");
                print_string("  info   - Show keyboard state\n");
                print_string("  test   - Test special keys\n");
            } else if (str_equals(input, "clear")) {
                clear_screen();
            } else if (str_equals(input, "info")) {
                print_string("Keyboard state:\n");
                print_string("  Shift: "); print_string(kbd_state.shift_pressed ? "ON" : "OFF"); print_char('\n');
                print_string("  Ctrl:  "); print_string(kbd_state.ctrl_pressed ? "ON" : "OFF"); print_char('\n');
                print_string("  Alt:   "); print_string(kbd_state.alt_pressed ? "ON" : "OFF"); print_char('\n');
                print_string("  Caps:  "); print_string(kbd_state.caps_lock ? "ON" : "OFF"); print_char('\n');
                print_string("  Num:   "); print_string(kbd_state.num_lock ? "ON" : "OFF"); print_char('\n');
            } else if (str_equals(input, "test")) {
                print_string("Try: Arrow keys, F1-F12, Insert, Delete, Home, End\n");
                print_string("     Shift+letters, number symbols, Caps Lock\n");
            } else {
                print_string("Unknown command: ");
                print_string(input);
                print_string(" (type 'help' for commands)\n");
            }
            
            input_len = 0;
            print_string("mini-kernel> ");
            continue;
        }

        /* Handle printable characters */
        if (ch && ch != 27) { /* Not ESC */
            if (input_len < (int)(sizeof(input)-1)) {
                input[input_len++] = ch;
                print_char(ch);
            }
        }
    }
}