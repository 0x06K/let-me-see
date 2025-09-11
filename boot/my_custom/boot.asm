; Simple Bootloader Template (512 bytes)

org 0x7C00             ; BIOS loads MBR at memory address 0x7C00

start:
    mov si, msg        ; Load address of message
    call print_string  ; Print message
    halt               ; Infinite loop to halt

; Print string routine
print_string:
    mov ah, 0x0E       ; BIOS teletype function
.next_char:
    lodsb              ; Load byte at [SI] into AL and increment SI
    cmp al, 0
    je .done
    int 0x10           ; BIOS interrupt to print character in AL
    jmp .next_char
.done:
    ret

; Message to print
msg db 'Hello, bootloader!', 0

; Fill remaining bytes with zeros
times 510 - ($ - $$) db 0

; Boot signature (0x55AA) at last two bytes
dw 0xAA55
