#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySprite(void *) asm("func_8094554");


void UpdateBattleHorizontalProjectileSprite(struct BattleDisplaySprite *sprite) asm("func_080D2528");

void UpdateBattleHorizontalProjectileSprite(struct BattleDisplaySprite *sprite) {
    register int next_x asm("r1");
    int speed;
    if ((sprite->flags & BATTLE_SPRITE_FLIP_X) == 0) {
        speed = sprite->user_data.horizontal_projectile.speed;
        next_x = sprite->x;
        next_x -= speed;
        sprite->x = next_x;
        if ((s16)next_x < -16) DestroySprite(sprite);
    } else {
        speed = sprite->user_data.horizontal_projectile.speed;
        next_x = sprite->x;
        next_x += speed;
        sprite->x = next_x;
        if ((s16)next_x > 256) DestroySprite(sprite);
    }
}
