#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s32 gRandomNumberCallback asm("D_03000010");
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */

void InitializeBattleMissileExplosionColumnEffect(struct BattleAnimationGroup *group) asm("func_080D7CD0");

void InitializeBattleMissileExplosionColumnEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    s32 *burst_elapsed_slot = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.missile_explosion_column.burst_elapsed_frames));
    *burst_elapsed_slot = 0;
    *effect_state = 0;
    BATTLE_ANIMATION_FIELD(group, s32, effect.missile_explosion_column.impact_x) = BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 15) - 0x20;
}
