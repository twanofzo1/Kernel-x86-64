#include <init.h>
#include <driver/vga.h>

void init(){
    // vga initialisation
    set_cursor_pos(0,0);
    clear_win(VGA_COLOR_BLACK,VGA_COLOR_BLACK);
}