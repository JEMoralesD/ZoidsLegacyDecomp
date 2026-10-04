#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

s16 Sin256(u8) asm("func_08092A90");
s16 Cos256(u8) asm("func_08092ADC");
void DestroySprite(void *) asm("func_08094554");

void UpdateBattleAngledProjectileSprite(struct BattleDisplaySprite *sprite) asm("func_080D2570");

void UpdateBattleAngledProjectileSprite(struct BattleDisplaySprite *sprite) {
    s32 next_x;
    s32 mirrored_next_x;
    s32 next_y;
    s32 x_step;
    s32 x_product;
    s32 y_product;
    s32 screen_x;
    s32 screen_y;
    u8 angle;

    angle = (u8) sprite->user_data.angled_projectile.angle;
    x_product = Cos256(angle) * sprite->user_data.angled_projectile.speed_fixed8;
    if (x_product < 0) x_product += 0xFF;
    x_step = x_product >> 8;
    if (!(sprite->flags & BATTLE_SPRITE_FLIP_X)) {
        next_x = sprite->user_data.angled_projectile.x_fixed8 + x_step;
        sprite->user_data.angled_projectile.x_fixed8 = next_x;
        if (((u32) (u8) (angle - 0x40) <= 0x80U) || (next_x <= 0x10000)) {
            if ((u32) (u8) (angle - 0x41) > 0x7EU) goto move_vertical;
            if (next_x >= (s32) 0xFFFFF000) goto move_vertical;
        }
        goto destroy_projectile;
    }
    mirrored_next_x = sprite->user_data.angled_projectile.x_fixed8 - x_step;
    sprite->user_data.angled_projectile.x_fixed8 = mirrored_next_x;
    if ((u32) (u8) (angle - 0x40) > 0x80U) {
        if (mirrored_next_x < (s32) 0xFFFFF000) goto destroy_projectile;
    }
    if ((u32) (u8) (angle - 0x41) <= 0x7EU) {
        if (mirrored_next_x > 0x10000) goto destroy_projectile;
    }
move_vertical:
    y_product = Sin256(angle) * sprite->user_data.angled_projectile.speed_fixed8;
    if (y_product < 0) y_product += 0xFF;
    next_y = sprite->user_data.angled_projectile.y_fixed8 + (y_product >> 8);
    sprite->user_data.angled_projectile.y_fixed8 = next_y;
    if ((u32) (u8) (angle - 1) <= 0x7EU) {
        if (next_y > 0x9000) goto destroy_projectile;
    }
    if ((u32) angle <= 0x80U) goto store_screen_position;
    if (next_y >= (s32) 0xFFFFF000) goto store_screen_position;
destroy_projectile:
    DestroySprite(sprite);
    return;
store_screen_position:
    screen_x = sprite->user_data.angled_projectile.x_fixed8;
    if (screen_x < 0) screen_x += 0xFF;
    sprite->x = (s16) (screen_x >> 8);
    screen_y = sprite->user_data.angled_projectile.y_fixed8;
    if (screen_y < 0) screen_y += 0xFF;
    sprite->y = (s16) (screen_y >> 8);
}
