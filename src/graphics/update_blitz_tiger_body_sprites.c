#include "m2c_prelude.h"
#include "zoid_body_sprites.h"
extern void SetSpriteAnimation(s32 *, s32) asm("func_8094564");

void UpdateBlitzTigerBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0EE0");

void UpdateBlitzTigerBodySprites(struct ZoidBodySpriteGroup *group) {
    s32 *phase_slot;
    s32 phase;
    s32 unpause_mask;
    s32 finished_mask;
    s32 *first_sprite_flags;
    s32 first_flags;
    s32 completion_flags;

    phase_slot = &ZOID_BODY_GROUP_FIELD(group, s32, phase);
    phase = *phase_slot;
    if (phase == ZOID_BODY_SPRITES_START) {
        first_sprite_flags = ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[0]);
        first_flags = *first_sprite_flags;
        unpause_mask = ~BATTLE_SPRITE_ANIMATION_PAUSED;
        *first_sprite_flags = first_flags & unpause_mask;
        *ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[1]) &= unpause_mask;
        if (group->animation.blitz_tiger_skip_startup != 0) {
            SetSpriteAnimation(first_sprite_flags, BLITZ_TIGER_BODY_FIRST_CLAW_WITHOUT_STARTUP);
            SetSpriteAnimation(ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[1]), BLITZ_TIGER_BODY_SECOND_CLAW_WITHOUT_STARTUP);
        }
        *phase_slot = ZOID_BODY_SPRITES_WAITING;
    } else if (phase == ZOID_BODY_SPRITES_WAITING) {
        completion_flags = *ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[0]);
        finished_mask = BATTLE_SPRITE_ANIMATION_FINISHED;
        if ((completion_flags & finished_mask) != 0) {
            if ((*ZOID_BODY_GROUP_FIELD(group, s32 *, sprites[1]) & finished_mask) != 0) {
                *phase_slot = ZOID_BODY_SPRITES_READY;
            }
        }
    }
}
