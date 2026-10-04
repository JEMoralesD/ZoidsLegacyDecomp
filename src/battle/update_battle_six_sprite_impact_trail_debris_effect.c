#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
extern s16 Sin256(s16) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern s32 IsBattleAnimationSpriteAtFacingPosition(void *, s32, s32) asm("func_080D2754");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleSixSpriteImpactTrailDebrisEffect(void *group) asm("func_080D3C94");

void UpdateBattleSixSpriteImpactTrailDebrisEffect(void *group) {
    char *owner = group;
    register s32 *state_slot asm("r4") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    s32 outgoing_reserve;
    u32 saved_y;
    char *saved_children;
    s32 *saved_state;

    asm volatile("" : "=m"(outgoing_reserve), "=m"(saved_y),
                           "=m"(saved_children),
                           "=m"(saved_state));

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
    if (current_state <= 7) {
        if (current_state == 1 &&
                (IsBattleAnimationSpriteAtFacingPosition(*(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])),
                    *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x)) - 0x50,
                    *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y))) << 24) != 0) {
            SetBattleAnimationCameraMode(6, 0);
            PlayBattleAnimationSound(1);
            *state_slot = *state_slot + 1;
        }
        {
            register s32 *state_view asm("r0") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
            register u32 current asm("r1");
            current = *state_view;
            saved_state = state_view;

            if (current > 1) {
                register s32 step_init asm("r0") = (u8)(current - 2);
                register s32 step asm("sl") = step_init;
                register s32 scaled asm("r6");
                register s32 scale_byte asm("r9");
                register s32 x_value asm("r5");
                register s32 random asm("r8");
                register s32 y_value asm("r4");
                register s32 phase asm("r0");
                s32 trig;
                void *created;

                trig = Sin256((s16)DivideSigned32(
                    step_init << 7, 5));
                if (trig < 0) {
                    trig += 0xF;
                }
                scaled = trig >> 4;
                scaled <<= 24;
                {
                    register s32 scale_init asm("r2") =
                        (u32)scaled >> 24;
                    asm volatile("" : "+r"(scale_init));
                    scale_byte = scale_init;
                }
                x_value = *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x));
                {
                    register s32 step_view asm("r1") = step;
                    register s32 step_add asm("r0") = step_view << 3;
                    asm volatile("" : "+r"(step_view));
                    x_value += step_add;
                }
                x_value = (u16)x_value;
                {
                    register u32 *rng asm("r2") = &gRandomNumberCallback;
                    random = CallFunctionR0(*rng);
                }
                y_value = *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y));
                phase = (u8)ModuloUnsigned32(step, 3);
                {
                    register s32 product asm("r1") = scale_byte;
                    asm volatile("mul %0, %1"
                        : "+r"(product) : "r"(phase));
                    phase = product;
                }
                asm volatile("" : "+r"(phase));
                phase >>= 1;
                y_value += phase;
                y_value -= (u32)scaled >> 25;
                {
                    register s32 perturb asm("r0");
                    register s32 random_view asm("r2") = random;
                    asm volatile("" : "+r"(random_view));
                    perturb = random_view << 3;
                    asm volatile("add %0, %1"
                                 : "+r"(perturb) : "r"(random));
                    perturb = (u32)perturb >> 15;
                    perturb += 0xFFFC;
                    asm volatile("" : "+r"(random_view));
                    y_value += perturb;
                }
                y_value <<= 16;
                x_value <<= 16;
                {
                    register s32 x_arg asm("r3") = x_value >> 16;
                    register u32 y_bits asm("r2") = (u32)y_value >> 16;
                    saved_y = y_bits;
                    y_value >>= 16;
                    created = CreateBattleAnimationSprite(owner, 1, 0,
                        x_arg, y_value, 0, 0, 0);
                }
                {
                    register s32 offset asm("r2") = step + 1;
                    register s32 grouped asm("r1") = offset << 2;
                    register char *children asm("r3");
                    /* Native slots 32–34 overlap state and impact coordinates; cleanup scans only slots 0–30. */
                    grouped += offset;
                    grouped <<= 2;
                    children = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    *(void **)(children + grouped) = created;
                    {
                        register u32 index asm("r6") = 0;
                        register u32 *rng asm("r9");
                        register s32 debris_group_index asm("r8");

                        saved_children = children;
                        asm volatile("" : "+m"(saved_children));
                        rng = &gRandomNumberCallback;
                        debris_group_index = offset;

                        do {
                            register s32 angle asm("r4");
                            register s32 speed_fixed8 asm("r1");
                            register s32 minimum_speed_fixed8 asm("r0");
                            register s32 slot_index asm("r1");
                            void *spawned;

                            {
                                register u32 *rng_view asm("r1") = rng;
                                angle = (u32)(CallFunctionR0(*rng_view) * 0x41)
                                    >> 15;
                            }
                            angle -= 0x20;
                            {
                                register u32 *rng_view asm("r2") = rng;
                                speed_fixed8 = (u32)(CallFunctionR0(*rng_view) *
                                    0x101) >> 15;
                            }
                            minimum_speed_fixed8 = 0x100;
                            speed_fixed8 += minimum_speed_fixed8;
                            asm volatile("" : "+r"(minimum_speed_fixed8));
                            {
                                register volatile s32 *outgoing asm("sp");
                                register u32 y_bits asm("r2") = saved_y;
                                register s32 y_arg asm("r0");
                                register s32 sprite_flags asm("r0");
                                register s32 zero asm("r0");
                                register char *call0 asm("r0");
                                register s32 call1 asm("r1");
                                register s32 call2 asm("r2");
                                register s32 x_arg asm("r3");

                                y_arg = y_bits << 16;
                                y_arg >>= 16;
                                outgoing[0] = y_arg;
                                sprite_flags = 0x500;
                                outgoing[1] = sprite_flags;
                                outgoing[2] = angle;
                                outgoing[3] = speed_fixed8;
                                zero = 0;
                                outgoing[4] = zero;
                                call0 = owner;
                                call1 = 2;
                                call2 = 0;
                                asm volatile("asr %0, %1, #16"
                                    : "=r"(x_arg) : "r"(x_value));
                                spawned = CreateBattleAngledProjectileSprite(
                                    call0, call1, call2, x_arg);
                            }
                            slot_index = debris_group_index << 2;
                            slot_index += debris_group_index;
                            slot_index += index;
                            slot_index += 1;
                            slot_index <<= 2;
                            {
                                register char *children_view asm("r2") =
                                    saved_children;
                                asm volatile("add %0, %1, %0"
                                    : "+r"(slot_index)
                                    : "r"(children_view));
                                *(void **)slot_index = spawned;
                            }
                            {
                                register u32 next asm("r0") = index + 1;
                                next <<= 24;
                                index = next >> 24;
                            }
                        } while (index <= 3);
                    }
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
        register u32 index asm("r6") = 0;
        if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *children asm("r1") = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register u32 next asm("r0") = index + 1;
                next <<= 24;
                index = next >> 24;
            } while (index <= 0x1E &&
                *(s32 *)(children + (index << 2)) == 0);
        }
        if (index == 0x1F) {
            DestroySpriteGroup(owner);
        }
    }
    }
}
