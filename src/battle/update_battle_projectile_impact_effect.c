#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
extern s32 IsBattleAnimationSpriteAtFacingPosition(void *, s32, s32) asm("func_080D2754");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern void DestroySprite(void *) asm("func_08094554");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");

void UpdateBattleProjectileImpactEffect(void *group) asm("func_080D342C");

void UpdateBattleProjectileImpactEffect(void *group) {
    u32 *state = (u32 *)((u8 *)group + 0x8C);
    s32 sound_slot;
    void *projectile;
    switch (*state) {
    case BATTLE_PROJECTILE_IMPACT_LAUNCH:
        {
        s32 launch_x = BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) + 0xFFFFFF00;
        projectile = CreateBattleAnimationSprite(group, 0, 0,
            launch_x,
            BATTLE_ANIMATION_FIELD(group, s16, effect.impact.y),
            0x410, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        }
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, s32, user_data.horizontal_projectile.speed) = 0x10;
        sound_slot = 0;
        goto play_phase_sound;
    case BATTLE_PROJECTILE_IMPACT_WAIT:
        {
        register s32 *impact_x asm("r5");
        register s32 *impact_y asm("r6");
        if ((IsBattleAnimationSpriteAtFacingPosition(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]),
                *(impact_x = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.impact.x))) - 16,
                *(impact_y = (s32 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.impact.y)))) << 24) != 0) {
            DestroySprite(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]));
            BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 1, 0,
                *(s16 *)impact_x, *(s16 *)impact_y, 0, 0, 0);
            SetBattleAnimationCameraMode(6, 0);
            sound_slot = 1;
            goto play_phase_sound;
        }
        }
        return;
    play_phase_sound:
        PlayBattleAnimationSound(sound_slot);
        *state += 1;
        return;
    case BATTLE_PROJECTILE_IMPACT_FINISH:
        if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0) {
            DestroySpriteGroup(group);
        }
        break;
    }
}
