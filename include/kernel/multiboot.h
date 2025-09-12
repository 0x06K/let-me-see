#ifndef KERNEL_MULTIBOOT_H
#define KERNEL_MULTIBOOT_H

#include "types.h"

// Multiboot magic numbers
#define MULTIBOOT_MAGIC         0x2BADB002
#define MULTIBOOT2_MAGIC        0x36d76289

// Multiboot flags
#define MULTIBOOT_FLAG_MEM      (1 << 0)
#define MULTIBOOT_FLAG_DEVICE   (1 << 1)
#define MULTIBOOT_FLAG_CMDLINE  (1 << 2)
#define MULTIBOOT_FLAG_MODS     (1 << 3)
#define MULTIBOOT_FLAG_MMAP     (1 << 6)

// Memory map entry types
#define MULTIBOOT_MMAP_AVAILABLE    1
#define MULTIBOOT_MMAP_RESERVED     2
#define MULTIBOOT_MMAP_ACPI_RECLAIM 3
#define MULTIBOOT_MMAP_ACPI_NVS     4
#define MULTIBOOT_MMAP_BADRAM       5

// Multiboot memory map entry
typedef struct {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed)) multiboot_mmap_entry_t;

// Multiboot info structure
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __attribute__((packed)) multiboot_info_t;

#endif // KERNEL_MULTIBOOT_H
