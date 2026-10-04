#include "m2c_prelude.h"
#include "../battle/battle_display.h"
extern s16 ProjectCameraPoint(void *, void *, u8 *) asm("func_08093E30");


void UpdateHalfScaleProjectedSprite(struct BattleDisplaySprite *sprite) asm("func_080BAE60");

void UpdateHalfScaleProjectedSprite(struct BattleDisplaySprite *sprite) {
    u8 visible;
    s32 projected_scale;
    projected_scale = ProjectCameraPoint(&BATTLE_SPRITE_FIELD(sprite, u8, user_data.position.x), &BATTLE_SPRITE_FIELD(sprite, u8, x), &visible) / 2;
    BATTLE_SPRITE_FIELD(sprite, s16, scale) = projected_scale;
    if (visible == 0 || projected_scale > 0x300) {
        BATTLE_SPRITE_FIELD(sprite, s32, flags) |= 0x20000;
    } else {
        BATTLE_SPRITE_FIELD(sprite, s32, flags) &= ~0x20000;
    }
}
