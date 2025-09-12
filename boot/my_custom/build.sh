#!/bin/bash

# Step 1: Compile kernel
gcc -ffreestanding -m32 -c kernel.c -o kernel.o 

# Step 2: Link kernel with linker script
ld -m elf_i386 -T kernel.ld -o kernel.bin kernel.o

# Step 3: Assemble bootloader
nasm -f bin boot.asm -o boot.bin 

# Pad kernel to multiple of 512 bytes
dd if=kernel.bin of=kernel_padded.bin bs=512 conv=sync

# Merge bootloader + kernel into disk image
cat boot.bin kernel_padded.bin > os-image.img

# Step 5: Run in QEMU
qemu-system-i386 -drive format=raw,file=os-image.img

