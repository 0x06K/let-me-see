#!/bin/bash

# Directories
OUT_DIR="build/i686"
mkdir -p $OUT_DIR

# File names
BOOT=boot.asm
KERNEL_SRC=("src/kmain.c" "src/vga.c")   # All kernel C files
KERNEL_ELF=$OUT_DIR/kernel.elf
KERNEL_BIN=$OUT_DIR/kernel.bin
IMG=$OUT_DIR/kernel.img
OBJ_FILES=()

# -------------------------------
# Assemble bootloader (512 bytes)
# -------------------------------
nasm -f bin $BOOT -o $OUT_DIR/boot.bin
echo "[*] Bootloader assembled: boot.bin"

# -------------------------------
# Compile kernel C files
# -------------------------------
for SRC in "${KERNEL_SRC[@]}"; do
    OBJ="$OUT_DIR/$(basename $SRC .c).o"
    i686-linux-gnu-gcc -ffreestanding -m32 -ffunction-sections -c $SRC -o $OBJ -Iinclude
    OBJ_FILES+=("$OBJ")
    echo "[*] Compiled $SRC -> $OBJ"
done

# -------------------------------
# Link kernel to ELF
# -------------------------------
i686-linux-gnu-ld -m elf_i386 -T linker.ld -nostdlib -o $KERNEL_ELF "${OBJ_FILES[@]}"
echo "[*] Linked kernel ELF: $KERNEL_ELF"

# -------------------------------
# Convert ELF to raw binary
# -------------------------------
i686-linux-gnu-objcopy -O binary $KERNEL_ELF $KERNEL_BIN
echo "[*] Converted ELF -> binary: $KERNEL_BIN"

# -------------------------------
# Create 2 MB blank bootable image
# -------------------------------
dd if=/dev/zero of=$IMG bs=512 count=4096
echo "[*] Created blank disk image: $IMG"

# Write bootloader first (sector 0)
dd if=$OUT_DIR/boot.bin of=$IMG conv=notrunc
# Write kernel starting at sector 1
dd if=$KERNEL_BIN of=$IMG bs=512 seek=1 conv=notrunc
echo "[*] Bootloader + kernel written to image"

# -------------------------------
# Print kernel size in sectors
# -------------------------------
KERNEL_SECTORS=$(( ( $(stat -c%s $KERNEL_BIN) + 511 ) / 512 ))
echo "[*] Kernel size: $KERNEL_SECTORS sectors"

# -------------------------------
# Boot in QEMU
# -------------------------------
echo "[*] Launching QEMU..."
qemu-system-i386 -drive format=raw,file=$IMG --no-shutdown --no-reboot
