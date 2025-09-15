ENTRY(kmain)
SECTIONS
{
    . = 0x10000;   /* start address of the kernel */

    .text : {
        *(.text.kmain)   
        *(.text*)         
    }

    .data : { *(.data*) }
    .bss  : { *(.bss COMMON) }
}
