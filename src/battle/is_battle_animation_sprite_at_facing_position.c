#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 IsBattleAnimationSpriteAtFacingPosition(void *sprite, s32 target_x, s32 target_y) asm("func_080D2754");

s32 IsBattleAnimationSpriteAtFacingPosition(void *sprite, s32 target_x, s32 target_y) {
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
        if (BATTLE_SPRITE_FIELD(sprite, s16, x) == (0xF0 - target_x)) {
            goto check_y;
        }
        goto position_differs;
    }
    if (BATTLE_SPRITE_FIELD(sprite, s16, x) == target_x) {
check_y:
        if (BATTLE_SPRITE_FIELD(sprite, s16, y) == target_y) {
            return 1;
        }
        goto position_differs;
    }
position_differs:
    return 0;
}
