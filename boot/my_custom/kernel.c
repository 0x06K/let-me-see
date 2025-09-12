// kernel.c - Minimal 32-bit kernel
// Compile with: i686-elf-gcc -ffreestanding -m32 -c kernel.c -o kernel.o

void kernel_main() {
    // Simple message to test
    char *video_memory = (char *)0xB8000; // VGA text mode buffer
    const char *msg = "Hello, kernel!";
    
    for (int i = 0; msg[i] != '\0'; i++) {
        video_memory[i*2] = msg[i];      // Character
        video_memory[i*2 + 1] = 0x07;    // Attribute byte (light gray on black)
    }

    // Infinite loop to halt kernel
    while (1) {}
}
