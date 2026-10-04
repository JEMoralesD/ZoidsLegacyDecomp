#include "m2c_prelude.h"
struct SpriteGroup { s32 flags; s32 reserved[2]; s32 *sprites[32]; };
void DestroySprite(s32 *) asm("func_8094554");
void DestroySpriteGroup(struct SpriteGroup *group) {
    u8 sprite_index; s32 *sprite;
    group->flags &= ~1;
    sprite_index = 0;
    do {
        sprite = group->sprites[sprite_index];
        if ((sprite != 0) && (*sprite & 1)) {
            DestroySprite(sprite);
        }
        sprite_index += 1;
    } while ((u32) sprite_index <= 0x1FU);
}
