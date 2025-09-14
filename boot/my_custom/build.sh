#!/bin/bash

nasm -f bin boot.asm -o boot.bin
# Compile
gcc -ffreestanding -O2 -m32 -nostdlib -fno-pie -c kernel.c -o kernel.o

# Link
ld -m elf_i386 -T linker.ld --oformat binary -o kernel.bin kernel.o

objcopy -O binary kernel.elf kernel.bin
dd if=/dev/zero of=os_image.img bs=512 count=2880
dd if=boot.bin of=os_image.img bs=512 count=1 conv=notrunc
dd if=kernel.bin of=os_image.img bs=512 seek=1 conv=notrunc

echo "To run in QEMU:   qemu-system-x86_64 -drive format=raw,file=os_image.img"
echo "To run with debugging:  qemu-system-x86_64 -drive format=raw,file=os_image.img -monitor stdio -d int,cpu_reset"
