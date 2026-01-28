#pragma once
#include <types.h>

#define VGA_START       0xB8000
#define VGA_WIDTH       80
#define VGA_HEIGHT      25
#define VGA_EXTENT      (VGA_WIDTH * VGA_HEIGHT)

#define CURSOR_PORT_COMMAND 0x3D4
#define CURSOR_PORT_DATA    0x3D5

#define COLOR_BLACK         0x0
#define COLOR_BLUE          0x1
#define COLOR_GREEN         0x2
#define COLOR_CYAN          0x3
#define COLOR_RED           0x4
#define COLOR_MAGENTA       0x5
#define COLOR_BROWN         0x6
#define COLOR_LIGHT_GREY    0x7
#define COLOR_DARK_GREY     0x8
#define COLOR_LIGHT_BLUE    0x9
#define COLOR_LIGHT_GREEN   0xA
#define COLOR_LIGHT_CYAN    0xB
#define COLOR_LIGHT_RED     0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_LIGHT_BROWN   0xE
#define COLOR_WHITE         0xF

typedef struct {
    u8 character;
    u8 style;
} __attribute__((packed)) vga_char;

u16 get_cursor_pos();
void show_cursor();
void hide_cursor();
void clear_win(const u8 fg_color, const u8 bg_color);
void put_char(const char character, const u8 fg_color, const u8 bg_color);
void put_str(const char *string, const u8 fg_color, const u8 bg_color);
u8 vga_color(const u8 fg_color, const u8 bg_color);
void advance_cursor();
void set_cursor_pos(u8 x, u8 y);