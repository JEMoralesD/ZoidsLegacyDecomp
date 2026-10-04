#include "m2c_prelude.h"

extern void *CreateSpriteFromTable() asm("func_08094374");

void *CreateMirroredSpriteFromTable(void *resource_table, u16 resource_id, u16 animation_id, u16 x,
                   u16 y, u16 tile_offset, u16 palette_bank, s32 flags,
                   s32 callback, s32 mirrored) asm("func_080D22B4");

void *CreateMirroredSpriteFromTable(void *resource_table, u16 resource_id, u16 animation_id, u16 x,
                   u16 y, u16 tile_offset, u16 palette_bank, s32 flags,
                   s32 callback, s32 mirrored) {
    s32 sprite_x;

    mirrored <<= 24;
    if (mirrored != 0) {
        s32 signed_x = (s16)x;

        if (flags & 0x1000) {
            sprite_x = 0x100 - signed_x;
            __asm__ volatile ("" : : : "memory");
            sprite_x <<= 16;
        } else {
            sprite_x = 0xF0;
            sprite_x -= signed_x;
            sprite_x <<= 16;
        }
    } else {
        sprite_x = x << 16;
    }

    return CreateSpriteFromTable(resource_table, resource_id, animation_id, sprite_x >> 16, (s16)y,
                        tile_offset, palette_bank, flags, callback);
}
