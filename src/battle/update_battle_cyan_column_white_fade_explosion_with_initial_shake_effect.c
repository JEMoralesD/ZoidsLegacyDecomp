#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
#define gBattleSelectedZoidBodySprite (*(u32 **)0x02033F50)

extern void DestroySprite(void *) asm("func_08094554");
extern void SetSpriteAnimation(void *, s32) asm("func_08094564");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");
extern u16 gBattleBlendControl asm("D_0300004E");
extern u16 gBattleBlendAlpha asm("D_03000050");
extern u16 gBattleBlendBrightness asm("D_03000052");

void UpdateBattleCyanColumnWhiteFadeExplosionWithInitialShakeEffect(struct BattleAnimationGroup *group) asm("func_080DFB64");

void UpdateBattleCyanColumnWhiteFadeExplosionWithInitialShakeEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    s32 *effect_state_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 *phase_advance_slot asm("r1");
    u32 phase;

    if ((u32)*effect_state_slot <= BATTLE_CYAN_COLUMN_WHITE_FADE_START_COLUMN_LOOP) {
        u32 particle_angle = CallFunctionR0(gRandomNumberCallback) >> 7;
        u32 speed_fixed8 =
            ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x101) >> 15) + 0x200;

        CreateBattleAngledProjectileSprite(group_bytes, 1, 0, 0x78, 0x40, BATTLE_SPRITE_SEMITRANSPARENT,
            particle_angle, speed_fixed8, 2);
    }

    phase = *effect_state_slot;
    switch (phase) {
    case BATTLE_CYAN_COLUMN_WHITE_FADE_CREATE_STREAK:
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) =
            CreateBattleAnimationSprite(group_bytes, 0, 0, 0x78, 0x40, (BATTLE_SPRITE_LOOP_ANIMATION | BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 2);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(0);
        goto select_state_slot;

    case BATTLE_CYAN_COLUMN_WHITE_FADE_CREATE_COLUMNS:
        if ((**(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_ANIMATION_FINISHED) == 0) {
            return;
        }
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) =
            CreateBattleAnimationSprite(group_bytes, 2, 0, 0x78, 0x80, (BATTLE_SPRITE_HOLD_LAST_FRAME | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 2);
        PlayBattleAnimationSound(1);
        goto select_state_slot;

    case BATTLE_CYAN_COLUMN_WHITE_FADE_START_COLUMN_LOOP: {
        u32 *primary_sprite = *(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1]));

        if ((*primary_sprite & BATTLE_SPRITE_ANIMATION_FINISHED) == 0) {
            return;
        }
        SetSpriteAnimation(primary_sprite, 1);
        primary_sprite = *(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1]));
        *primary_sprite = (*primary_sprite & ~(BATTLE_SPRITE_HOLD_LAST_FRAME | BATTLE_SPRITE_LOOP_ANIMATION)) | BATTLE_SPRITE_LOOP_ANIMATION;
        DestroySprite(*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])));
        phase_advance_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_column_white_fade.phase_updates));
        *phase_advance_slot = 0;
        phase_advance_slot -= 1;
        goto increment_selected;
    }

    case BATTLE_CYAN_COLUMN_WHITE_FADE_WAIT_BEFORE_FADE: {
        s32 *phase_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_column_white_fade.phase_updates));
        s32 phase_updates = *phase_updates_slot + 1;

        *phase_updates_slot = phase_updates;
        if (phase_updates != BATTLE_CYAN_COLUMN_WHITE_FADE_DELAY_UPDATES) {
            return;
        }
        gBattleBlendControl = 0xD95;
        gBattleBlendAlpha = 0x10;
        gBattleBlendBrightness = 0;
        *gBattleSelectedZoidBodySprite |= BATTLE_SPRITE_SEMITRANSPARENT;
        *phase_updates_slot = 0;
        goto select_state_slot;
    }

    case BATTLE_CYAN_COLUMN_WHITE_FADE_INCREASE_BRIGHTNESS: {
        s32 *phase_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_column_white_fade.phase_updates));
        s32 phase_updates = *phase_updates_slot + 1;

        *phase_updates_slot = phase_updates;
        gBattleBlendBrightness = (u16)((u32)phase_updates >> 2);
        if (phase_updates != BATTLE_CYAN_COLUMN_WHITE_FADE_BRIGHTNESS_UPDATES) {
            return;
        }
        DestroySprite(*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])));
        *phase_updates_slot = 0;
        goto select_state_slot;
    }

select_state_slot:
        phase_advance_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
increment_selected:
        *phase_advance_slot = *phase_advance_slot + 1;
        return;

    case BATTLE_CYAN_COLUMN_WHITE_FADE_EMIT_EXPLOSIONS: {
        s32 explosion_x =
            (s32)(CallFunctionR0(gRandomNumberCallback) * 0x1E0) >> 16;
        s32 explosion_y =
            (s32)(CallFunctionR0(gRandomNumberCallback) << 8) >> 16;
        s32 *phase_updates_slot;
        s32 phase_updates;

        CreateBattleAnimationSprite(group_bytes, 3, 0, explosion_x, explosion_y, BATTLE_SPRITE_SEMITRANSPARENT, 0, 2);
        phase_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_column_white_fade.phase_updates));
        if ((*phase_updates_slot & 7) == 0) {
            PlayBattleAnimationSound(2);
        }
        phase_updates = *phase_updates_slot + 1;
        *phase_updates_slot = phase_updates;
        if (phase_updates == BATTLE_CYAN_COLUMN_WHITE_FADE_EXPLOSION_UPDATES) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
    return;
}
