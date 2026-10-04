#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleRotatingTriangleEffect(void *group) asm("func_080D27EC");

void InitializeBattleRotatingTriangleEffect(void *group) {
    s32 *initialized = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(state));
    s32 *angle = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.rotating_triangles.angle_x));
    s32 *tilt = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.rotating_triangles.angle_y));
    s32 *progress = (s32 *)((s32)group + BATTLE_ANIMATION_OFFSET(effect.rotating_triangles.elapsed_frames));
    *progress = 0;
    *tilt = 0;
    *angle = 0;
    *initialized = 0;
}
