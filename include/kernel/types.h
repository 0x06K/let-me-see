#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

// Basic integer types
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

// Size and pointer types
typedef uint64_t size_t;
typedef int64_t  ssize_t;
typedef uint64_t uintptr_t;
typedef int64_t  intptr_t;

// Boolean type
typedef enum {
    false = 0,
    true = 1
} bool;

// NULL pointer
#ifndef NULL
#define NULL ((void*)0)
#endif

// Common constants
#define PAGE_SIZE    4096
#define PAGE_SHIFT   12
#define KERNEL_BASE  0xFFFF800000000000ULL

#endif // KERNEL_TYPES_H
