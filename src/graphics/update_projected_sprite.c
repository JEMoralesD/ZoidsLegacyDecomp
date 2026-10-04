#include "m2c_prelude.h"
#include "../battle/battle_display.h"
s32 ProjectCameraPoint() asm("func_08093E30");

void UpdateProjectedSprite(u32 *sprite) asm("func_080BADD4");

void UpdateProjectedSprite(u32 *sprite) {
    u8 visible;
    s32 projected_scale;
    projected_scale = ProjectCameraPoint((u8 *)sprite + 0x28, sprite + 1, &visible);
    BATTLE_SPRITE_FIELD(sprite, s16, scale) = projected_scale;
    if (visible == 0 || (projected_scale << 16) > 0x3000000) {
        BATTLE_SPRITE_FIELD(sprite, u32, flags) |= 0x20000;
    } else {
        BATTLE_SPRITE_FIELD(sprite, u32, flags) &= 0xFFFDFFFF;
    }
}
