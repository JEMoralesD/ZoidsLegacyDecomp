#include "m2c_prelude.h"
#include "window.h"

s32 ExpandSingleByteGlyphTile(u8, s32, s32) asm("func_08097C24");

s32 ExpandSingleByteTextTiles(u8 *text, s32 text_color, s32 glyph_tiles) asm("func_08097D80");

s32 ExpandSingleByteTextTiles(u8 *text, s32 text_color, s32 glyph_tiles) {
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
    last_result = ExpandSingleByteGlyphTile(*text_cursor, color, tile_address);
    tile_address += 0x20;
    text_cursor++;
test:
    {
        register s32 character_code asm("r1");
        asm volatile("ldrb %0, [%1]"
                     : "=r"(character_code)
                     : "r"(text_cursor));
        if (character_code != 0) {
            goto loop;
        }
    }
    return last_result;
}
