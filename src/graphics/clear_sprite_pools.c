#include "m2c_prelude.h"
void ClearSpritePools(void) {
    s32 next_sprite_index_bits;
    u8 pool_index;
    s32 groups;
    pool_index = 0;
    do {
        *(s32 *)(pool_index * 0x38 + 0x03003FE4) = 0;
        next_sprite_index_bits = (pool_index + 1) << 0x18;
        pool_index = (u32)next_sprite_index_bits >> 0x18;
    } while (next_sprite_index_bits >= 0);
    pool_index = 0;
    groups = 0x030034A4;
    do {
        *(s32 *)(pool_index * 0xB4 + groups) = 0;
        pool_index = pool_index + 1;
    } while (pool_index <= 0xF);
}
