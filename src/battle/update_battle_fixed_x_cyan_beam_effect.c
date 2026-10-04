#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
s32 CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleFixedXCyanBeamEffect(struct BattleAnimationGroup *group) asm("func_080DD220");

void UpdateBattleFixedXCyanBeamEffect(struct BattleAnimationGroup *group) {
    s32 phase;

    phase = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAnimationSprite(group, 0, 1, 0x80, (s32) BATTLE_ANIMATION_FIELD(group, s16, y), BATTLE_SPRITE_SEMITRANSPARENT, phase, 1);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = (s32) (BATTLE_ANIMATION_FIELD(group, s32, state) + 1);
        return;
    }
    if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
