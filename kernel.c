// kernel.c
typedef unsigned short uint16_t;

void kernel_main() {
    volatile uint16_t* video = (uint16_t*)0xB8000;
    const char* msg = "Hello World !!";
    for (int i = 0; msg[i] != '\0'; i++) {
        video[i] = (uint16_t)msg[i] | 0x0F00; // white on black
    }
    while(1) { }
}
