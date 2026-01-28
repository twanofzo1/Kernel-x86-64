%ifndef PRINT_HEX_BIOS_ASM
%define PRINT_HEX_BIOS_ASM

[bits 16]
; print a 16-bit value in hexadecimal format
; input : BX -> value to print in hex
bios_print_hex:
    ; save registers
    push ax
    push bx
    push cx

    ; enable print mode
    mov ah, 0x0E

    ; print : 0x
    mov al, '0'
    int 0x10
    mov al, 'x'
    int 0x10

    ; 16 bits / 4 bits per hex digit = 4 hex digits
    mov cx, 4

    bios_print_hex_loop:
        ; if cx == 0 (looped 4 times)
        cmp cx, 0
        je bios_print_hex_end

        ; save bx 
        push bx

        ; isolate top 4 bits
        shr bx, 12

        ; if bx >= hex'A' (10)
        cmp bx, 10
        jge bios_print_hex_alpha

        bios_print_hex_digit:
            ; print the digit by adding offset to char '0'
            mov al, '0'
            add al, bl
            jmp bios_print_hex_loop_end

        bios_print_hex_alpha:
            ; calculate offset
            sub bl, 10
            ; offset + char 'A'
            mov al, 'A'
            add al, bl

        bios_print_hex_loop_end:
        ; print the character
        int 0x10

        ; get the rest of bx and shift it left by 4 bits to get next hex digit
        pop bx
        shl bx, 4

        ; decrement cx and loop
        dec cx
        jmp bios_print_hex_loop

bios_print_hex_end:
    ; restore registers and return
    pop cx
    pop bx
    pop ax
    ret

%endif ; PRINT_HEX_BIOS_ASM