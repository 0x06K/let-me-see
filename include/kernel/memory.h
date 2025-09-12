#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include "types.h"
#include "multiboot.h"

// Memory layout constants
#define KERNEL_VIRTUAL_BASE     0xFFFF800000000000ULL
#define KERNEL_HEAP_START       0xFFFF800000400000ULL
#define USER_SPACE_END          0x00007FFFFFFFFFFFULL

// Page flags
#define PAGE_PRESENT    (1 << 0)
#define PAGE_WRITABLE   (1 << 1)
#define PAGE_USER       (1 << 2)
#define PAGE_WRITETHROUGH (1 << 3)
#define PAGE_CACHE_DISABLE (1 << 4)
#define PAGE_ACCESSED   (1 << 5)
#define PAGE_DIRTY      (1 << 6)
#define PAGE_HUGE       (1 << 7)
#define PAGE_GLOBAL     (1 << 8)
#define PAGE_NO_EXECUTE (1ULL << 63)

// Physical Memory Manager
typedef struct {
    uint64_t total_memory;
    uint64_t free_memory;
    uint64_t used_memory;
    uint32_t *bitmap;
    uint64_t bitmap_size;
    uint64_t last_frame;
} pmm_info_t;

// Virtual Memory Manager
typedef struct {
    uint64_t *pml4;
    uint64_t cr3_value;
} vmm_context_t;

// Memory region structure
typedef struct memory_region {
    uint64_t start;
    uint64_t end;
    uint32_t type;
    struct memory_region *next;
} memory_region_t;

// ===== PHYSICAL MEMORY MANAGER =====

// Initialize physical memory manager
void pmm_init(multiboot_info_t *mbi);

// Allocate a physical frame (4KB)
void *pmm_alloc_frame(void);

// Allocate multiple contiguous frames
void *pmm_alloc_frames(size_t count);

// Free a physical frame
void pmm_free_frame(void *frame);

// Free multiple frames
void pmm_free_frames(void *frame, size_t count);

// Get memory statistics
pmm_info_t pmm_get_info(void);

// Check if frame is allocated
bool pmm_is_frame_allocated(void *frame);

// ===== VIRTUAL MEMORY MANAGER =====

// Initialize virtual memory manager
void vmm_init(void);

// Create new virtual memory context
vmm_context_t *vmm_create_context(void);

// Switch virtual memory context
void vmm_switch_context(vmm_context_t *context);

// Map virtual address to physical address
bool vmm_map_page(uint64_t virtual_addr, uint64_t physical_addr, uint32_t flags);

// Unmap virtual address
void vmm_unmap_page(uint64_t virtual_addr);

// Get physical address from virtual
uint64_t vmm_get_physical_addr(uint64_t virtual_addr);

// Map multiple pages
bool vmm_map_pages(uint64_t virtual_addr, uint64_t physical_addr, size_t count, uint32_t flags);

// ===== KERNEL HEAP =====

// Initialize kernel heap
void kheap_init(void);

// Allocate kernel memory
void *kmalloc(size_t size);

// Allocate aligned kernel memory
void *kmalloc_aligned(size_t size, size_t alignment);

// Allocate and zero kernel memory
void *kzalloc(size_t size);

// Reallocate kernel memory
void *krealloc(void *ptr, size_t new_size);

// Free kernel memory
void kfree(void *ptr);

// Get heap statistics
typedef struct {
    size_t total_size;
    size_t used_size;
    size_t free_size;
    size_t blocks_allocated;
    size_t blocks_free;
} heap_stats_t;

heap_stats_t kheap_get_stats(void);

// ===== UTILITY FUNCTIONS =====

// Convert virtual address to physical
#define VIRT_TO_PHYS(addr) ((uint64_t)(addr) - KERNEL_VIRTUAL_BASE)

// Convert physical address to virtual
#define PHYS_TO_VIRT(addr) ((void*)((uint64_t)(addr) + KERNEL_VIRTUAL_BASE))

// Align address up to page boundary
#define PAGE_ALIGN_UP(addr) (((uint64_t)(addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

// Align address down to page boundary
#define PAGE_ALIGN_DOWN(addr) ((uint64_t)(addr) & ~(PAGE_SIZE - 1))

// Get page frame number from address
#define ADDR_TO_FRAME(addr) ((uint64_t)(addr) >> PAGE_SHIFT)

// Get address from page frame number
#define FRAME_TO_ADDR(frame) ((uint64_t)(frame) << PAGE_SHIFT)

// Check if address is page aligned
#define IS_PAGE_ALIGNED(addr) (((uint64_t)(addr) & (PAGE_SIZE - 1)) == 0)

#endif // KERNEL_MEMORY_H
