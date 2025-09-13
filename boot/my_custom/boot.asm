; bootloader.asm - Real Mode Bootloader for 32-bit kernel

[bits 16]
org 0x7C00           ; BIOS loads bootloader here

start: 
    cli                ; Disable interrupts during setup
    
    ; Setup stack somewhere safe (above bootloader)
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    sti
    
    ; Print message
    mov si, msg
    call print_string
    
    ; -----------------------------
    ; Load kernel from disk
    ; DISK - READ SECTOR(S) INTO MEMORY
    ; AH = 02h
    ; AL = number of sectors to read (must be nonzero)
    ; CH = low eight bits of cylinder number
    ; CL = sector number 1-63 (bits 0-5)
    ; high two bits of cylinder (bits 6-7, hard disk only)
    ; DH = head number
    ; DL = drive number (bit 7 set for hard disk)
    ; ES:BX -> data buffer

    
    mov ah, 0x02        ; AH = 02h function number
    mov al, 10          ; AL = number of sectors to read (must be nonzero)
    mov ch, 0           ; CH = low eight bits of cylinder number
    mov cl, 2           ; CL = sector number 1-63 (bits 0-5)
    mov dh, 0           ; DH = head number
    mov dl, 0x80        ; DL = drive number (bit 7 set for hard disk)
    mov bx, 0x1000      ; setting up the data buffer last argument
    mov es, bx          ; ES:BX 
    mov bx, 0x0000      ; ES:BX

    int 0x13            ; call BIOS
                        ; Return:
                        ; CF set on error
                        ; if AH = 11h (corrected ECC error), AL = burst length
                        ; CF clear if successful
                        ; AH = status (see #00234)
                        ; AL = number of sectors transferred (only valid if CF set for someBIOSes)
    
    jc disk_error       ; jump if carry flag set (error)
    
    cmp al, 10          ; Verify we read the correct number of sectors
    jne disk_error
    
    ; Kernel is loaded at 0x1000:0000 (physical 0x10000)
    mov si, success_msg
    call print_string

    ; Next step: switch to Protected Mode and jump to kernel    
gdt_start:
    ; Entry 0: Null Descriptor (mandatory - all zeros)
    dw 0x0000    ; Limit (bits 0-15)
    dw 0x0000    ; Base (bits 0-15) 
    db 0x00      ; Base (bits 16-23)
    db 0x00      ; Access byte
    db 0x00      ; Flags + Limit (bits 16-19)
    db 0x00      ; Base (bits 24-31)

    ; Entry 1: Kernel Code Segment (Ring 0)
    dw 0xFFFF    ; Limit (bits 0-15) = 65535
    dw 0x0000    ; Base (bits 0-15) = 0
    db 0x00      ; Base (bits 16-23) = 0
    db 0x9A      ; Access: Present, Ring 0, Code, Executable, Readable
    db 0xCF      ; Flags: 4KB granularity, 32-bit + Limit bits 16-19 = F
    db 0x00      ; Base (bits 24-31) = 0

    ; Entry 2: Kernel Data Segment (Ring 0) 
    dw 0xFFFF    ; Limit (bits 0-15) = 65535
    dw 0x0000    ; Base (bits 0-15) = 0
    db 0x00      ; Base (bits 16-23) = 0
    db 0x92      ; Access: Present, Ring 0, Data, Writable
    db 0xCF      ; Flags: 4KB granularity, 32-bit + Limit bits 16-19 = F
    db 0x00      ; Base (bits 24-31) = 0

    ; Entry 3: User Code Segment (Ring 3)
    dw 0xFFFF    ; Limit (bits 0-15) = 65535
    dw 0x0000    ; Base (bits 0-15) = 0  
    db 0x00      ; Base (bits 16-23) = 0
    db 0xFA      ; Access: Present, Ring 3, Code, Executable, Readable
    db 0xCF      ; Flags: 4KB granularity, 32-bit + Limit bits 16-19 = F
    db 0x00      ; Base (bits 24-31) = 0

    ; Entry 4: User Data Segment (Ring 3)
    dw 0xFFFF    ; Limit (bits 0-15) = 65535
    dw 0x0000    ; Base (bits 0-15) = 0
    db 0x00      ; Base (bits 16-23) = 0  
    db 0xF2      ; Access: Present, Ring 3, Data, Writable
    db 0xCF      ; Flags: 4KB granularity, 32-bit + Limit bits 16-19 = F
    db 0x00      ; Base (bits 24-31) = 0

gdt_end:

; GDT Descriptor Pointer
gdt_descriptor:
    dw gdt_end - gdt_start - 1    ; Size of GDT (minus 1)
    dd gdt_start                  ; 32-bit address of GDT


    cli                      ; Clear interrupt flag - disable interrupts
    lgdt [gdt_descriptor]    ; CPU reads: "GDT is 23 bytes, starts at 0x1006"
    mov ax, cr0            ; Get current CR0 value
    or ax, 1               ; Set PE bit (bit 0)
    mov cr0, ax            ; Write back to CR0
    jmp 0x08:protected_mode_start   ; 0x08 = kernel code selector

[bits 32]               ; Tell assembler we're now in 32-bit mode
protected_mode_start:
    mov ax, 0x10         ; 0x10 = kernel data selector
    mov ds, ax           ; Set data segment
    mov es, ax           ; Set extra segment  
    mov fs, ax           ; Set FS segment
    mov gs, ax           ; Set GS segment
    mov ss, ax           ; Set stack segment
    
    ; Set up stack pointer
    mov esp, 0x90000     ; Set stack to some safe location
    
    ; You're now in 32-bit protected mode!
    ; Your 32-bit kernel code goes here...

disk_error:
    mov si, err_msg
    call print_string
    jmp hang

hang:
    hlt                  ; halt CPU to save power
    jmp hang


; Print string routine (BIOS teletype)
print_string:
    pusha               ; save all registers
    mov ah, 0x0E        ; teletype function
    mov bh, 0           ; page number
    mov bl, 0x07        ; text attribute (white on black)

.next_char:
    lodsb               ; load byte from SI into AL, increment SI
    cmp al, 0           ; check for null terminator
    je .done
    int 0x10            ; BIOS video interrupt
    jmp .next_char

.done:
    popa                ; restore all registers
    ret

; -----------------------------
; Messages
msg db 'Bootloader loaded, loading kernel...', 0x0D, 0x0A, 0
success_msg db 'Kernel loaded successfully!', 0x0D, 0x0A, 0
err_msg db 'Disk read error!', 0x0D, 0x0A, 0

; -----------------------------
; Fill remaining bytes with zeros and add boot signature
times 510 - ($ - $$) db 0
dw 0xAA55               ; Boot signature