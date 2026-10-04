#include "m2c_prelude.h"
#include "battle_display.h"
extern void DestroySprite(void *) asm("func_08094554");

void UpdateBattleDiagonalCloudSprite(void *sprite) asm("func_080CD258");

void UpdateBattleDiagonalCloudSprite(void *sprite) {
    s32 sprite_flags = BATTLE_SPRITE_FIELD(sprite, s32, flags);
    s32 next_y;
    if ((0xC0 & sprite_flags) == 0xC0) {
        if (!(sprite_flags & BATTLE_SPRITE_FLIP_X)) {
            BATTLE_SPRITE_FIELD(sprite, u16, x) = BATTLE_SPRITE_FIELD(sprite, u16, x) + 0x10;
        } else {
            BATTLE_SPRITE_FIELD(sprite, u16, x) = BATTLE_SPRITE_FIELD(sprite, u16, x) - 0x10;
        }
        next_y = BATTLE_SPRITE_FIELD(sprite, u16, y) - 8;
    } else {
        if (!(sprite_flags & BATTLE_SPRITE_FLIP_X)) {
            BATTLE_SPRITE_FIELD(sprite, u16, x) = BATTLE_SPRITE_FIELD(sprite, u16, x) + 0x18;
        } else {
            BATTLE_SPRITE_FIELD(sprite, u16, x) = BATTLE_SPRITE_FIELD(sprite, u16, x) - 0x18;
        }
        next_y = BATTLE_SPRITE_FIELD(sprite, u16, y) - 0xC;
    }
    BATTLE_SPRITE_FIELD(sprite, u16, y) = next_y;
    if (BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X) {
        if ((s32) BATTLE_SPRITE_FIELD(sprite, s16, x) < -0x40) {
            goto destroy_cloud;
        }
        goto check_vertical_boundary;
    }
    if ((s32) BATTLE_SPRITE_FIELD(sprite, s16, x) <= 0x130) {
check_vertical_boundary:
        if ((s32) BATTLE_SPRITE_FIELD(sprite, s16, y) < -0x40) {
            goto destroy_cloud;
        }
    } else {
destroy_cloud:
        DestroySprite(sprite);
    }
}
