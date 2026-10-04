#include "m2c_prelude.h"
#include "zoid_body_sprites.h"

void UpdateStrumFuryBodySprites(struct ZoidBodySpriteGroup *group) asm("func_080E0D40");

void UpdateStrumFuryBodySprites(struct ZoidBodySpriteGroup *group) {
    u32 *phase_slot = &group->phase;
    u32 phase = *phase_slot;
    if (phase == ZOID_BODY_SPRITES_START) {
        u32 *sprite_flags;
        u32 flags;
        register int unpause_mask asm("r2");
        sprite_flags = ZOID_BODY_GROUP_FIELD(group, u32 *, sprites[0]);
        flags = *sprite_flags;
        unpause_mask = ~BATTLE_SPRITE_ANIMATION_PAUSED;
        flags &= unpause_mask;
        *sprite_flags = flags;
        sprite_flags = ZOID_BODY_GROUP_FIELD(group, u32 *, sprites[1]);
        flags = *sprite_flags;
        flags &= unpause_mask;
        *sprite_flags = flags;
        *phase_slot = ZOID_BODY_SPRITES_WAITING;
    } else if (phase == ZOID_BODY_SPRITES_WAITING) {
        u32 flags;
        int finished_mask;
        flags = *ZOID_BODY_GROUP_FIELD(group, u32 *, sprites[0]);
        finished_mask = BATTLE_SPRITE_ANIMATION_FINISHED;
        if ((flags & finished_mask) != 0) {
            flags = *ZOID_BODY_GROUP_FIELD(group, u32 *, sprites[1]);
            if ((flags & finished_mask) != 0) {
                *phase_slot = ZOID_BODY_SPRITES_READY;
            }
        }
    }
}
