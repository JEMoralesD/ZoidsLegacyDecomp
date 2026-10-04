#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gBattleZoidScrollX asm("D_02034034");
extern struct SpriteBackgroundScrollOffsets gSpriteBackgroundScroll[] asm("D_03000054");
extern int CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");
extern void SetBattleAnimationCameraMode() asm("func_080D12A0");
extern void DestroySpriteGroup() asm("func_08095114");

void UpdateBattleCyanBeamScrollEffect(struct BattleAnimationGroup *group) asm("func_080DB8CC");

void UpdateBattleCyanBeamScrollEffect(struct BattleAnimationGroup *group) {
    u32 *effect_state = (u32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
    s32 sound_slot;
    s32 group_x, camera_x_offset;
    switch (*effect_state) {
    case BATTLE_CYAN_BEAM_SCROLL_CREATE:
        group_x = BATTLE_ANIMATION_FIELD(group, s32, x);
        camera_x_offset = gBattleZoidScrollX / 256 + 0x40;
        BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) = CreateBattleAnimationSprite(group, 0, 0,
            (s16)(group_x + camera_x_offset),
            /* The original adds the raw Q8 scroll value here. */
            (s16)(BATTLE_ANIMATION_FIELD(group, s32, y) + gSpriteBackgroundScroll[0].y_fixed8),
            (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_SEMITRANSPARENT), 0, 0);
        sound_slot = 0;
        goto shared;
    case BATTLE_CYAN_BEAM_SCROLL_WAIT_FOR_CAMERA_STEP:
        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, s32, sprites[0]), u16, animation_step) == BATTLE_CYAN_BEAM_SCROLL_CAMERA_ANIMATION_STEP) {
            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_EXTENDED_SCROLL_SWEEP, 0);
            sound_slot = 1;
        shared:
            PlayBattleAnimationSound(sound_slot);
            *effect_state = *effect_state + 1;
        }
        break;
    case BATTLE_CYAN_BEAM_SCROLL_WAIT_FOR_SPRITES:
        if (BATTLE_ANIMATION_FIELD(group, s32, sprites[0]) == 0) {
            DestroySpriteGroup(group);
        }
        break;
    }
}

void ToggleBattleShieldSpriteVisibility(struct BattleDisplaySprite *sprite) asm("func_080DB964");

void ToggleBattleShieldSpriteVisibility(struct BattleDisplaySprite *sprite) {
    BATTLE_SPRITE_FIELD(sprite, s32, flags) = BATTLE_SPRITE_FIELD(sprite, s32, flags) ^ BATTLE_SPRITE_HIDDEN;
}

void InitializeBattleFlickeringShieldEffect(struct BattleAnimationGroup *group) asm("func_080DB970");

void InitializeBattleFlickeringShieldEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, s32, state) = 0;
}

void UpdateBattleFlickeringShieldEffect(struct BattleAnimationGroup *group) asm("func_080DB978");

void UpdateBattleFlickeringShieldEffect(struct BattleAnimationGroup *group) {
    s32 phase;
    phase = BATTLE_ANIMATION_FIELD(group, s32, state);
    if (phase == BATTLE_SHIELD_EFFECT_CREATE) {
        *(s32 *)0x02033F4C = CreateBattleAnimationSprite(group, 0, 0,
            BATTLE_ANIMATION_FIELD(group, s16, x), BATTLE_ANIMATION_FIELD(group, s16, y),
            BATTLE_SPRITE_HOLD_LAST_FRAME, ToggleBattleShieldSpriteVisibility, phase);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = BATTLE_ANIMATION_FIELD(group, s32, state) + 1;
        return;
    }
    if (BATTLE_SPRITE_FIELD(*(s32 **)0x02033F4C, s32, flags) & BATTLE_SPRITE_ANIMATION_FINISHED) {
        BATTLE_ANIMATION_FIELD(group, s32, flags) = BATTLE_ANIMATION_FIELD(group, s32, flags) & ~BATTLE_ANIMATION_GROUP_ACTIVE;
    }
}
