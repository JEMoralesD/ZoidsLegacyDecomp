#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern s32 gRandomNumberCallback asm("D_03000010");
void InitializeBattleFallingStreakImpactSparkEffect(struct BattleAnimationGroup *group) asm("func_080DBFF0");

void InitializeBattleFallingStreakImpactSparkEffect(struct BattleAnimationGroup *group) {
    register s32 *effect_state asm("r2") = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    register s32 *elapsed_updates_slot asm("r1") = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.impact.elapsed_frames));
    s32 initial_phase_and_elapsed_updates = 0;
    *elapsed_updates_slot = initial_phase_and_elapsed_updates;
    *effect_state = initial_phase_and_elapsed_updates;
    BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) = BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 15) - 0x20;
    {
        s32 y_random = CallFunctionR0(gRandomNumberCallback);
        register s32 *impact_y_slot asm("r3") = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.impact.y));
        register s32 impact_y asm("r2") = BATTLE_ANIMATION_FIELD(group, s32, y);
        register u32 y_random_offset asm("r1");
        impact_y -= 0x10;
        y_random_offset = (u32)(y_random * 0x21) >> 15;
        impact_y += y_random_offset;
        *impact_y_slot = impact_y;
    }
}
