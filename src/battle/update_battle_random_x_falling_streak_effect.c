#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
s32 CreateBattleAngledProjectileSprite(void *, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_080D2660");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");
s32 CallFunctionR0(s32) asm("func_080ECD5C");

void UpdateBattleRandomXFallingStreakEffect(struct BattleAnimationGroup *group) asm("func_080DC1E8");

void UpdateBattleRandomXFallingStreakEffect(struct BattleAnimationGroup *group) {
    s32 phase;

    phase = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAngledProjectileSprite(group, 0, 0, (s16) (BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0x41) >> 0xF)), phase, (BATTLE_SPRITE_LOOP_ANIMATION | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0x4A, 0xC00, 1);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = (s32) (BATTLE_ANIMATION_FIELD(group, s32, state) + 1);
        return;
    }
    if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
