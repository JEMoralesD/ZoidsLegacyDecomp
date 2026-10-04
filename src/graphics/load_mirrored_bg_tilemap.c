#include "m2c_prelude.h"
#include "graphics_resources.h"
void BiosLz77ToVram() asm("func_80ECD34");
void BiosLz77ToWram() asm("func_80ECD38");

void LoadMirroredBgTilemap(s32 compressed_tilemap, u8 screen_block, s32 mirrored, s32 work_buffer) asm("func_0809AC30");

void LoadMirroredBgTilemap(s32 compressed_tilemap, u8 screen_block, s32 mirrored, s32 work_buffer) {
    s16 *tilemap_destination;
    u8 row, column;
    u8 destination_screen_block = screen_block;

    if ((mirrored << 0x18) == 0) {
        BiosLz77ToVram(compressed_tilemap, (destination_screen_block << 0xB) + 0x06000000);
        return;
    }
    BiosLz77ToWram(compressed_tilemap, work_buffer);
    tilemap_destination = (s16 *)((destination_screen_block << 0xB) + 0x06000000);
    row = 0;
    do {
        column = 0;
        do {
            *tilemap_destination = *(s16 *)((row << 6) + (((0x1F - column) * 2) + work_buffer)) ^ GRAPHICS_BG_TILEMAP_FLIP_X;
            tilemap_destination += 1;
            column += 1;
        } while ((u32)column <= 0x1F);
        row += 1;
    } while ((u32)row <= 0x1F);
}
