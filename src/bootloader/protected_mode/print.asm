%ifndef PROTECTED_PRINT_ASM
%define PROTECTED_PRINT_ASM

[bits 32]

; msg address stored in esi
protected_print:
    ;save all registers and set up edx to point to vga memory
    pushad
    mov edx, vga_start

    protected_print_loop:
        ; if char == '\0'
        cmp byte[esi], 0
        je  protected_print_done

        ; move character to al, style to ah
        mov al, byte[esi]
        mov ah, style_wb

        ; print character to vga memory location
        mov word[edx], ax

        ; increment counter registers
        add esi, 1
        add edx, 2

        jmp protected_print_loop

protected_print_done:
    ; restore the registers
    popad
    ret

%endif