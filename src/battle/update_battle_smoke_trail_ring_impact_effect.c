#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleSmokeTrailRingImpactEffect(void *group) asm("func_080D558C");

void UpdateBattleSmokeTrailRingImpactEffect(void *group)
{
    register char *owner asm("r4") = group;
    register u32 *state asm("r5") = (u32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register u32 state_value asm("r0") = *state;

    if (state_value <= 7U) {
        if (state_value == 0) {
            void *child;

            child = CreateBattleAnimationSprite(owner, 1, 0,
                (s16)(*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(x)) - 0x100),
                (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)), 0x10, (s32)UpdateBattleHorizontalProjectileSprite, 1);
            *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = child;
            *(s32 *)((u8 *)child + 0x28) = 0x20;
            PlayBattleAnimationSound(0);
        }

        {
            void *child;
            register u32 next asm("r2");
            register u32 offset asm("r3");
            register void **slot asm("r1");

            child = CreateBattleAnimationSprite(owner, 0, 0,
                (s16)(*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(x)) + 0xFF00 + (*state << 5)),
                (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_SEMITRANSPARENT, 0, 1);
            next = *state;
            next += 1;
            asm volatile("" : "+r"(next));
            offset = next << 2;
            asm volatile("" : "+r"(offset));
            slot = (void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
            asm volatile("" : "+r"(slot));
            slot = (void **)((u8 *)slot + offset);
            *slot = child;
            *state = next;
        }
        return;
    }

    if (state_value == BATTLE_SMOKE_TRAIL_START_IMPACT) {
        *(void **)(owner + 0x30) = CreateBattleAnimationSprite(owner, 2, 0,
            *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
            BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
        SetBattleAnimationCameraMode(6, 0);
        PlayBattleAnimationSound(1);
        *state = *state + 1;
        return;
    }

    /* Native cleanup checks slots 0–8; the impact sprite occupies slot 9. */
    if (state_value == BATTLE_SMOKE_TRAIL_WAIT_IMPACT) {
        register u32 i asm("r1") = 0;

        if (*(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register void **slots asm("r2") = (void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));

            do {
                register u32 next asm("r0") = i + 1;

                next <<= 24;
                i = next >> 24;
                if (i > 8U) {
                    break;
                }
                {
                    register u32 offset asm("r0") = i << 2;
                    register void **slot asm("r0");

                    slot = (void **)((u32)slots + offset);
                    asm volatile("" : "+r"(slot));
                    if (*slot != 0) {
                        break;
                    }
                }
            } while (1);
        }
        if (i == 9) {
            DestroySpriteGroup(owner);
        }
    }
}
