#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleHeldFrameProjectileFlashEffect(void *group) asm("func_080D3360");

void UpdateBattleHeldFrameProjectileFlashEffect(void *group) {
    s32 state;
    void *projectile;
    state = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (state == 0) {
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0, BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y), 0x400, state, state);
        projectile = CreateBattleAnimationSprite(group, 1, 0, BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y), 0x510, (s32)UpdateBattleHorizontalProjectileSprite, state);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[1]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, s32, user_data.horizontal_projectile.speed) = 0x10;
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) += 1;
        return;
    }
    if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0 && BATTLE_ANIMATION_FIELD(group, void *, sprites[1]) == 0) {
        DestroySpriteGroup(group);
    }
}
