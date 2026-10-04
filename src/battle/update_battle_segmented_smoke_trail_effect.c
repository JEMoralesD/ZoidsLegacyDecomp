#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleSegmentedSmokeTrailEffect(void *group) asm("func_080D4C84");

void UpdateBattleSegmentedSmokeTrailEffect(void *group)
{
    register char *owner asm("r5") = group;
    u32 *state_slot = (u32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r1") = *state_slot;

    if (state <= 7U) {
        register u32 *saved_state;
        register s32 *children asm("r6");

        saved_state = state_slot;
        children = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
        if (state == 0) {
            register s32 index asm("r4");
            void *created;

            created = CreateBattleAnimationSprite(owner, 1, 0,
                *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
                0x10, (s32)UpdateBattleHorizontalProjectileSprite, state);
            *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
            *(s32 *)((u8 *)created + 0x28) = 0x20;

            index = 0;
            do {
                u32 random;
                register s32 speed_fixed8 asm("r1");
                void *spawned;

                random = CallFunctionR0(gRandomNumberCallback);
                speed_fixed8 = (random * 0x41) >> 15;
                speed_fixed8 += 0xC0;
                spawned = CreateBattleAngledProjectileSprite(owner, 2, 0,
                    *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
                    BATTLE_SPRITE_SEMITRANSPARENT, (index << 2) - 0xA, speed_fixed8, 0);
                *(void **)((u8 *)children + ((index + 9) << 2)) = spawned;
                {
                    register u32 next asm("r0") = index + 1;

                    next <<= 24;
                    index = next >> 24;
                }
            } while ((u32)index <= 5);
            PlayBattleAnimationSound(0);
            SetBattleAnimationCameraMode(5, 0);
        }

        {
            void *child;
            register u32 next asm("r2");
            register u32 offset asm("r1");

            child = CreateBattleAnimationSprite(owner, 0, 0,
                (s16)(*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(x)) - (*saved_state << 5)),
                (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
            next = *saved_state;
            next += 1;
            offset = next << 2;
            *(void **)((u8 *)children + offset) = child;
            *saved_state = next;
        }
        return;
    }

    {
        register u32 i asm("r4") = 0;

        if (*(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register void **slots asm("r1") = (void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));

            do {
                register u32 next asm("r0") = i + 1;

                next <<= 24;
                i = next >> 24;
                if (i > 14U) {
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
        if (i == 15) {
            DestroySpriteGroup(owner);
        }
    }
}
