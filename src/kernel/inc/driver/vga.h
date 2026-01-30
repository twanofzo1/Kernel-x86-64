#pragma once
#include <types.h>

#define VGA_START       0xB8000
#define VGA_WIDTH       80
#define VGA_HEIGHT      25
#define VGA_EXTENT      (VGA_WIDTH * VGA_HEIGHT)

#define CURSOR_PORT_COMMAND 0x3D4
#define CURSOR_PORT_DATA    0x3D5
#define TAB_WIDTH           4

#define VGA_COLOR_BLACK         0x0
#define VGA_COLOR_BLUE          0x1
#define VGA_COLOR_GREEN         0x2
#define VGA_COLOR_CYAN          0x3
#define VGA_COLOR_RED           0x4
#define VGA_COLOR_MAGENTA       0x5
#define VGA_COLOR_BROWN         0x6
#define VGA_COLOR_LIGHT_GREY    0x7
#define VGA_COLOR_DARK_GREY     0x8
#define VGA_COLOR_LIGHT_BLUE    0x9
#define VGA_COLOR_LIGHT_GREEN   0xA
#define VGA_COLOR_LIGHT_CYAN    0xB
#define VGA_COLOR_LIGHT_RED     0xC
#define VGA_COLOR_LIGHT_MAGENTA 0xD
#define VGA_COLOR_LIGHT_BROWN   0xE
#define VGA_COLOR_WHITE         0xF

typedef struct {
    u8 character;
    u8 style;
} __attribute__((packed)) vga_char;
_Static_assert(sizeof(vga_char) == 2, "VGA entry must be 2 bytes");


u16 get_cursor_pos();
void show_cursor();
void hide_cursor();
void clear_win();
void put_char(const char character, const u8 fg_color, const u8 bg_color);
void put_str(const char *string, const u8 fg_color, const u8 bg_color);
u8 vga_color(const u8 fg_color, const u8 bg_color);
void advance_cursor();
void set_cursor_pos(u16 pos);

