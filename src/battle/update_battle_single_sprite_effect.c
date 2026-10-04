#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");

void UpdateBattleSingleSpriteEffect(void *group) asm("func_080D27D8");

void UpdateBattleSingleSpriteEffect(void *group) {
    if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
