#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern void DestroySpriteGroup(void *) asm("func_8095114");

void UpdateBattleFacingStrikeEffect(struct BattleAnimationGroup *group) asm("func_080D5708");

void UpdateBattleFacingStrikeEffect(struct BattleAnimationGroup *group) {
    s32 *state_slot = &BATTLE_ANIMATION_FIELD(group, s32, state);
    s32 state = *state_slot;
    if (state == 0) {
        BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0,
            BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y), BATTLE_SPRITE_SEMITRANSPARENT, state, 1);
        PlayBattleAnimationSound(0);
        *state_slot += 1;
    } else if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}

void InitializeBattleSlashBurstEffect(struct BattleAnimationGroup *group) asm("func_080D575C");

void InitializeBattleSlashBurstEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}
