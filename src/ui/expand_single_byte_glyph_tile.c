#include "m2c_prelude.h"
#include "window.h"

s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");


s32 ExpandSingleByteGlyphTile(s32 character_code, s32 text_color, s16 *glyph_tile) asm("func_08097C24");

s32 ExpandSingleByteGlyphTile(s32 character_code, s32 text_color, s16 *glyph_tile)
{
    register u32 code asm("r4");
    register u32 color_variant asm("r5");
    s16 *expanded_pixels = glyph_tile;
    register struct SingleByteFontRange *ranges asm("r3");
    register u32 range_index asm("r2");
    register u8 *glyph_pixels asm("r3");

    character_code <<= 24;
    code = (u32)character_code >> 24;
    text_color <<= 24;
    color_variant = (u32)text_color >> 24;
    {
        register u32 result asm("r0") = ModuloUnsigned32(color_variant, FONT_COLOR_VARIANTS_PER_PALETTE);

        result += 1;
        result <<= 24;
        color_variant = result >> 24;
    }

    range_index = 0;
    {
        register u32 range_start asm("r0");
        register u32 range_count asm("r1");
        register struct SingleByteFontRange *initial_range asm("r0") =
            (struct SingleByteFontRange *)0x087A0B48;
        register u32 initial_range_start asm("r1");

        initial_range_start = initial_range->first_code;
        ranges = initial_range;
        if (initial_range_start <= code) {
            range_start = ranges->first_code;
            range_count = ranges->glyph_count;
            goto test_font_range;
        }

next_font_range:
        {
            register u32 next asm("r0") = range_index + 1;

            next <<= 24;
            range_index = next >> 24;
        }
        if (range_index > (u32)(FONT_RANGE_COUNT - 1)) {
            goto glyph_unavailable;
        }
        {
            register u32 offset asm("r0") = range_index << 3;
            register u8 *entry asm("r1") =
                (u8 *)(offset + (u32)ranges);

            range_start = entry[0];
            if (range_start > code) {
                goto next_font_range;
            }
            range_count = entry[1];
        }
test_font_range:
        range_start += range_count;
        if ((s32)range_start <= (s32)code) {
            goto next_font_range;
        }
    }
    if (range_index > (u32)(FONT_RANGE_COUNT - 1)) {
        goto glyph_unavailable;
    }
    {
        register u32 offset asm("r0") = range_index << 3;
        register u8 *data_base asm("r1") = (u8 *)ranges + 4;
        register u8 **data_slot asm("r1");
        register u32 delta asm("r0");
        register u8 *glyphs asm("r1");

        data_slot = (u8 **)(offset + (u32)data_base);
        delta = code - *(u8 *)(offset + (u32)ranges);
        delta <<= 4;
        glyphs = *data_slot;
        glyph_pixels = glyphs + delta;
    }
    {
        register u32 source_byte asm("r4") = 0;
        register u32 color_lane0 asm("r8") = color_variant << 2;
        u32 color_lane1_value = color_variant << 4;
        register u32 color_lane1 asm("r12") = color_lane1_value;
        register u32 color_lane2 asm("r9") = color_variant << 6;
        register u32 color_lane3 asm("r5") = color_variant << 8;

        do {
            register u32 value asm("r2") = *glyph_pixels;
            register u32 packed asm("r1") = 3;
            register u32 temp asm("r0");
            u32 color_lane_value;
            register u32 next asm("r0");

            packed &= value;
            color_lane_value = color_lane0;
            packed |= color_lane_value;
            temp = 0xC;
            temp &= value;
            color_lane_value = color_lane1;
            temp |= color_lane_value;
            temp <<= 2;
            packed |= temp;
            temp = 0x30;
            temp &= value;
            color_lane_value = color_lane2;
            temp |= color_lane_value;
            temp <<= 4;
            packed |= temp;
            temp = 0xC0;
            temp &= value;
            temp |= color_lane3;
            temp <<= 6;
            packed |= temp;
            *expanded_pixels++ = packed;
            glyph_pixels += 1;
            next = source_byte + 1;
            next <<= 24;
            source_byte = next >> 24;
        } while (source_byte <= 15U);
    }
    return 1;

glyph_unavailable:
    return 0;
}
