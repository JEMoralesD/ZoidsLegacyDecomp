#include "m2c_prelude.h"
#include "zoid_body_sprites.h"

void UpdateZeroSchneiderBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0B74");

void UpdateZeroSchneiderBodySprites(struct ZoidBodySpriteGroup *group) {
    s32 phase = ZOID_BODY_GROUP_FIELD(group, s32, phase);
    if (phase == ZOID_BODY_SPRITES_START) {
        s32 *sprite_flags = ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[0]);
        *sprite_flags = *sprite_flags & ~BATTLE_SPRITE_ANIMATION_PAUSED;
        ZOID_BODY_GROUP_FIELD(group, s32, phase) = ZOID_BODY_SPRITES_WAITING;
    } else if (phase == ZOID_BODY_SPRITES_WAITING) {
        s32 *sprite_flags = ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[0]);
        if (*sprite_flags & BATTLE_SPRITE_ANIMATION_FINISHED) {
            ZOID_BODY_GROUP_FIELD(group, s32, phase) = ZOID_BODY_SPRITES_READY;
        }
    }
}
