#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");
void UpdateBattleRandomHeightProjectileEffect(void *group) asm("func_080D3014");

void UpdateBattleRandomHeightProjectileEffect(void *group) {
    s32 state = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (state == 0) {
        void *projectile = CreateBattleAnimationSprite(group, 0, 0, (s16)(BATTLE_ANIMATION_FIELD(group, s32, x) - 0x10), BATTLE_ANIMATION_FIELD(group, s16, effect.random_height.y), state, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, s32, user_data.horizontal_projectile.speed) = 0x10;
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state)++;
        return;
    }
    if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0) DestroySpriteGroup(group);
}
