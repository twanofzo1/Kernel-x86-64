#include <io.h>

int main(){
    set_cursor_pos(0,0);
    clear_win(VGA_COLOR_BLACK,VGA_COLOR_BLACK);

    printf("hello im %i years old",21);

    show_cursor();
    return 0;
}