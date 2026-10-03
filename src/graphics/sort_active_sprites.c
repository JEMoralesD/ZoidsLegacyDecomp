/* Compatible with the m2c_prelude.h types used by tools/pmatch.py. */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Sprite {
    u32 flags;
    u16 x;
    s16 y;
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

extern u8 gActiveSpriteCount;
extern struct Sprite gSpritePool[];
extern struct Sprite *gActiveSprites[];

void SortActiveSprites(void) {
    s16 sprite_index;
    s16 insert_index;
    struct Sprite *sprite;
    struct Sprite *previous_sprite;

    gActiveSpriteCount = 0;
    for (sprite_index = 0; sprite_index < 0x80; sprite_index++) {
        sprite = &gSpritePool[sprite_index];
        if (sprite->flags & 1) {
            gActiveSprites[gActiveSpriteCount] = sprite;
            gActiveSpriteCount++;
        } else {
            sprite->flags &= ~2;
        }
    }

    for (sprite_index = 1; sprite_index < gActiveSpriteCount; sprite_index++) {
        sprite = gActiveSprites[sprite_index];
        insert_index = sprite_index - 1;
        while (insert_index >= 0) {
            previous_sprite = gActiveSprites[insert_index];
            if ((sprite->flags & 0xC0) < (previous_sprite->flags & 0xC0)) {
                /* shift */
            } else if ((sprite->flags & 0xC0) != (previous_sprite->flags & 0xC0)) {
                break;
            } else if ((sprite->flags & 0x300) < (previous_sprite->flags & 0x300)) {
                /* shift */
            } else if ((sprite->flags & 0x40000) == 0) {
                break;
            } else if ((previous_sprite->flags & 0x40000) == 0) {
                /* shift */
            } else if (sprite->y > previous_sprite->y) {
                /* shift */
            } else {
                break;
            }
            gActiveSprites[insert_index + 1] = previous_sprite;
            insert_index--;
        }
        gActiveSprites[insert_index + 1] = sprite;
    }
}
