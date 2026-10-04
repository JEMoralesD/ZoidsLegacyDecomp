#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattlePurplePairedSlashSparkAndFragmentEffect(struct BattleAnimationGroup *group) asm("func_080DB5DC");

void UpdateBattlePurplePairedSlashSparkAndFragmentEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r6") = (char *)group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state;
    s32 sprite_index;

    switch (phase) {
    case BATTLE_PAIRED_SLASH_CREATE: {
        void *slash_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
            *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
            0, 0, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = slash_sprite;
        *effect_state = *effect_state + 1;
        break;
    }
    case BATTLE_PAIRED_SLASH_WAIT_FOR_SPARK_STEP: {
        if (BATTLE_SPRITE_FIELD(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])), u16, animation_step) == BATTLE_PAIRED_SLASH_SPARK_ANIMATION_STEP) {
            register u32 *random_callback_slot asm("r8");
            register s32 mirroring_flags asm("r10");
            register s32 *sprite_slots asm("r9");
            s32 * volatile saved_sprite_slots;
            s32 * volatile saved_effect_state;
            register s32 *sprite_slots_init asm("r2");

            sprite_index = 0;
            saved_effect_state = effect_state;
            sprite_slots_init = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            saved_sprite_slots = sprite_slots_init;
            {
                register u32 *random_callback_init asm("r0") = &gRandomNumberCallback;
                random_callback_slot = random_callback_init;
            }
            {
                register s32 mirroring_flags_init asm("r1") = 0;
                __asm__ volatile ("" : "+r" (mirroring_flags_init));
                mirroring_flags = mirroring_flags_init;
            }
            sprite_slots = sprite_slots_init;
            do {
                register s32 random asm("r0");
                register s32 sprite_angle asm("r4");
                register s32 next_sprite_index_bits asm("r5");
                register s32 sprite_offset asm("r1");
                register s32 speed_fixed8 asm("r1");
                register s32 minimum_speed_fixed8_r2 asm("r2");
                void *effect_sprite;

                {
                    register u32 *random_callback_view asm("r2") = random_callback_slot;
                    random = CallFunctionR0(*random_callback_view);
                }
                sprite_angle = random << 1;
                sprite_angle += random;
                sprite_angle <<= 4;
                sprite_angle += random;
                sprite_angle = (u32)sprite_angle >> 15;
                sprite_angle += 168;
                {
                    register u32 *random_callback_view asm("r1") = random_callback_slot;
                    speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x201) >> 15;
                }
                minimum_speed_fixed8_r2 = 0x200;
                speed_fixed8 += minimum_speed_fixed8_r2;
                {
                    register s32 spawn_x asm("r3");
                    spawn_x = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
                    __asm__ volatile ("" : "+r" (minimum_speed_fixed8_r2));
                    effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                        spawn_x, (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                        ({
                            register s32 flags asm("r0") = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
                            flags;
                        }),
                        sprite_angle, speed_fixed8,
                        ({
                            register s32 mirroring_flags_view asm("r1") = mirroring_flags;
                            __asm__ volatile ("" : "+r" (mirroring_flags_view));
                            mirroring_flags_view;
                        }));
                }
                next_sprite_index_bits = sprite_index + 1;
                sprite_offset = next_sprite_index_bits << 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;

                {
                    register u32 *random_callback_view asm("r2") = random_callback_slot;
                    random = CallFunctionR0(*random_callback_view);
                }
                sprite_angle = random << 1;
                sprite_angle += random;
                sprite_angle <<= 4;
                sprite_angle += random;
                sprite_angle = (u32)sprite_angle >> 15;
                sprite_angle += 40;
                {
                    register u32 *random_callback_view asm("r1") = random_callback_slot;
                    speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x201) >> 15;
                }
                {
                    register s32 minimum_speed_fixed8 asm("r2") = 0x200;
                    __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                    speed_fixed8 += minimum_speed_fixed8;
                }
                effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0,
                    *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                    ({
                        register s32 flags asm("r0") = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
                        flags;
                    }),
                    sprite_angle, speed_fixed8,
                    ({
                        register s32 mirroring_flags_view asm("r1") = mirroring_flags;
                        __asm__ volatile ("" : "+r" (mirroring_flags_view));
                        mirroring_flags_view;
                    }));
                sprite_offset = sprite_index + 9;
                sprite_offset <<= 2;
                *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)effect_sprite;
                next_sprite_index_bits <<= 24;
                sprite_index = (u32)next_sprite_index_bits >> 24;
            } while ((u32)sprite_index <= 7);

            sprite_index = 0;
            {
                register u32 *random_callback_init asm("r2") = &gRandomNumberCallback;
                __asm__ volatile ("" : "+r" (random_callback_init));
                random_callback_slot = random_callback_init;
            }
            do {
                register s32 angle_random asm("r4");
                register s32 sprite_angle asm("r5");
                register s32 angle_jitter asm("r0");
                register s32 minimum_speed_fixed8 asm("r0");
                register s32 sprite_offset asm("r1");
                s32 speed_fixed8;
                void *effect_sprite;

                {
                    register u32 *random_callback_view asm("r1") = random_callback_slot;
                    angle_random = CallFunctionR0(*random_callback_view);
                }
                sprite_angle = DivideSigned32(sprite_index << 6, 7);
                angle_jitter = (u32)(angle_random * 9) >> 15;
                angle_jitter -= 36;
                sprite_angle += angle_jitter;
                {
                    register u32 *random_callback_view asm("r2") = random_callback_slot;
                    speed_fixed8 = (CallFunctionR0(*random_callback_view) * 0x201) >> 15;
                }
                minimum_speed_fixed8 = 0x200;
                speed_fixed8 += minimum_speed_fixed8;
                {
                    register s32 spawn_x asm("r3");
                    spawn_x = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
                    effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0,
                        spawn_x, (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                        BATTLE_SPRITE_LOOP_ANIMATION, sprite_angle, speed_fixed8, 0);
                }
                sprite_offset = sprite_index + 17;
                sprite_offset <<= 2;
                {
                    register s32 *sprite_slots_view asm("r2") = saved_sprite_slots;
                    *(s32 *)((char *)sprite_slots_view + sprite_offset) = (s32)effect_sprite;
                }
                sprite_index = (u8)(sprite_index + 1);
            } while ((u32)sprite_index <= 7);

            SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE_AND_RECOIL_SCROLL, 0);
            PlayBattleAnimationSound(0);
            {
                register s32 *phase_slot asm("r1") = saved_effect_state;
                *phase_slot = *phase_slot + 1;
            }
        }
        break;
    }
    case BATTLE_PAIRED_SLASH_WAIT_FOR_SPRITES: {
        sprite_index = 0;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            s32 sprite_offset;
            do {
                sprite_index = (u8)(sprite_index + 1);
            } while ((u32)sprite_index <= 0x18 &&
                (sprite_offset = sprite_index << 2,
                 *(s32 *)((char *)sprite_slots + sprite_offset)) == 0);
        }
        if (sprite_index == 0x19) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
