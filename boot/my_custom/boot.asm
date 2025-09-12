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
    ; Assumes kernel starts at sector 2 (LBA 2)
    ; Kernel fits in 4 sectors (adjust as needed)
    mov bx, 0x0000       ; offset in ES
    mov es, bx           ; clear ES first
    mov ax, 0x1000       ; segment -> physical 0x10000
    mov es, ax
    
    mov cx, 4            ; number of sectors to read
    mov ah, 0x02         ; BIOS read sector function
    mov al, 4            ; read 4 sectors at once (more efficient)
    mov ch, 0            ; cylinder 0
    mov cl, 2            ; starting sector number (sector 2)
    mov dh, 0            ; head 0
    mov dl, 0x80         ; first hard disk (0x80 = first HDD, 0x00 = first floppy)
    
    int 0x13             ; call BIOS
    jc disk_error        ; jump if carry flag set (error)
    
    ; Verify we read the correct number of sectors
    cmp al, 4
    jne disk_error
    
    ; Print success message
    mov si, success_msg
    call print_string
    
    ; Kernel is now loaded at 0x1000:0000 (physical 0x10000)
    ; Next step: switch to Protected Mode and jump to kernel
    
    ; For now, just hang
    jmp hang

disk_error:
    mov si, err_msg
    call print_string
    jmp hang

hang:
    hlt                  ; halt CPU to save power
    jmp hang

; -----------------------------
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