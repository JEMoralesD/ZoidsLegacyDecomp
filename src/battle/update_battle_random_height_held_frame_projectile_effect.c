#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
void *CreateBattleAnimationSprite(void *, int, int, int, int, int, int, int) asm("func_080D2450");
void PlayBattleAnimationSound(int) asm("func_080D2790");
void DestroySpriteGroup(void *) asm("func_8095114");

void UpdateBattleRandomHeightHeldFrameProjectileEffect(void *group) asm("func_080D3750");

void UpdateBattleRandomHeightHeldFrameProjectileEffect(void *group) {
    u32 *state;
    void *projectile;

    state = (u32 *)((s8 *)group + 0x8C);
    if (*state == 0) {
        projectile = CreateBattleAnimationSprite(group, 0, 0, (s16)(BATTLE_ANIMATION_FIELD(group, int, x) - 16),
                         BATTLE_ANIMATION_FIELD(group, s16, effect.random_height.y), 0x410, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, int, user_data.horizontal_projectile.speed) = 16;
        PlayBattleAnimationSound(0);
        *state += 1;
    } else if (BATTLE_ANIMATION_FIELD(group, int, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}

void InitializeBattleDelayedProjectileEffect(void *group) asm("func_080D37B8");

void InitializeBattleDelayedProjectileEffect(void *group) {
    BATTLE_ANIMATION_FIELD(group, u32, state) = 0;
}
