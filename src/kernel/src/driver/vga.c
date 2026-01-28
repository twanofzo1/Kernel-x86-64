#include <driver/vga.h>
#include <cpu/ports.h>
#include <bitwise.h>

#define CURSOR_LOCATION_HIGH_BYTE_ADDR 0x0F      // High byte of cursor position 
#define CURSOR_LOCATION_LOW_BYTE_ADDR 0x0E       // Low byte of cursor position 
#define CURSOR_START_ADDR 0x0A                   // Cursor shape start scanline + disable bit 
#define CURSOR_END_ADDR 0x0B                     // Cursor shape end scanline 
#define CURSOR_DISABLE_BIT_NR 5

u16 get_cursor_pos(){
    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_HIGH_BYTE_ADDR);
    u8 high = byte_in(CURSOR_PORT_DATA);

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_LOW_BYTE_ADDR);
    u8 low = byte_in(CURSOR_PORT_DATA);

    return MAKE_16(low,high);
}

void show_cursor(){
    byte_out(CURSOR_PORT_COMMAND, CURSOR_START_ADDR);
    u8 current = byte_in(CURSOR_PORT_DATA);
    u8 shown = SET_BIT_8(current, CURSOR_DISABLE_BIT_NR, false); // Set disable bit
    byte_out(CURSOR_PORT_DATA, shown); 
}

void hide_cursor(){
    byte_out(CURSOR_PORT_COMMAND, CURSOR_START_ADDR);
    u8 current = byte_in(CURSOR_PORT_DATA);
    u8 hidden = SET_BIT_8(current, CURSOR_DISABLE_BIT_NR, true); // Set disable bit
    byte_out(CURSOR_PORT_DATA, hidden);   
}




volatile vga_char *TEXT_AREA = (vga_char*) VGA_START;

void clear_win(const u8 fg_color, const u8 bg_color){
    vga_char clear_char = {
        .character=' ',
        .style=COLOR_WHITE
    };

    for(unsigned int i = 0; i < VGA_EXTENT; i++){
        TEXT_AREA[i] = clear_char;
    }
}

void put_char(const char character, const u8 fg_color, const u8 bg_color){
    vga_char c = {
        .character=character,
        .style=vga_color(fg_color,bg_color)
    };
    u16 pos = get_cursor_pos();
    TEXT_AREA[pos] = c;
}

void put_str(const char *string, const u8 fg_color, const u8 bg_color){
    while (*string != '\0')
    {
        put_char(*string++, fg_color, bg_color);
        advance_cursor(); 
    }
    
}


u8 vga_color(const u8 fg_color, const u8 bg_color){
    return fg_color | (bg_color << 4);
}




void advance_cursor(){
    u16 pos = get_cursor_pos();
    pos++;

    if (pos >= VGA_EXTENT){
        pos = VGA_EXTENT - 1;
    }

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_HIGH_BYTE_ADDR);
    byte_out(CURSOR_PORT_DATA, GET_HIGHER_8(pos));

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_LOW_BYTE_ADDR);
    byte_out(CURSOR_PORT_DATA, GET_LOWER_8(pos));
}


void set_cursor_pos(u8 x, u8 y){
    u16 pos = x + ((u16) VGA_WIDTH * y);

    if (pos >= VGA_EXTENT){
        pos = VGA_EXTENT - 1;
    }

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_HIGH_BYTE_ADDR);
    byte_out(CURSOR_PORT_DATA, GET_HIGHER_8(pos));

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_LOW_BYTE_ADDR);
    byte_out(CURSOR_PORT_DATA, GET_LOWER_8(pos));
}
