#include "m2c_prelude.h"
#include "zoid_body_sprites.h"
void SetSpriteAnimation() asm("func_8094564");

void UpdateBerserkFuryBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0C44");

void UpdateBerserkFuryBodySprites(struct ZoidBodySpriteGroup *group) {
    s32 *phase_slot = &ZOID_BODY_GROUP_FIELD(group, s32, phase);
    s32 phase = *phase_slot;
    s32 next_phase;
    if (phase == ZOID_BODY_SPRITES_START) {
        SetSpriteAnimation((s32 *)ZOID_BODY_GROUP_FIELD(group, s32, sprites[0]), group->animation.berserk_fury_animation_id);
        SetSpriteAnimation((s32 *)ZOID_BODY_GROUP_FIELD(group, s32, sprites[1]), group->animation.berserk_fury_animation_id);
        *(s32 *)ZOID_BODY_GROUP_FIELD(group, s32, sprites[0]) &= ~BATTLE_SPRITE_ANIMATION_PAUSED;
        *(s32 *)ZOID_BODY_GROUP_FIELD(group, s32, sprites[1]) &= ~BATTLE_SPRITE_ANIMATION_PAUSED;
        next_phase = ZOID_BODY_SPRITES_WAITING;
    } else {
        if (phase != ZOID_BODY_SPRITES_WAITING) {
            return;
        }
        if ((*(s32 *)ZOID_BODY_GROUP_FIELD(group, s32, sprites[0]) & BATTLE_SPRITE_ANIMATION_FINISHED) == 0) {
            return;
        }
        if ((*(s32 *)ZOID_BODY_GROUP_FIELD(group, s32, sprites[1]) & BATTLE_SPRITE_ANIMATION_FINISHED) == 0) {
            return;
        }
        next_phase = ZOID_BODY_SPRITES_READY;
    }
    *phase_slot = next_phase;
}
