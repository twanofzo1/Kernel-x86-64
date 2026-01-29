#include <io.h>
#include <driver/vga.h>



void printf(const char* str, ...){
    char buffer[255];
    va_list args;
    va_start(args, str);
    vformat_str(buffer, str, args);
    va_end(args);
    put_str(buffer, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

void printf_color(const char* str, u8 vga_color, ...){
    char buffer[255];
    va_list args;
    va_start(args, vga_color);
    vformat_str(buffer, str, args);
    va_end(args);
    put_str(buffer, vga_color, VGA_COLOR_BLACK);
}