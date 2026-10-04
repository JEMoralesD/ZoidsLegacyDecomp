#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u16 gBattleBlendControl asm("D_0300004E");
extern u16 gBattleBlendAlpha asm("D_03000050");

void UpdateBattleLightningFadeEffect(struct BattleAnimationGroup *group) asm("func_080DCCB0");

void UpdateBattleLightningFadeEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r5") = group;
    register s32 *effect_state_slot asm("r6") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state_slot;

    switch (phase) {
    case BATTLE_LIGHTNING_FADE_CREATE: {
        register s32 zero asm("r4");
        void *effect_sprite;

        effect_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - 0x80),
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), 0x20,
            ({ zero = 0; zero; }), zero);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
        gBattleBlendControl = 0x2044;
        gBattleBlendAlpha = 0x1004;
        *(u16 *)0x05000000 = zero;
        PlayBattleAnimationSound(0);
        *effect_state_slot = *effect_state_slot + 1;
        break;
    }
    case BATTLE_LIGHTNING_FADE_BLEND_BACKDROP: {
        register s32 *progress_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_fade.backdrop_blend_progress));
        register s32 previous_progress asm("r0") = *progress_slot;
        register s32 progress asm("r3") = previous_progress + 2;

        *progress_slot = progress;
        {
            register u16 *blend_alpha_view asm("r2") = &gBattleBlendAlpha;
            *blend_alpha_view = (previous_progress + 6) | 0x1000;
        }
        if (progress == BATTLE_LIGHTNING_FADE_HIDE_BLEND_PROGRESS) {
            s32 *sprite_flags = *(s32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            *sprite_flags |= BATTLE_SPRITE_HIDDEN;
        }
        if (*progress_slot == BATTLE_LIGHTNING_FADE_END_BLEND_PROGRESS) {
            *effect_state_slot = *effect_state_slot + 1;
        }
        break;
    }
    case BATTLE_LIGHTNING_FADE_START_SPRITE_FADE: {
        gBattleBlendControl = 0x740;
        gBattleBlendAlpha = 0x1010;
        {
            s32 *sprite_flags = *(s32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            *sprite_flags = (*sprite_flags & ~BATTLE_SPRITE_HIDDEN) | BATTLE_SPRITE_SEMITRANSPARENT;
        }
        *effect_state_slot = *effect_state_slot + 1;
        break;
    }
    case BATTLE_LIGHTNING_FADE_WAIT_FOR_FADE: {
        s32 *progress_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_fade.fade_updates));
        s32 progress = *progress_slot + 1;
        *progress_slot = progress;
        gBattleBlendAlpha = (16 - ((u32)progress >> 1)) | 0x1000;
        if (progress == BATTLE_LIGHTNING_FADE_UPDATES) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
