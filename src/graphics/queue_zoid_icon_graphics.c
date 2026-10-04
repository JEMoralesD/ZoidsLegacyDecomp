#include "m2c_prelude.h"
#include "graphics_resources.h"
extern s32 gZoidIconTiles[] asm("D_087AA70C");
extern s32 gZoidIconPalettes[][8] asm("D_087AA96C");
extern void BiosLz77ToWram() asm("func_80ECD38");
extern void QueueCopy() asm("func_8095208");

void QueueZoidIconGraphics(u8 zoid_id, u8 palette_variant, u16 tile_offset, u8 palette_bank, s32 work_buffer) asm("func_0809A52C");

void QueueZoidIconGraphics(u8 zoid_id, u8 palette_variant, u16 tile_offset, u8 palette_bank, s32 work_buffer) {
    s32 palette_buffer;
    s32 tile_bytes;
    s32 compressed_palette;
    BiosLz77ToWram(gZoidIconTiles[zoid_id], work_buffer);
    compressed_palette = gZoidIconPalettes[zoid_id][palette_variant];
    tile_bytes = GRAPHICS_ZOID_ICON_BYTES;
    palette_buffer = work_buffer + tile_bytes;
    BiosLz77ToWram(compressed_palette, palette_buffer);
    QueueCopy(work_buffer, (tile_offset << 5) + 0x06010000, tile_bytes);
    QueueCopy(palette_buffer, (palette_bank << 5) + 0x05000200, 0x20);
}
