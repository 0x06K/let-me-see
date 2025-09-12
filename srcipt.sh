#!/bin/bash

# Memory Management Files Generator
# Creates only the essential MM files with proper directory structure

echo "Creating Memory Management files..."

# Create directories
mkdir -p kernel/mm/physical
mkdir -p kernel/mm/virtual
mkdir -p kernel/mm/heap
mkdir -p include/kernel

# Phase 1: Physical Memory Manager
touch kernel/mm/physical/pmm.c
touch kernel/mm/physical/frame.c

# Phase 2: Virtual Memory Manager
touch kernel/mm/virtual/vmm.c
touch kernel/mm/virtual/paging.c
touch kernel/mm/virtual/page_table.c

# Phase 3: Kernel Heap
touch kernel/mm/heap/kheap.c
touch kernel/mm/heap/malloc.c

# Required Headers
touch include/kernel/memory.h
touch include/kernel/types.h
touch include/kernel/multiboot.h

echo "Memory Management files created successfully!"
echo ""
echo "Files created:"
echo "📁 Physical Memory Manager:"
echo "   - kernel/mm/physical/pmm.c"
echo "   - kernel/mm/physical/frame.c"
echo ""
echo "📁 Virtual Memory Manager:"
echo "   - kernel/mm/virtual/vmm.c"
echo "   - kernel/mm/virtual/paging.c"
echo "   - kernel/mm/virtual/page_table.c"
echo ""
echo "📁 Kernel Heap:"
echo "   - kernel/mm/heap/kheap.c"
echo "   - kernel/mm/heap/malloc.c"
echo ""
echo "📁 Headers:"
echo "   - include/kernel/memory.h"
echo "   - include/kernel/types.h"
echo "   - include/kernel/multiboot.h"
echo ""
echo "Total: $(find kernel/mm include/kernel -name '*.c' -o -name '*.h' | wc -l) files"
echo ""
echo "🎯 Start with: kernel/mm/physical/pmm.c and include/kernel/memory.h"
