#include "m2c_prelude.h"
#include "../battle/battle_display.h"
extern s32 Sin256(s32) asm("func_08092A90");
extern s32 ProjectCameraPoint(void *, void *, void *) asm("func_08093E30");

void UpdateBobbingProjectedSprite(s32 *sprite) asm("func_080A6DB0");

void UpdateBobbingProjectedSprite(s32 *sprite) {
    s32 sine_y;
    u8 visible;
    s32 projected_scale;

    sine_y = Sin256(BATTLE_SPRITE_FIELD(sprite, s16, user_data.bobbing_projection.bob_phase));
    sine_y = (s16)sine_y;
    if (sine_y < 0) {
        sine_y += 0x1F;
    }
    BATTLE_SPRITE_FIELD(sprite, s16, offset_y) = (sine_y >> 5) - 0x20;
    BATTLE_SPRITE_FIELD(sprite, s32, user_data.bobbing_projection.bob_phase) += 2;
    projected_scale = ProjectCameraPoint((u8 *)sprite + BATTLE_SPRITE_OFFSET(user_data.bobbing_projection.position),
        (u8 *)sprite + BATTLE_SPRITE_OFFSET(x), &visible);
    BATTLE_SPRITE_FIELD(sprite, s16, scale) = projected_scale;
    if (visible == 0 || ((s16)projected_scale << 0x10) > 0x03000000) {
        sprite[0] |= BATTLE_SPRITE_HIDDEN;
    } else {
        sprite[0] &= ~BATTLE_SPRITE_HIDDEN;
    }
}
