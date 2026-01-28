#include <driver/vga.h>

int main(){
    set_cursor_pos(0,0);
    clear_win(COLOR_BLACK,COLOR_BLACK);
    const char *welcome_msg = "hello";
    put_str(welcome_msg,COLOR_GREEN,COLOR_BLACK);
    show_cursor();
    return 0;
}