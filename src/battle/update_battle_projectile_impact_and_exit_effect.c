#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(s32) asm("func_080ECD5C");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite(void *, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_080D2660");
void UpdateBattleProjectileImpactAndExitEffect(void *group) asm("func_080D2B9C");

void UpdateBattleProjectileImpactAndExitEffect(void *group) {
    u32 *state = (u32 *)((u8 *)group + 0x8C);
    switch (*state) {
    case BATTLE_PROJECTILE_IMPACT_LAUNCH: {
        register void *projectile asm("r0");
        projectile = CreateBattleAnimationSprite(group, 0, 0, (s16)(BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) - 0x100), BATTLE_ANIMATION_FIELD(group, s16, effect.impact.y), 0x100, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, s32, user_data.horizontal_projectile.speed) = 0x10;
        PlayBattleAnimationSound(0);
        *state += 1;
        return;
    }
    case BATTLE_PROJECTILE_IMPACT_WAIT: {
        register void *projectile asm("r0");
        u32 exit_angle;
        if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) != 0) return;
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 1, 0, BATTLE_ANIMATION_FIELD(group, s16, effect.impact.x), BATTLE_ANIMATION_FIELD(group, s16, effect.impact.y), 0, 0, 0);
        exit_angle = ((CallFunctionR0(*(s32 *)0x03000010) * 0x41) >> 15) - 0x20;
        projectile = CreateBattleAngledProjectileSprite(group, 2, 0, BATTLE_ANIMATION_FIELD(group, s16, effect.impact.x), BATTLE_ANIMATION_FIELD(group, s16, effect.impact.y), 0x100, exit_angle, 0x800, 0);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[1]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, s8, rotation) = exit_angle;
        SetBattleAnimationCameraMode(6, 0);
        PlayBattleAnimationSound(1);
        *state += 1;
        return;
    }
    case BATTLE_PROJECTILE_IMPACT_FINISH:
        if (!BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) && !BATTLE_ANIMATION_FIELD(group, void *, sprites[1])) DestroySpriteGroup(group);
    }
}
