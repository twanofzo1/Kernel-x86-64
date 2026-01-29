%ifndef BIOS_PRINT_ASM
%define BIOS_PRINT_ASM
[bits 16]

; print a null-terminated string
; input : BX -> pointer to null-terminated string
bios_print:
    ; save registeers
    push ax
    push bx

    ; enable print mode
    mov ah, 0x0E


    bios_print_loop:
        ; null terminator
        cmp byte[bx], `\0`
        je bios_print_end
        ; print the char
        mov al, byte[bx]
        int 0x10
        ; increment pointer and loop
        inc bx
        jmp bios_print_loop


bios_print_end:
    ; restore registers and return
    pop bx
    pop ax
    ret

%endif 