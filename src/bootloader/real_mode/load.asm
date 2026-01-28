%ifndef LOAD_BIOS_ASM
%define LOAD_BIOS_ASM
[bits 16]


; constants
read_bios_utility_code equ 0x02
bios_disk_read_code equ 0x13
error_msg:   db `\r\nERROR Loading Sectors. Code: `, 0
success_msg: db `\r\nAdditional Sectors Loaded Successfully!\r\n`, 0


; sector start point in bx
; number of sectors to read in cx
; destination address in dx

bios_load:
    ; Save the registers
    push ax
    push bx
    push cx
    push dx

    ; save the number of sectors to load for later
    push cx

    ; Set up BIOS disk read parameters
    mov ah, read_bios_utility_code
    ; the number of sectors to read must be in al, but our function takes it in cl
    mov al, cl
    ; the sector to read from must be in cl, but our function takes it in bl
    mov cl, bl
    ; the destination address must be in bx, but our function takes it in dx
    mov bx, dx
    mov ch, 0x00        ; Cylinder goes in ch
    mov dh, 0x00        ; Cylinder head goes in dh

    ; store boot drive in dl
    mov dl, byte[boot_drive]

    ; execute the bios disk read
    int bios_disk_read_code

    ; if the carry flag is set, there was an error so jump to error handler
    jc bios_disk_error

    ; compare the number of sectors read to the number we wanted to read
    ; if not equal, jump to error handler
    pop bx
    cmp al, bl
    jne bios_disk_error

    ; print success message
    mov bx, success_msg
    call bios_print

    ; restore registers and return
    pop dx
    pop cx
    pop bx
    pop ax
    ret


bios_disk_error:
    ; print error message
    mov bx, error_msg
    call bios_print

    ; shift to get only the error code in ah and print it in hex
    shr ax, 8
    mov bx, ax
    call bios_print_hex

    ; hang forever
    jmp $



%endif