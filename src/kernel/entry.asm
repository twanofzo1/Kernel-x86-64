[bits 64]
[extern main]

section .text
global _start
_start:
    and rsp, -16
    sub rsp, 8        ; <-- make rsp % 16 == 8

    call main
    jmp $

hang:
    hlt
    jmp hang