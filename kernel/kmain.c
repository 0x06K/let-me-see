/**
 * kmain.c - Complete function implementations
 * Detailed definitions for all kernel main functions
 */

 #include "../include/types.h"
 #include "../drivers/terminal/terminal.h"
 #include "../system/gdt/gdt.h"
 #include "../system/idt/idt.h"
 #include "../lib/string.h"
 
 // Kernel version and status
 #define KERNEL_VERSION "0.1.0"
 #define KERNEL_NAME "MyOS"
 #define KERNEL_ARCH "x86-32"
 
 // Global kernel status
 kernel_status_t kernel_status = KERNEL_INITIALIZING;
 
 // Function counters for debugging
 static uint32_t interrupt_count = 0;
 static uint32_t panic_count = 0;
 
 /**
  * kmain - Main kernel entry point
  * 
  * This is the first C function called after the bootloader transfers control.
  * It orchestrates the entire kernel initialization process and enters the
  * main execution loop.
  * 
  * Execution flow:
  * 1. Early initialization (critical systems)
  * 2. Display kernel information  
  * 3. Late initialization (additional systems)
  * 4. Enter main kernel loop (never returns)
  * 
  * @param None
  * @return Never returns
  */
 void kmain(void)
 {
     // Set kernel status to initializing
     kernel_status = KERNEL_INITIALIZING;
     
     // Phase 1: Early initialization - critical systems only
     // Must succeed or kernel cannot continue
     kernel_early_init();
     
     // Phase 2: Display kernel information
     // Shows user that kernel loaded successfully
     print_kernel_info();
     
     // Phase 3: Late initialization - additional systems
     // Can use terminal output for debugging since early_init completed
     kernel_late_init();
     
     // Initialization complete - kernel is ready
     kernel_status = KERNEL_RUNNING;
     terminal_writestring("[INFO] Kernel initialization complete!\n");
     terminal_writestring("[INFO] System ready for operation.\n\n");
     
     // Phase 4: Enter main execution loop
     // This never returns - kernel runs until power off
     kernel_main_loop();
     
     // Should never reach here
     kernel_panic("kmain() returned - this should never happen!");
 }
 
 /*
  * kernel_early_init - Initialize critical systems
  * 
  * Initializes only the most essential systems needed for basic operation.
  * These systems are dependencies for everything else, so they must be
  * initialized first and must succeed.
  * 
  * Critical systems:
  * - Terminal/VGA driver (for debug output)
  * - Basic memory management (if needed)
  * - GDT reinitialization (optional)
  */
 void kernel_early_init(void)
 {
     // Initialize terminal first - we need output for debugging
     terminal_initialize();
     if (!terminal_is_initialized()) {
         // Cannot call kernel_panic() - no terminal output available
         // Hang the system - something is seriously wrong
         asm volatile("cli; hlt");
     }
     
     terminal_writestring("[EARLY] Starting kernel early initialization...\n");
     
     // Optional: Reinitialize GDT to replace bootloader's minimal setup
     // Uncomment if you want more control over memory segmentation
     /*
     if (gdt_init() != 0) {
         kernel_panic("Failed to initialize GDT");
     }
     terminal_writestring("[EARLY] GDT reinitialized\n");
     */
     
     // Initialize basic memory structures if needed
     // This might include setting up heap, page directory, etc.
     
     terminal_writestring("[EARLY] Critical systems initialized\n");
     terminal_writestring("[EARLY] Early initialization complete\n\n");
 }
 
 /**
  * kernel_late_init - Initialize additional systems
  * 
  * Initializes non-critical systems after basic output is working.
  * These systems enhance functionality but aren't required for basic operation.
  * Failures here may be recoverable or result in reduced functionality.
  * 
  * Additional systems:
  * - Interrupt handling (IDT)
  * - Device drivers (keyboard, timer, etc.)
  * - Memory management
  * - File systems
  * - Process management
  * 
  * @param None
  * @return void
  */
 void kernel_late_init(void)
 {

    
     terminal_writestring("[LATE] Starting late initialization...\n");
     
     // Initialize Interrupt Descriptor Table
     // This is critical - without it, we can't handle hardware interrupts
     if (idt_init() != 0) {
         kernel_panic("Failed to initialize IDT - cannot handle interrupts");
     }
     terminal_writestring("[LATE] IDT initialized - interrupt handling ready\n");
     
     // Enable hardware interrupts
     // Now the CPU can respond to keyboard, timer, etc.
     asm volatile("sti");
     terminal_writestring("[LATE] Hardware interrupts enabled\n");
     
     // Initialize device drivers
     // Add these as you implement them:
     /*
     if (keyboard_init() != 0) {
         terminal_writestring("[LATE] Warning: Keyboard driver failed\n");
         // Continue anyway - system still usable
     } else {
         terminal_writestring("[LATE] Keyboard driver initialized\n");
     }
     
     if (timer_init() != 0) {
         terminal_writestring("[LATE] Warning: Timer driver failed\n");
     } else {
         terminal_writestring("[LATE] Timer driver initialized\n");
     }
     
     if (memory_manager_init() != 0) {
         kernel_panic("Failed to initialize memory manager");
     }
     terminal_writestring("[LATE] Memory manager initialized\n");
     */
     
     terminal_writestring("[LATE] Late initialization complete\n\n");
 }
 
 /**
  * kernel_main_loop - Main kernel execution loop
  * 
  * This is where the kernel spends most of its time during normal operation.
  * Currently implements a simple idle loop, but will evolve to include:
  * - Process scheduling
  * - System call handling  
  * - Background tasks
  * - Resource management
  * 
  * The loop uses HLT instruction to reduce CPU usage when idle.
  * 
  * @param None
  * @return Never returns
  */
 void kernel_main_loop(void)
 {
     kernel_status = KERNEL_IDLE;
     
     terminal_writestring("Entering kernel main loop...\n");
     terminal_writestring("Kernel is now idle and responding to interrupts.\n");
     terminal_writestring("System is fully operational.\n\n");
     
     // Main kernel loop - this runs forever
     while(1) {
         // Halt CPU until next interrupt
         // This saves power and reduces heat
         asm volatile("hlt");
         
         // When we reach here, an interrupt occurred
         interrupt_count++;
         
         // Future functionality will be added here:
         // - Check for runnable processes (scheduler)
         // - Handle system calls
         // - Perform background maintenance
         // - Update system timers
         // - Balance system resources
         
         // For now, just return to idle state
         kernel_status = KERNEL_IDLE;
     }
     
     // Should never reach here
     kernel_panic("Main kernel loop exited - impossible!");
 }
 
 /**
  * print_kernel_info - Display kernel information banner
  * 
  * Shows kernel version, system information, and capabilities.
  * Provides visual confirmation that the kernel loaded successfully
  * and gives user/developer information about the system.
  * 
  * @param None
  * @return void
  */
 void print_kernel_info(void)
 {
     // Clear screen for clean display
     terminal_clear();
     
     // Print main banner
     terminal_writestring("========================================\n");
     terminal_writestring("  Welcome to ");
     terminal_writestring(KERNEL_NAME);
     terminal_writestring(" v");
     terminal_writestring(KERNEL_VERSION);
     terminal_writestring("\n");
     terminal_writestring("  ");
     terminal_writestring(KERNEL_ARCH);
     terminal_writestring(" Protected Mode Kernel\n");
     terminal_writestring("========================================\n\n");
     
     // System information
     terminal_writestring("[INFO] Architecture: x86 32-bit Protected Mode\n");
     terminal_writestring("[INFO] Memory Model: Flat segmentation\n");
     terminal_writestring("[INFO] Display: VGA text mode 80x25\n");
     
     // Build information (you can expand this)
     terminal_writestring("[INFO] Build: Debug version\n");
     terminal_writestring("[INFO] Compiler: GCC cross-compiler\n");
     
     // Memory information (add when memory detection is implemented)
     /*
     terminal_writestring("[INFO] Available Memory: ");
     terminal_write_dec(get_available_memory_kb());
     terminal_writestring(" KB\n");
     */
     
     terminal_writestring("\n");
     terminal_writestring("Starting system initialization...\n\n");
 }
 
 /**
  * kernel_panic - Handle critical unrecoverable errors
  * 
  * Called when a fatal error occurs that prevents normal kernel operation.
  * This function:
  * 1. Disables all interrupts to prevent further damage
  * 2. Changes display to red panic screen
  * 3. Shows error information
  * 4. Halts the system completely
  * 
  * This is a last resort - once called, the system cannot continue.
  * 
  * @param message - Descriptive error message to display
  * @return Never returns (system halted)
  */
 void kernel_panic(const char* message)
 {
     // Increment panic counter for debugging
     panic_count++;
     
     // Disable all interrupts immediately
     // Prevents further interrupts from causing more problems
     asm volatile("cli");
     
     // Update kernel status
     kernel_status = KERNEL_PANIC;
     
     // Change to red background for panic screen
     // Makes it obvious something is wrong
     terminal_setcolor(VGA_COLOR_WHITE, VGA_COLOR_RED);
     terminal_clear();
     
     // Display panic header
     terminal_writestring("*** KERNEL PANIC ***\n\n");
     
     // Show system information
     terminal_writestring("System: ");
     terminal_writestring(KERNEL_NAME);
     terminal_writestring(" v");
     terminal_writestring(KERNEL_VERSION);
     terminal_writestring("\n");
     
     terminal_writestring("Architecture: ");
     terminal_writestring(KERNEL_ARCH);
     terminal_writestring("\n\n");
     
     // Display the error message
     terminal_writestring("A critical error has occurred:\n");
     terminal_writestring(message);
     terminal_writestring("\n\n");
     
     // Show additional debug information
     terminal_writestring("Debug Information:\n");
     terminal_writestring("- Interrupts processed: ");
     terminal_write_dec(interrupt_count);
     terminal_writestring("\n");
     terminal_writestring("- Previous panics: ");
     terminal_write_dec(panic_count - 1);
     terminal_writestring("\n\n");
     
     // Instructions for user
     terminal_writestring("The system has been halted to prevent damage.\n");
     terminal_writestring("Please restart your computer.\n\n");
     terminal_writestring("System halted.\n");
     
     // Infinite loop with halt instruction
     // System cannot continue - wait for manual restart
     while(1) {
         asm volatile("hlt");
     }
 }