# Assemble boot.asm
nasm -f elf32 boot.asm -o boot.o

# Compile C kernel
gcc -m32 -ffreestanding -c kernel.c -o kernel.o

# Link everything
ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o

#run
qemu-system-i386 -kernel kernel.bin

