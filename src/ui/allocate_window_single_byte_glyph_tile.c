#include "m2c_prelude.h"
#include "window.h"

s32 FindFreeWindowTiles(s32, u16 *) asm("func_080979A4");
void ReserveWindowTile(u16) asm("func_0809795C");
s32 ExpandSingleByteGlyphTile(s32, s32, s32) asm("func_08097C24");
s32 DivideUnsigned32(s32, s32) asm("func_080ECF00");

void AllocateWindowSingleByteGlyphTile(s32 character_code, s32 text_color, u16 *tilemap_entry) asm("func_08097CEC");

void AllocateWindowSingleByteGlyphTile(s32 character_code, s32 text_color, u16 *tilemap_entry)
{
    u16 selected_tile;
    register u16 *output_entry asm("r6") = tilemap_entry;
    register u16 *palette_attribute asm("r5");
    register u32 normalized asm("r0");
    register u32 code asm("r4");
    u32 color;

    asm volatile("" : : "r"(output_entry));
    character_code <<= 24;
    normalized = (u32)character_code >> 24;
    asm volatile("" : : "r"(normalized));
    code = normalized;
    text_color <<= 24;
    color = (u32)text_color >> 24;
    if (code != FONT_SINGLE_BYTE_SPACE &&
        (u8)FindFreeWindowTiles(1, &selected_tile) != 0 &&
        (u8)ExpandSingleByteGlyphTile(code, color,
            *(s32 *)0x02021654 + ((u32)selected_tile << 5)) != 0) {
        u32 glyph_tile_entry;
        u32 color_palette;

        ReserveWindowTile(selected_tile);
        glyph_tile_entry = *(s32 *)0x02021670;
        asm volatile("" : "+r"(glyph_tile_entry));
        glyph_tile_entry += selected_tile;
        palette_attribute = (u16 *)0x02021668;
        color_palette = (u8)DivideUnsigned32(color, FONT_COLOR_VARIANTS_PER_PALETTE);
        color_palette <<= 12;
        color_palette += *palette_attribute;
        glyph_tile_entry |= color_palette;
        *output_entry = glyph_tile_entry;
    } else {
        *output_entry = (*(s32 *)0x02021664 + 1) | *(u16 *)0x02021668;
    }
}
