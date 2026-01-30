#include <driver/vga.h>
#include <cpu/ports.h>
#include <bitwise.h>

#define CURSOR_LOCATION_HIGH_BYTE_ADDR 0x0F      // High byte of cursor position 
#define CURSOR_LOCATION_LOW_BYTE_ADDR 0x0E       // Low byte of cursor position 
#define CURSOR_START_ADDR 0x0A                   // Cursor shape start scanline + disable bit 
#define CURSOR_END_ADDR 0x0B                     // Cursor shape end scanline 
#define CURSOR_DISABLE_BIT_NR 5


volatile vga_char* TEXT_AREA = (vga_char*) VGA_START;


#define WHITE_SPACE ((vga_char){' ',vga_color(VGA_COLOR_BLACK,VGA_COLOR_BLACK)});




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


void clear_win(){
    for(unsigned int i = 0; i < VGA_EXTENT; i++){
        TEXT_AREA[i] = WHITE_SPACE;
    }
}

void scroll_vga(){
    // move all 1y up
    for (int i = VGA_WIDTH; i < VGA_EXTENT; i++){
        TEXT_AREA[i - VGA_WIDTH] = TEXT_AREA[i];
    }
    // clear last line
    for (int i = VGA_EXTENT - VGA_WIDTH; i < VGA_EXTENT; i++){
        TEXT_AREA[i] = WHITE_SPACE;
    }
    set_cursor_pos(VGA_EXTENT - VGA_WIDTH);
}


void put_char(const char character, const u8 fg_color, const u8 bg_color){
    u16 pos = get_cursor_pos();
    if (pos + 1 == VGA_EXTENT){
        scroll_vga();
    }
    

    if (character == '\n'){
        // just print whitespaces till next line
        int offset = VGA_WIDTH - (pos % VGA_WIDTH);
        for (int i = 0; i < offset; i++){
            put_char(' ',VGA_COLOR_BLACK,VGA_COLOR_BLACK);
        }
        return;
    }

    if (character == '\t'){
        // print whitespaces to fill a TAB_WIDTH sized grid
        int offset = TAB_WIDTH - (pos % TAB_WIDTH);
        for (int i = 0; i < offset; i++){
            put_char(' ',VGA_COLOR_BLACK,VGA_COLOR_BLACK);
        }
        return;
    }

    if (character == '\r'){
        // return to the begin of the line
        set_cursor_pos(pos - pos%VGA_WIDTH);
        return;
    }

    if (character == '\b'){
        if (pos-1 >=0){
            return;
        }
        set_cursor_pos(pos - 1);
        return;
    }

    vga_char c = {
        .character=character,
        .style=vga_color(fg_color,bg_color)
    };
    TEXT_AREA[pos] = c;
    advance_cursor(); 
}

void put_str(const char *string, const u8 fg_color, const u8 bg_color){
    while (*string != '\0')
    {
        put_char(*string++, fg_color, bg_color);
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


void set_cursor_pos(u16 pos){
    if (pos >= VGA_EXTENT){
        pos = VGA_EXTENT - 1;
    }

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_HIGH_BYTE_ADDR);
    byte_out(CURSOR_PORT_DATA, GET_HIGHER_8(pos));

    byte_out(CURSOR_PORT_COMMAND, CURSOR_LOCATION_LOW_BYTE_ADDR);
    byte_out(CURSOR_PORT_DATA, GET_LOWER_8(pos));
}
