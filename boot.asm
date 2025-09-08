; boot.asm
; Multiboot header for GRUB/QEMU

section .multiboot
align 4
MB_MAGIC      equ 0x1BADB002
MB_FLAGS      equ 0x0
MB_CHECKSUM   equ -(MB_MAGIC + MB_FLAGS)

dd MB_MAGIC
dd MB_FLAGS
dd MB_CHECKSUM

; Jump to C kernel
extern kernel_main
global _start
_start:
    call kernel_main
    cli
    hlt
