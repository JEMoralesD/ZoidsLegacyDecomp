#include "m2c_prelude.h"
#include "window.h"

s32 FindFreeWindowTiles(s32, u16 *) asm("func_080979A4");
void ReserveWindowTile(u16) asm("func_0809795C");
s32 ExpandShiftJisGlyphTiles(s32, s32, s32, s32) asm("func_08097A2C");
s32 DivideUnsigned32(s32, s32) asm("func_080ECF00");

void AllocateWindowShiftJisGlyphTiles(s32 character_code, s32 text_color, u16 *tilemap_entries) asm("func_08097B2C");

void AllocateWindowShiftJisGlyphTiles(s32 character_code, s32 text_color, u16 *tilemap_entries)
{
    u16 selected_tiles[2];
    u16 *output_entries = tilemap_entries;
    register u32 normalized asm("r0");
    register u32 code asm("r4");
    register u32 color asm("r8");

    asm volatile("" : : "r"(output_entries));
    character_code <<= 16;
    normalized = (u32)character_code >> 16;
    asm volatile("" : : "r"(normalized));
    code = normalized;
    text_color <<= 24;
    color = (u32)text_color >> 24;
    if (code != FONT_SHIFT_JIS_SPACE &&
        (u8)FindFreeWindowTiles(2, selected_tiles) != 0 &&
        (u8)ExpandShiftJisGlyphTiles(code, color,
            *(s32 *)0x02021654 + ((u32)selected_tiles[0] << 5),
            *(s32 *)0x02021654 + ((u32)selected_tiles[1] << 5)) != 0) {
        register s32 *text_tile_offset asm("r6");
        register u16 *palette_attribute asm("r5");
        register u32 top_tile_entry asm("r4");
        register u32 bottom_tile_entry asm("r2");
        register u32 color_palette asm("r0");
        register u32 palette_bits asm("r1");

        ReserveWindowTile(selected_tiles[0]);
        ReserveWindowTile(selected_tiles[1]);
        text_tile_offset = (s32 *)0x02021670;
        top_tile_entry = *text_tile_offset;
        top_tile_entry += selected_tiles[0];
        palette_attribute = (u16 *)0x02021668;
        color_palette = (u8)DivideUnsigned32(color, FONT_COLOR_VARIANTS_PER_PALETTE);
        color_palette <<= 12;
        palette_bits = *palette_attribute;
        palette_bits += color_palette;
        top_tile_entry |= palette_bits;
        output_entries[0] = top_tile_entry;

        bottom_tile_entry = *text_tile_offset;
        bottom_tile_entry += selected_tiles[1];
        palette_bits = *palette_attribute;
        palette_bits += color_palette;
        bottom_tile_entry |= palette_bits;
        output_entries[1] = bottom_tile_entry;
    } else {
        register s32 *frame_tile_offset asm("r3") = (s32 *)0x02021664;
        register u16 *palette_attribute asm("r2");
        register u32 top_tile_entry asm("r0");
        register u32 fallback_palette asm("r1");

        top_tile_entry = *frame_tile_offset;
        top_tile_entry += 1;
        palette_attribute = (u16 *)0x02021668;
        fallback_palette = *palette_attribute;
        top_tile_entry |= fallback_palette;
        output_entries[0] = top_tile_entry;
        top_tile_entry = *frame_tile_offset;
        top_tile_entry += 1;
        fallback_palette = *palette_attribute;
        top_tile_entry |= fallback_palette;
        output_entries[1] = top_tile_entry;
    }
}
