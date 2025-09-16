mov eax, [esp + 4]   ; first argument is at ESP+4 (return addr is at ESP)
    lgdt [eax]           ; load GDT with the pointer passed from C
    ; reload segment registers if needed...
    ret