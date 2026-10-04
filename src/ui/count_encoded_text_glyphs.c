#include "m2c_prelude.h"
#include "window.h"
u8 CountEncodedTextGlyphs(u8 *text) asm("func_08098B58");

u8 CountEncodedTextGlyphs(u8 *text) {
    u8 *text_cursor;
    u8 character;
    u8 glyph_count;

    text_cursor = text;
    glyph_count = 0;
    while ((character = *text_cursor) != 0) {
        if ((u32) character <= 0x1FU) {
            switch (*text_cursor) {
            case WINDOW_TEXT_SET_COLOR:
                text_cursor += 1;
                goto block_11;
            case WINDOW_TEXT_SET_POSITION:
                text_cursor += 2;
                goto block_11;
            case WINDOW_TEXT_PLAYER_NAME:
                /* The native routine ignores the expanded player-name length. */
                CountEncodedTextGlyphs((u8 *)0x02021774);
            default:
block_11:
                text_cursor += 1;
                break;
            }
        } else {
            if ((u32) (u8) (character + 0x80) <= 0x1FU) {
                text_cursor += 2;
            } else {
                text_cursor += 1;
            }
            glyph_count += 1;
        }
    }
    return glyph_count;
}
