#include "m2c_prelude.h"
#include "battle_display.h"
void DestroySprite(void *) asm("func_8094554");

void UpdateBattleHorizontalStreakParticle(void *sprite) asm("func_080CCEF0");

void UpdateBattleHorizontalStreakParticle(void *sprite) {
    register u32 x_fixed8_or_pixel_bits asm("r0");

    if ((BATTLE_SPRITE_FIELD(sprite, u32, flags) & 0x8000) == 0) {
        x_fixed8_or_pixel_bits = BATTLE_SPRITE_FIELD(sprite, u32, user_data.horizontal_streak.x_fixed8);
        x_fixed8_or_pixel_bits += BATTLE_SPRITE_FIELD(sprite, u32, user_data.horizontal_streak.speed_fixed8);
        BATTLE_SPRITE_FIELD(sprite, u32, user_data.horizontal_streak.x_fixed8) = x_fixed8_or_pixel_bits;
        x_fixed8_or_pixel_bits >>= 8;
        BATTLE_SPRITE_FIELD(sprite, u16, x) = x_fixed8_or_pixel_bits;
        if ((s32)(x_fixed8_or_pixel_bits << 16) > 0x01100000) {
            DestroySprite(sprite);
        }
    } else {
        x_fixed8_or_pixel_bits = BATTLE_SPRITE_FIELD(sprite, u32, user_data.horizontal_streak.x_fixed8) - BATTLE_SPRITE_FIELD(sprite, u32, user_data.horizontal_streak.speed_fixed8);
        BATTLE_SPRITE_FIELD(sprite, u32, user_data.horizontal_streak.x_fixed8) = x_fixed8_or_pixel_bits;
        x_fixed8_or_pixel_bits >>= 8;
        BATTLE_SPRITE_FIELD(sprite, u16, x) = x_fixed8_or_pixel_bits;
        if ((s32)(x_fixed8_or_pixel_bits << 16) < 0) {
            DestroySprite(sprite);
        }
    }
}
