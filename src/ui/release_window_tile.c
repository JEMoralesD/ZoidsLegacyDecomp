#include "m2c_prelude.h"
extern s32 gWindowTilesToRelease[];
void ReleaseWindowTile(s32 tile_index) {
    u32 tile_bits = tile_index << 0x10;
    s32 released_base = (s32)gWindowTilesToRelease;
    s32 *released_word = (s32 *)(released_base + (tile_bits >> 0x15) * 4);
    u32 bit_index = 0x1F0000;
    bit_index &= tile_bits;
    bit_index >>= 0x10;
    *released_word |= 1 << bit_index;
}
