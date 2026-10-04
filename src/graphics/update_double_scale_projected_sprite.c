#include "m2c_prelude.h"
#include "../battle/battle_display.h"
int ProjectCameraPoint(void *, void *, void *) asm("func_08093E30");

void UpdateDoubleScaleProjectedSprite(void *sprite) asm("func_080BAE18");

void UpdateDoubleScaleProjectedSprite(void *sprite) {
    u8 visible;
    int projected_scale;
    register u32 flags asm("r0");

    projected_scale = (s16) ProjectCameraPoint((u8 *)sprite + 0x28, (u8 *)sprite + 4, &visible);
    BATTLE_SPRITE_FIELD(sprite, u16, scale) = projected_scale << 1;
    if (visible == 0 || (projected_scale << 17) > 0x03000000) {
        flags = BATTLE_SPRITE_FIELD(sprite, u32, flags) | 0x20000;
    } else {
        flags = BATTLE_SPRITE_FIELD(sprite, u32, flags) & 0xFFFDFFFF;
    }
    BATTLE_SPRITE_FIELD(sprite, u32, flags) = flags;
}
