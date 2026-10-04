#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySpriteGroup() asm("func_08095114");
extern int CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");

void InitializeBattleSingleSpriteSoundEffect(s32 group_address) asm("func_080DB9E4");

void InitializeBattleSingleSpriteSoundEffect(s32 group_address) {
    BATTLE_ANIMATION_FIELD((void *)group_address, s32, state) = 0;
}

void UpdateBattleSingleSpriteSoundEffect(struct BattleAnimationGroup *group) asm("func_080DB9EC");

void UpdateBattleSingleSpriteSoundEffect(struct BattleAnimationGroup *group) {
    s32 phase;
    phase = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0,
            BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y),
            phase, phase, phase);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = BATTLE_ANIMATION_FIELD(group, s32, state) + 1;
        return;
    }
    if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
