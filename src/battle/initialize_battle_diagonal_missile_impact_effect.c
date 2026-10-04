#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gRandomNumberCallback asm("D_03000010");
s32 CallFunctionR0() asm("func_80ECD5C");

void InitializeBattleDiagonalMissileImpactEffect(struct BattleAnimationGroup *group) asm("func_080D701C");

void InitializeBattleDiagonalMissileImpactEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    s32 *impact_elapsed_slot = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.missile_impact.elapsed_frames));
    *impact_elapsed_slot = 0;
    *effect_state = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.missile_impact.x) =
        (BATTLE_ANIMATION_FIELD(group, s32, x) + (s32)((u32)(CallFunctionR0(gRandomNumberCallback) * 65) >> 15)) - 0x20;
    {
        s32 random = CallFunctionR0(gRandomNumberCallback);
        s32 *impact_y_slot = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y));
        s32 minimum_y = BATTLE_ANIMATION_FIELD(group, s32, y) - 0x10;
        *impact_y_slot = minimum_y + (s32)((u32)(random * 33) >> 15);
    }
}
