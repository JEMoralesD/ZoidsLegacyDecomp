#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s32 gRandomNumberCallback asm("D_03000010");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");

void InitializeBattlePurpleRingImpactEffect(struct BattleAnimationGroup *group) asm("func_080DA0C0");

void InitializeBattlePurpleRingImpactEffect(struct BattleAnimationGroup *group) {
    s32 random;
    s32 y_jitter;
    s32 minimum_y;
    s32 *impact_y_slot;

    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) = (s32)(
        (BATTLE_ANIMATION_FIELD(group, s32, x)
            + ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 0xF))
        - 0x20
    );
    random = CallFunctionR0(gRandomNumberCallback);
    impact_y_slot = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.impact.y));
    minimum_y = BATTLE_ANIMATION_FIELD(group, s32, y) - 0x10;
    y_jitter = (u32)(random * 0x21) >> 0xF;
    *impact_y_slot = minimum_y + y_jitter;
    PlayBattleAnimationSound(0);
}
