#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleThinBeamRayImpactEffect(struct BattleAnimationGroup *group) asm("func_080DDC50");

void UpdateBattleThinBeamRayImpactEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *effect_state_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r2") = *effect_state_slot;

    switch (phase) {
    case BATTLE_THIN_BEAM_IMPACT_CREATE_BEAM:
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 0, 1,
            (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - 0x80), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
            (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2), 0, 1);
        goto advance;
    case BATTLE_THIN_BEAM_IMPACT_WAIT_FOR_RAY_STEP:
        if (*(u16 *)(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) + BATTLE_SPRITE_OFFSET(animation_step)) != BATTLE_THIN_BEAM_IMPACT_RAY_ANIMATION_STEP) {
            break;
        }
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(group_bytes, 1, 0,
            *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
        goto advance;
    case BATTLE_THIN_BEAM_IMPACT_WAIT_FOR_IMPACT_STEP:
        if (*(u16 *)(*(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) + BATTLE_SPRITE_OFFSET(animation_step)) != BATTLE_THIN_BEAM_IMPACT_TRIGGER_ANIMATION_STEP) {
            break;
        }
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(0);
        goto advance;
advance:
        *effect_state_slot = *effect_state_slot + 1;
        break;
    case BATTLE_THIN_BEAM_IMPACT_EMIT_PARTICLES_AND_WAIT: {
        register void *beam_sprite asm("r0") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 beam_animation_step asm("r1") =
            *(u16 *)((char *)beam_sprite + BATTLE_SPRITE_OFFSET(animation_step));

        if (beam_animation_step <= BATTLE_THIN_BEAM_IMPACT_LAST_PARTICLE_ANIMATION_STEP) {
            register s32 *particle_updates_slot asm("r9") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.thin_beam_impact.particle_updates));
            register s32 emission_mask asm("r6") = *particle_updates_slot & phase;

            if (emission_mask == 0) {
                register u32 *random_callback_slot asm("r5") = &gRandomNumberCallback;
                register s32 minimum_speed_fixed8 asm("r8");
                register s32 particle_flags asm("sl");
                register s32 particle_angle asm("r4");
                register s32 speed_fixed8 asm("r1");

                particle_angle = (u32)(CallFunctionR0(*random_callback_slot) * 0x21) >> 15;
                particle_angle += 0x80;
                speed_fixed8 = (u32)(CallFunctionR0(*random_callback_slot) * 0x101) >> 15;
                minimum_speed_fixed8 = 0x100;
                asm volatile("" : "+r"(minimum_speed_fixed8));
                speed_fixed8 += minimum_speed_fixed8;
                {
                    register volatile s32 *outgoing asm("sp");
                    register s32 x_offset asm("r0");
                    register s32 x_value asm("r3");
                    register s32 y_value asm("r0");
                    register char *call0 asm("r0");
                    register s32 call1 asm("r1");
                    register s32 call2 asm("r2");

                    asm volatile(
                        "mov %0, #4\n\t"
                        "ldrsh %1, [%2, %0]"
                        : "=r"(x_offset), "=r"(x_value)
                        : "r"(group_bytes));
                    y_value = (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) - 4);
                    outgoing[0] = y_value;
                    particle_flags = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
                    outgoing[1] = particle_flags;
                    outgoing[2] = particle_angle;
                    outgoing[3] = speed_fixed8;
                    outgoing[4] = emission_mask;
                    call0 = group_bytes;
                    call1 = 2;
                    call2 = 0;
                    asm volatile("" : "+r"(call0), "+r"(call1),
                                       "+r"(call2), "+r"(x_value));
                    CreateBattleAngledProjectileSprite(call0, call1, call2, x_value);
                }

                particle_angle = (u32)(CallFunctionR0(*random_callback_slot) * 0x21) >> 15;
                particle_angle += 0x60;
                speed_fixed8 = (u32)(CallFunctionR0(*random_callback_slot) * 0x101) >> 15;
                speed_fixed8 += minimum_speed_fixed8;
                CreateBattleAngledProjectileSprite(group_bytes, 2, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
                    (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) + 4),
                    particle_flags, particle_angle, speed_fixed8, emission_mask);
            }
            {
                register s32 *particle_updates_view asm("r1") = particle_updates_slot;
                *particle_updates_view = *particle_updates_view + 1;
            }
            beam_sprite = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        }

        {
            u8 sprite_index = 0;
            if (beam_sprite == 0) {
                register char *sprite_slots asm("r2") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    sprite_index = (u8)(sprite_index + 1);
                } while (sprite_index <= 0xA &&
                    *(s32 *)(sprite_slots + (sprite_index << 2)) == 0);
            }
            if (sprite_index == 0xB) {
                DestroySpriteGroup(group_bytes);
            }
        }
        break;
    }
    }
    return;
}
