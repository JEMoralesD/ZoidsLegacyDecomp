#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern s16 Sin256(s16) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern s32 IsBattleAnimationSpriteAtFacingPosition(void *, s32, s32) asm("func_080D2754");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFourSpriteImpactTrailEffect(void *group) asm("func_080D3968");

void UpdateBattleFourSpriteImpactTrailEffect(void *group) {
    char *owner = group;
    register s32 *state_slot asm("r4") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));

    if (*state_slot == 0) {
        void *created = CreateBattleAnimationSprite(owner, 0, 0,
            (s16)(*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x)) - 0x100),
            *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y)), 0x110, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        BATTLE_SPRITE_FIELD(created, s32, user_data.horizontal_projectile.speed) = 0x10;
        PlayBattleAnimationSound(0);
        *state_slot = *state_slot + 1;
    }

    {
        register u32 current_state asm("r0") = *state_slot;
    if (current_state <= 5) {
        if (current_state == 1 &&
                (IsBattleAnimationSpriteAtFacingPosition(*(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])),
                    *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x)) - 0x30,
                    *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y))) << 24) != 0) {
            SetBattleAnimationCameraMode(6, 0);
            PlayBattleAnimationSound(1);
            *state_slot = *state_slot + 1;
        }
        {
            s32 *saved_state;
            register s32 *state_view asm("r0") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
            register u32 current asm("r0");
            asm volatile("" : "=m"(saved_state));
            saved_state = state_view;
            current = *state_view;

            if (current > 1) {
                register s32 step asm("sl") = (u8)(current - 2);
                register s32 scaled asm("r6");
                register s32 scale_byte asm("r9");
                register s32 x_value asm("r5");
                register s32 random asm("r8");
                register s32 y_value asm("r4");
                register s32 phase asm("r0");
                s32 trig;
                void *created;

                trig = Sin256((s16)DivideSigned32(step << 7, 3));
                if (trig < 0) {
                    trig += 0xF;
                }
                scaled = trig >> 4;
                scaled <<= 24;
                {
                    register s32 scale_init asm("r1") =
                        (u32)scaled >> 24;
                    asm volatile("" : "+r"(scale_init));
                    scale_byte = scale_init;
                }
                x_value = *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x));
                {
                    register s32 step_view asm("r1") = step;
                    register s32 offset asm("r0");
                    asm volatile(".short 0x0048, 0x4450, 0x0080"
                                 : "=r"(offset) : "r"(step_view));
                    x_value += offset;
                }
                x_value = (u16)x_value;
                random = CallFunctionR0(gRandomNumberCallback);
                y_value = *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y));
                phase = (u8)ModuloUnsigned32(step, 3);
                phase *= scale_byte;
                asm volatile("" : "+r"(phase));
                phase >>= 1;
                y_value += phase;
                y_value -= (u32)scaled >> 25;
                {
                    register s32 perturb asm("r0");
                    register s32 random_view asm("r1") = random;
                    asm volatile("" : "+r"(random_view));
                    perturb = random_view << 3;
                    asm volatile("add %0, %1"
                                 : "+r"(perturb) : "r"(random));
                    perturb = (u32)perturb >> 15;
                    perturb += 0xFFFC;
                    y_value += perturb;
                }
                x_value = (s16)x_value;
                y_value = (s16)y_value;
                created = CreateBattleAnimationSprite(owner, 1, 0,
                    x_value, y_value, 0, 0, 0);
                {
                    register s32 offset asm("r2") = step + 1;
                    register char *slot asm("r1");
                    asm volatile("lsl %0, %0, #2" : "+r"(offset));
                    slot = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    asm volatile("add %0, %1" : "+r"(slot) : "r"(offset));
                    *(void **)slot = created;
                }
                {
                    register s32 *saved_view asm("r1") = saved_state;
                    register s32 saved_value asm("r0");
                    asm volatile(".short 0x6808, 0x3001, 0x6008"
                                 : "=r"(saved_value)
                                 : "r"(saved_view) : "memory");
                }
            }
        }
    } else {
        u8 index = 0;
        if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *children asm("r2") = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                index = (u8)(index + 1);
            } while (index <= 4 &&
                *(s32 *)(children + (index << 2)) == 0);
        }
        if (index == 5) {
            DestroySpriteGroup(owner);
        }
    }
    }
}
