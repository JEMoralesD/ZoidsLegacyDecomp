#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern u32 CallFunctionR0(u32) asm("func_80ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void InitializeBattleMissileImpactEffect(struct BattleAnimationGroup *group) asm("func_080D6590");

void InitializeBattleMissileImpactEffect(struct BattleAnimationGroup *group) {
    group->state = group->effect.missile_impact.elapsed_frames = 0;
    group->effect.missile_impact.x = group->x + (CallFunctionR0(gRandomNumberCallback) * 65 >> 15) - 32;
    {
        u32 random = CallFunctionR0(gRandomNumberCallback);
        s32 *impact_y_slot = &group->effect.missile_impact.y;
        s32 minimum_y = group->y - 16;
        *impact_y_slot = minimum_y + (random * 33 >> 15);
    }
}
