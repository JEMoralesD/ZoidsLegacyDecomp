#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
s32 CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleOffsetLightningEffect(struct BattleAnimationGroup *group) asm("func_080DB17C");

void UpdateBattleOffsetLightningEffect(struct BattleAnimationGroup *group) {
    s32 phase;

    phase = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAnimationSprite(
            group, 0, 0,
            (s16)(BATTLE_ANIMATION_FIELD(group, s32, x)
                + BATTLE_LIGHTNING_SPRITE_HALF_WIDTH_PIXELS),
            (s32)BATTLE_ANIMATION_FIELD(group, s16, y), phase, phase,
            BATTLE_ANIMATION_SPRITE_REVERSE_FACING
        );
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = (s32) (BATTLE_ANIMATION_FIELD(group, s32, state) + 1);
        return;
    }
    if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
