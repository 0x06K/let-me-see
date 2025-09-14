ENTRY(kernel_main)
SECTIONS
{
    . = 0x10000;
    kernel_main = .;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss COMMON) }
}