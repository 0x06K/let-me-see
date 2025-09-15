//  kmain.h - Kernel main function declarations and definitions

#ifndef KMAIN_H
#define KMAIN_H

// Kernel version and identification
#define KERNEL_VERSION "0.1.0"
#define KERNEL_NAME "idk-just-lookatmyos"
#define KERNEL_ARCH "x86-32"

// Function declarations

/*
 * Main kernel entry point
 * Called from bootloader after switching to 32-bit protected mode
 */
void kmain(void);


/**
 * Early kernel initialization
 * Initialize only the most critical systems (terminal, basic setup)
 */
void kernel_early_init(void);

/**
 * Late kernel initialization  
 * Initialize additional systems after basic output is working
 */
void kernel_late_init(void);

/**
 * Main kernel loop
 * Where the kernel spends most of its time (scheduler, idle, etc.)
 */
void kernel_main_loop(void);

/**
 * Print kernel information and welcome banner
 * Displays kernel version, system info, etc.
 */
void print_kernel_info(void);

/**
 * Kernel panic function
 * Called when a critical unrecoverable error occurs
 * @param message Error message to display
 */
void kernel_panic(const char* message);

// Kernel status flags (for future use)
typedef enum {
    KERNEL_INITIALIZING,
    KERNEL_RUNNING,
    KERNEL_IDLE,
    KERNEL_PANIC
} kernel_status_t;

// External kernel status variable (define in kmain.c)
extern kernel_status_t kernel_status;

#endif
