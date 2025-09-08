typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void clear_screen() {
    volatile unsigned short* video = (unsigned short*)0xB8000;
    int row, col;
    for (row = 0; row < 25; row++) {
        for (col = 0; col < 80; col++) {
            int index = row * 80 + col;
            video[index] = ' ' | 0x0200; // dark green on black space
        }
    }
}

void kernel_main() {
    clear_screen();
    // VGA text buffer starts at 0xB8000
    volatile uint16_t* video = (uint16_t*)0xB8000;

    // Define screen size manually
    uint8_t width = 80;   // number of columns
    uint8_t height = 25;  // number of rows

    // Messages to print
    const char* messages[] = {
        "==============================",
        "   My First Manual Kernel",
        "==============================",
        "1. Option One",
        "2. Option Two",
        "3. Option Three",
        "==============================",
        "Press any key to continue..."
    };

    // Manually calculate number of lines
    uint8_t num_lines = 0;
    while (messages[num_lines] != 0) {
        num_lines++;
    }

    // Print each line
    for (uint8_t row = 0; row < num_lines; row++) {
        const char* line = messages[row];
        uint8_t col = 0;
        while (line[col] != '\0') {
            // Manual index calculation: row * width + column
            uint16_t index = row;
            index = index * width + col;

            // Dark green on black = 0x02 in upper byte
            video[index] = ((uint16_t)line[col]) | 0x0200;

            col++;
        }
    }

    // Infinite loop to keep kernel running
    while(1) { }
}
