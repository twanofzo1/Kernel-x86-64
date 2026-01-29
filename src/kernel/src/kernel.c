#include <init.h>
#include <io.h>
#include <driver/vga.h>
int main(){
    init();
    printf_color("hello\ntest",VGA_COLOR_BLUE);
    printf_color("hello\ntest",VGA_COLOR_RED);

    show_cursor();
    return 0;
}