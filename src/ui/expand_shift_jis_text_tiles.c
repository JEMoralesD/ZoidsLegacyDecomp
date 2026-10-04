#include "m2c_prelude.h"
#include "window.h"

s32 ExpandShiftJisGlyphTiles(s32, s32, s32, s32) asm("func_08097A2C");

s32 ExpandShiftJisTextTiles(u8 *text, s32 text_color, s32 glyph_tiles) asm("func_08097BF4");

s32 ExpandShiftJisTextTiles(u8 *text, s32 text_color, s32 glyph_tiles) {
    u8 *text_cursor;
    s32 tile_address;
    s32 last_result;
    register s32 color asm("r6");

    last_result = (s32)text;
    text_cursor = text;
    tile_address = glyph_tiles;
    color = (u8)text_color;
    asm volatile("" : "+r"(color));
    goto test;
loop:
    {
        register s32 character_code asm("r0");
        register s32 high_byte asm("r1");
        high_byte = text_cursor[0] << 8;
        character_code = text_cursor[1] | high_byte;
        last_result = ExpandShiftJisGlyphTiles(character_code, color, tile_address, tile_address + 0x20);
    }
    tile_address += 0x40;
    text_cursor += 2;
test:
    {
        register s32 lead_byte asm("r1");
        asm volatile("ldrb %0, [%1]"
                     : "=r"(lead_byte)
                     : "r"(text_cursor));
        if (lead_byte != 0) {
            goto loop;
        }
    }
    return last_result;
}
