#include "m2c_prelude.h"

struct Sprite {
    s32 flags;
    u16 x;
    u16 y;
    u16 offset_x;
    u16 offset_y;
    u16 scale;
    u16 tile_offset;
    u8 palette_bank;
    u8 rotation;
    u16 animation_id;
    u16 animation_frame;
    u16 frame_timer;
    s32 frame_table;
    s32 animation_table;
    s32 graphics;
    s32 update_callback;
    s32 field28;
    s32 field2C;
    s32 field30;
    s32 field34;
};

extern struct Sprite gSpritePool[];
void BiosCpuFastSet(s32, void *, s32) asm("func_080ECD28");

struct Sprite *CreateSpriteFromTable(s32 resource_table, u16 resource_id, u16 animation_id,
    u16 x, u16 y, u16 tile_offset, u16 palette_bank, s32 flags, s32 update_callback)
{
    struct Sprite *sprite;
    struct Sprite *sprites;
    struct Sprite *scan_sprites;
    s16 slot_index;

    slot_index = 0x7F;
    sprites = gSpritePool;
    sprite = (struct Sprite *)((s32)sprites + 0x1BC8);
    if (sprite->flags & 3) {
        scan_sprites = sprites;
        do {
            slot_index--;
            if (slot_index < 0) {
                break;
            }
            sprite = (struct Sprite *)(slot_index * 0x38 +
                (s32)scan_sprites);
        } while (sprite->flags & 3);
    }
    if (slot_index == -1) {
        return 0;
    }

    sprite->flags = flags | 1;
    sprite->x = x;
    sprite->y = y;
    sprite->offset_y = 0;
    sprite->offset_x = 0;
    sprite->rotation = 0;
    sprite->scale = 0x100;
    sprite->tile_offset = tile_offset;
    sprite->palette_bank = palette_bank;
    sprite->animation_id = animation_id;
    sprite->animation_frame = 0;
    sprite->frame_timer = 0;

    if ((flags & 0x400000) == 0) {
        s32 *source = (s32 *)(resource_id * 8 + resource_table);

        sprite->frame_table = source[0];
        sprite->animation_table = source[1];
    } else {
        s32 *source = (s32 *)(resource_id * 16 + resource_table);

        sprite->frame_table = source[0];
        sprite->animation_table = source[1];
        sprite->graphics = source[2];
        BiosCpuFastSet(source[3],
            (void *)(0x05000200 + (sprite->palette_bank << 5)), 8);
    }
    sprite->update_callback = update_callback;
    return sprite;
}
