#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleSmokeTrailProjectileEffect(void *group) asm("func_080D4058");

void InitializeBattleSmokeTrailProjectileEffect(void *group) {
    register s32 *state asm("r2");
    register s32 *effect_parameter asm("r0");
    register s32 zero asm("r1");

    state = group;
    state = (s32 *)((u8 *)state + BATTLE_ANIMATION_OFFSET(state));
    effect_parameter = group;
    effect_parameter = (s32 *)((u8 *)effect_parameter + BATTLE_ANIMATION_OFFSET(effect.words[0]));
    zero = 0;
    *effect_parameter = zero;
    *state = zero;
}
