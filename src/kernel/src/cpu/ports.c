#include <cpu/ports.h>

u8 byte_in(u16 port){
    // We're using inline assembly to read from the port

    // "=a" (result) means to put value of rax in result
    // "d" (port) means put value of port in dx

    // asm instructions use the form "command" : "output" : "input"
    // Use the "in" assembly command to read from a port
    u8 result;
    __asm__ volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}


void byte_out(u16 port, u8 data){
    // Use the "out" command to write to a port
    __asm__ volatile ("out %%al, %%dx" : : "a" (data), "d" (port));
}