#include "m2c_prelude.h"
extern s32 gWindowTileUsage[];
void ReserveWindowTile(s32 tile_index) {
    u32 tile_bits = tile_index << 0x10;
    s32 usage_base = (s32)gWindowTileUsage;
    s32 *usage_word = (s32 *)(usage_base + (tile_bits >> 0x15) * 4);
    u32 bit_index = 0x1F0000;
    bit_index &= tile_bits;
    bit_index >>= 0x10;
    *usage_word |= 1 << bit_index;
}
