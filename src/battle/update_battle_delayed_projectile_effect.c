#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleDelayedProjectileEffect(void *group) asm("func_080D37C0");

void UpdateBattleDelayedProjectileEffect(void *group) {
    /* The native word load compares both animation step and frame timer. */
    switch (BATTLE_ANIMATION_FIELD(group, u32, state)) {
    case BATTLE_DELAYED_PROJECTILE_SHOW_LAUNCH_FLASH:
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0, BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y), 0x400, 0, 0);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, u32, state) += 1;
        break;
    case BATTLE_DELAYED_PROJECTILE_CREATE:
        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]), s32, animation_step) == 1) {
            BATTLE_ANIMATION_FIELD(group, void *, sprites[1]) = CreateBattleAnimationSprite(group, 1, 0, BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y), 0x510, 0, 0);
            BATTLE_ANIMATION_FIELD(group, u32, state) += 1;
        }
        break;
    case BATTLE_DELAYED_PROJECTILE_START_MOTION:
        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]), s32, animation_step) == 2) {
            void *projectile = BATTLE_ANIMATION_FIELD(group, void *, sprites[1]);
            BATTLE_SPRITE_FIELD(projectile, s32, update_callback) = (s32)UpdateBattleHorizontalProjectileSprite;
            BATTLE_SPRITE_FIELD(projectile, s32, user_data.horizontal_projectile.speed) = 0x10;
            BATTLE_ANIMATION_FIELD(group, u32, state) += 1;
        }
        break;
    case BATTLE_DELAYED_PROJECTILE_FINISH:
        if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0 && BATTLE_ANIMATION_FIELD(group, void *, sprites[1]) == 0) {
            DestroySpriteGroup(group);
        }
        break;
    }
}
