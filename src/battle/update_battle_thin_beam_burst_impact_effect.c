#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
M2C_UNK SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
s32 CreateBattleAngledProjectileSprite(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");
s32 CallFunctionR0(s32) asm("func_080ECD5C");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");

void UpdateBattleThinBeamBurstImpactEffect(struct BattleAnimationGroup *group, M2C_UNK unused_argument) asm("func_080DD7DC");

void UpdateBattleThinBeamBurstImpactEffect(struct BattleAnimationGroup *group, M2C_UNK unused_argument) {
    volatile s32 saved_sprite_flags;
    volatile s32 fragment_fan_angle;
    volatile s32 fragment_angle;
    s32 *step_particle_updates_slot;
    s32 frame_gap0;
    s32 frame_gap1;
    s32 burst_y;
    s32 frame_gap2;
    s32 *burst_updates_slot;
    s32 *sprite_flags_slot;
    s32 burst_y_value;
    s32 burst_x;
    s32 *state_or_sprite_slots;
    s32 next_burst_update;
    register void *burst_sprite asm("r0");
    s32 emission_mask;
    register s32 burst_slot_offset asm("r1");
    s32 upper_particle_angle;
    s32 lower_particle_angle;
    u16 burst_x_bits;
    u32 phase;
    register s32 inherited_sprite_slot_index asm("r6");
    register void *burst_sprite_slot_address asm("r8");
    register u32 state_address_or_emission_parity asm("sl");

    phase = BATTLE_ANIMATION_FIELD(group, u32, state);
    asm volatile("" : "=m"(step_particle_updates_slot), "=m"(frame_gap0), "=m"(frame_gap1),
        "=m"(burst_y), "=m"(frame_gap2), "=m"(burst_updates_slot), "=m"(sprite_flags_slot));
    switch (phase) {                              /* irregular */
    case BATTLE_THIN_BEAM_IMPACT_CREATE_BEAM:
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = CreateBattleAnimationSprite(group, 0, 1, (s16) (BATTLE_ANIMATION_FIELD(group, s32, x) - 0x80), (s32) BATTLE_ANIMATION_FIELD(group, s16, y), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2), 0, 1);
        PlayBattleAnimationSound(0);
        goto increment_state;
    case BATTLE_THIN_BEAM_IMPACT_WAIT_FOR_RAY_STEP:
        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]), u16, animation_step) != BATTLE_THIN_BEAM_IMPACT_RAY_ANIMATION_STEP) {
            return;
        }
        BATTLE_ANIMATION_FIELD(group, void *, sprites[1]) = CreateBattleAnimationSprite(group, 5, 0, (s16) BATTLE_ANIMATION_FIELD(group, s32, x), (s32) BATTLE_ANIMATION_FIELD(group, s16, y), BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
        goto increment_state;
    case BATTLE_THIN_BEAM_IMPACT_WAIT_FOR_IMPACT_STEP: {
        register u32 saved_sprite_slots_address asm("r9");
        register s32 *random_callback_register asm("r8");

        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]), u16, animation_step) != BATTLE_THIN_BEAM_IMPACT_TRIGGER_ANIMATION_STEP) {
            return;
        }
        {
            register s32 previous_sprite_flags asm("r2") = BATTLE_ANIMATION_FIELD(group, s32, sprite_flags);
            saved_sprite_flags = previous_sprite_flags;
        }
        BATTLE_ANIMATION_FIELD(group, s32, sprite_flags) = BATTLE_ANIMATION_HARDWARE_SPRITE_PRIORITY_3;
        BATTLE_ANIMATION_FIELD(group, void *, sprites[2]) = CreateBattleAnimationSprite(group, 0, 1, (s16) (BATTLE_ANIMATION_FIELD(group, s32, x) + 0x80), ({
            register s32 guard_r2 asm("r2");
            s32 y_value;
            asm volatile("" : "=r"(guard_r2));
            y_value = BATTLE_ANIMATION_FIELD(group, s16, y);
            asm volatile("" : "+r"(guard_r2));
            y_value;
        }), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_2), 0, 1);
        inherited_sprite_slot_index = 0;
        {
            register u32 state_address_init asm("r2") = BATTLE_ANIMATION_OFFSET(state);
            asm volatile("" : "+r"(state_address_init));
            state_address_init += (u32)group;
            state_address_or_emission_parity = state_address_init;
        }
        sprite_flags_slot = ((u8 *)group + BATTLE_ANIMATION_OFFSET(sprite_flags));
        {
            register s32 guard_r2 asm("r2");
            asm volatile("" : "=r"(guard_r2));
            saved_sprite_slots_address = (u32)((u8 *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
            asm volatile("" : "+r"(guard_r2));
        }
        random_callback_register = (s32 *)0x03000010;
        do {
            register s32 angle_random asm("r4");
            register s32 jittered_fragment_angle asm("r0");
            s32 sprite_offset;
            void *particle_sprite;

            {
                register s32 *random_callback_view asm("r2") = random_callback_register;
                angle_random = CallFunctionR0(*random_callback_view);
            }
            fragment_fan_angle = DivideSigned32(inherited_sprite_slot_index << 6, 7);
            jittered_fragment_angle = ((u32) (angle_random * 9) >> 0xF) - 0x24;
            {
                register s32 guard_r2 asm("r2");
                asm volatile("" : "=r"(guard_r2));
                asm volatile("add %0, %2, %0"
                    : "+r"(jittered_fragment_angle), "+r"(guard_r2) : "r"(fragment_fan_angle));
            }
            fragment_angle = jittered_fragment_angle;
            {
                register s32 *random_callback_view asm("r2") = random_callback_register;
                particle_sprite = (void *)CreateBattleAngledProjectileSprite(group, 3, 0, (s16) BATTLE_ANIMATION_FIELD(group, s32, x), (s32) BATTLE_ANIMATION_FIELD(group, s16, y), 0x20, fragment_angle, ((u32) (CallFunctionR0(*random_callback_view) * 0x201) >> 0xF) + ({ register s32 minimum_fragment_speed_fixed8 asm("r0") = 0x200; asm volatile("" : "+r"(minimum_fragment_speed_fixed8)); minimum_fragment_speed_fixed8; }), 0);
            }
            sprite_offset = inherited_sprite_slot_index + 3;
            sprite_offset <<= 2;
            *(void **)(saved_sprite_slots_address + sprite_offset) = particle_sprite;
            {
                register s32 next_r0 asm("r0") = inherited_sprite_slot_index + 1;
                asm volatile("" : "+r"(next_r0));
                inherited_sprite_slot_index = (u8) next_r0;
            }
        } while ((u32) inherited_sprite_slot_index <= 7U);
        {
            register s32 restore_r1 asm("r1") = saved_sprite_flags;
            register s32 *restore_r2 asm("r2") = sprite_flags_slot;
            asm volatile("" : "+r"(restore_r1), "+r"(restore_r2));
            *restore_r2 = restore_r1;
        }
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(1);
        state_or_sprite_slots = (s32 *)state_address_or_emission_parity;
        goto increment_value;
    }
    case BATTLE_THIN_BEAM_IMPACT_EMIT_PARTICLES_AND_WAIT:
        if ((u32) BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group, void *, sprites[0]), u16, animation_step) <= BATTLE_THIN_BEAM_IMPACT_LAST_PARTICLE_ANIMATION_STEP) {
            {
                register s32 *step_particle_updates_view asm("r2") = ((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.thin_beam_impact.particle_updates));
                asm volatile("" : "+r"(step_particle_updates_view));
                step_particle_updates_slot = step_particle_updates_view;
                asm volatile(
                    ".syntax unified\n\t"
                    "ldr r0, [%1]\n\t"
                    "mov %0, r0\n\t"
                    ".syntax divided"
                    : "=r"(emission_mask)
                    : "r"(step_particle_updates_view)
                    : "r0");
            }
            emission_mask &= 3;
            asm volatile("" : : "r"(emission_mask));
            if (emission_mask == 0) {
                register s32 *random_callback_slot asm("sl");
                s32 flags_register_carrier;
                s32 rng_value;
                {
                    register s32 *rng_seed asm("r2") = (s32 *)0x03000010;
                    asm volatile("" : "+r"(rng_seed));
                    random_callback_slot = rng_seed;
                    rng_value = *rng_seed;
                }
                upper_particle_angle = ((u32) (CallFunctionR0(rng_value) * 0x21) >> 0xF) + 0x80;
                {
                    register s32 *rng_r1 asm("r1") = random_callback_slot;
                    asm volatile("" : "+r"(rng_r1));
                    CreateBattleAngledProjectileSprite(group, 4, 0, (s16) BATTLE_ANIMATION_FIELD(group, s32, x), (s32) (s16) (BATTLE_ANIMATION_FIELD(group, s32, y) - 4), ({
                        register s32 particle_flags asm("r2");
                        asm volatile("" : "=l"(flags_register_carrier));
                        asm volatile("" : : "l"(flags_register_carrier));
                        asm volatile(
                            ".syntax unified\n\t"
                            "movs %0, #160\n\t"
                            "lsls %0, %0, #3\n\t"
                            ".syntax divided"
                            : "=r"(particle_flags));
                        particle_flags;
                    }), upper_particle_angle, ((u32) (CallFunctionR0(*rng_r1) * 0x101) >> 0xF) + 0x100, ({
                        register s32 mask_r0 asm("r0");
                        asm volatile(
                            ".syntax unified\n\t"
                            "mov %0, %1\n\t"
                            ".syntax divided"
                            : "=r"(mask_r0)
                            : "r"(emission_mask));
                        mask_r0;
                    }));
                }
                {
                    register s32 clobbered_r6 asm("r6");
                    asm volatile("" : "=r"(clobbered_r6));
                }
                {
                    register s32 *rng_r1 asm("r1") = random_callback_slot;
                    asm volatile("" : "+r"(rng_r1));
                    lower_particle_angle = ((u32) (CallFunctionR0(*rng_r1) * 0x21) >> 0xF) + 0x60;
                }
                {
                    register s32 *rng_r2 asm("r2") = random_callback_slot;
                    asm volatile("" : "+r"(rng_r2));
                    CreateBattleAngledProjectileSprite(group, 4, 0, ({
                        register s32 x_r3 asm("r3");
                        register s32 offset_r0 asm("r0") = 4;
                        asm volatile(
                            ".syntax unified\n\t"
                            "ldrsh %0, [%1, %2]\n\t"
                            ".syntax divided"
                            : "=r"(x_r3)
                            : "r"(group), "r"(offset_r0));
                        x_r3;
                    }), (s32) (s16) (BATTLE_ANIMATION_FIELD(group, s32, y) + 4), ({ register s32 particle_flags asm("r2"); asm volatile(".syntax unified\n\tmovs %0, #160\n\tlsls %0, %0, #3\n\t.syntax divided" : "=r"(particle_flags)); particle_flags; }), lower_particle_angle, ((u32) (CallFunctionR0(*rng_r2) * 0x101) >> 0xF) + 0x100, ({
                        register s32 mask_r0 asm("r0");
                        asm volatile(
                            ".syntax unified\n\t"
                            "mov %0, %1\n\t"
                            ".syntax divided"
                            : "=r"(mask_r0)
                            : "r"(emission_mask));
                        mask_r0;
                    }));
                }
                asm volatile("" : "+r"(flags_register_carrier));
            }
            *step_particle_updates_slot += 1;
        }
        inherited_sprite_slot_index = 0;
        if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0) {
            state_or_sprite_slots = (s32 *)((u8 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(sprites[0])));
scan_beam_sprites:
            {
                register s32 next_r0 asm("r0") = inherited_sprite_slot_index + 1;
                asm volatile("" : "+r"(next_r0));
                inherited_sprite_slot_index = (u8) next_r0;
            }
            if ((u32) inherited_sprite_slot_index <= 0xAU) {
                register u32 beam_wait_slot_address asm("r0") = inherited_sprite_slot_index * 4;
                asm volatile("add %0, %1, %0"
                    : "+r"(beam_wait_slot_address) : "r"(state_or_sprite_slots));
                if (*(void **)beam_wait_slot_address == 0) {
                    goto scan_beam_sprites;
                }
            }
        }
        if (inherited_sprite_slot_index != 0xB) {
            return;
        }
        state_or_sprite_slots = (s32 *)((u8 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.thin_beam_impact.particle_updates)));
        *state_or_sprite_slots = 0;
        state_or_sprite_slots -= 1;
        goto increment_value;
    case BATTLE_THIN_BEAM_IMPACT_EMIT_BURSTS: {
        /* Native R9 supplies the slot index. Each burst replaces the same four slots. */
        register s32 *burst_updates_init asm("r1") = ((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.thin_beam_impact.particle_updates));
        register u32 progress asm("r2") = *burst_updates_init;
        register u32 one asm("r0") = 1;
        register s32 minimum_speed_fixed8_r9 asm("r9");

        progress &= one;
        state_address_or_emission_parity = progress;
        burst_updates_slot = burst_updates_init;
        if (progress != 0) {

        } else {
            register s32 y_raw_r2 asm("r2");
            register s32 rng_result_r0 asm("r0");
            {
                register s32 *random_callback_view asm("r1") = (s32 *)0x03000010;
                burst_x_bits = (BATTLE_ANIMATION_FIELD(group, s32, x) + ((u32) (CallFunctionR0(*random_callback_view) * 9) >> 0xF)) - 4;
            }
            {
                register s32 *random_callback_view asm("r2") = (s32 *)0x03000010;
                asm volatile("" : "+r"(random_callback_view));
                rng_result_r0 = CallFunctionR0(*random_callback_view);
            }
            y_raw_r2 = BATTLE_ANIMATION_FIELD(group, s32, y);
            asm volatile("" : "+r"(y_raw_r2), "+r"(rng_result_r0));
            y_raw_r2 += (u32) (rng_result_r0 * 9) >> 0xF;
            y_raw_r2 -= 4;
            asm volatile(
                ".syntax unified\n\t"
                "lsls %0, %0, #16\n\t"
                "asrs %0, %0, #16\n\t"
                "lsls r0, %4, #16\n\t"
                "asrs %1, r0, #16\n\t"
                "str %1, %2\n\t"
                ".syntax divided"
                : "=r"(burst_x), "=r"(burst_y_value), "=m"(burst_y)
                : "0"(burst_x_bits), "r"(y_raw_r2)
                : "r0");
            {
                register s32 zero_r2 asm("r2");
                burst_sprite = CreateBattleAnimationSprite(group, 1, 0, burst_x,
                    burst_y_value, ({ zero_r2 = state_address_or_emission_parity; asm volatile("" : "+r"(zero_r2)); zero_r2; }), zero_r2, zero_r2);
            }
            burst_slot_offset = inherited_sprite_slot_index * 4;
            burst_sprite_slot_address = ((u8 *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
            *(void **)((u8 *)burst_sprite_slot_address + burst_slot_offset) = burst_sprite;
            {
                register s32 guard_r2 asm("r2");
                s32 rng_seed;
                s32 speed_fixed8;
                asm volatile("" : "=r"(guard_r2));
                rng_seed = *(s32 *)0x03000010;
                asm volatile("" : "+r"(guard_r2));
                speed_fixed8 = (u32) (CallFunctionR0(rng_seed) * 0x401) >> 0xF;
                {
                    register s32 minimum_speed_fixed8_r2 asm("r2") = 0x200;
                    asm volatile("" : "+r"(minimum_speed_fixed8_r2));
                    minimum_speed_fixed8_r9 = minimum_speed_fixed8_r2;
                }
                speed_fixed8 += minimum_speed_fixed8_r9;
                speed_fixed8 *= 2;
                burst_sprite = (void *)CreateBattleAngledProjectileSprite(group, 2, 0, burst_x,
                    burst_y, 0x100, 0x80, speed_fixed8, state_address_or_emission_parity);
                burst_slot_offset = inherited_sprite_slot_index + 8;
                burst_slot_offset <<= 2;
                *(void **)((u8 *)burst_sprite_slot_address + burst_slot_offset) = burst_sprite;
            }
            {
                register s32 clobbered_r5 asm("r5");
                asm volatile("" : "=r"(clobbered_r5));
            }
            {
                register s32 guard_r2 asm("r2");
                s32 rng_seed;
                s32 speed_fixed8;
                asm volatile("" : "=r"(guard_r2));
                rng_seed = *(s32 *)0x03000010;
                asm volatile("" : "+r"(guard_r2));
                speed_fixed8 = (u32) (CallFunctionR0(rng_seed) * 0x201) >> 0xF;
                speed_fixed8 += minimum_speed_fixed8_r9;
                speed_fixed8 *= 2;
                {
                    register s32 y_r2 asm("r2") = burst_y;
                    asm volatile("" : "+r"(y_r2));
                    burst_sprite = (void *)CreateBattleAngledProjectileSprite(group, 3, 0, burst_x,
                        y_r2, 0x120, ({ register s32 guard_r2 asm("r2"); s32 particle_angle = 0x80; asm volatile("" : "=r"(guard_r2)); asm volatile("" : "+r"(particle_angle), "+r"(guard_r2)); particle_angle; }), speed_fixed8, ({ register s32 zero_r1 asm("r1") = state_address_or_emission_parity; asm volatile("" : "+r"(zero_r1)); zero_r1; }));
                }
                burst_slot_offset = inherited_sprite_slot_index + 0x10;
                burst_slot_offset <<= 2;
                *(void **)((u8 *)burst_sprite_slot_address + burst_slot_offset) = burst_sprite;
            }
            {
                register s32 *sprite_flags_slot_view asm("r5") = ((u8 *)group + BATTLE_ANIMATION_OFFSET(sprite_flags));
                register s32 previous_sprite_flags asm("r2") = *sprite_flags_slot_view;
                asm volatile("" : "+r"(sprite_flags_slot_view));
                saved_sprite_flags = previous_sprite_flags;
                *sprite_flags_slot_view = BATTLE_ANIMATION_HARDWARE_SPRITE_PRIORITY_3;
                {
                    register s32 guard_r2 asm("r2");
                    s32 rng_seed;
                    s32 speed_fixed8;
                    asm volatile("" : "=r"(guard_r2));
                    rng_seed = *(s32 *)0x03000010;
                    asm volatile("" : "+r"(guard_r2));
                    speed_fixed8 = (u32) (CallFunctionR0(rng_seed) * 0x401)
                        >> 0xF;
                    speed_fixed8 += minimum_speed_fixed8_r9;
                    speed_fixed8 *= 2;
                    {
                        register s32 y_r2 asm("r2") = burst_y;
                        register s32 zero_r0 asm("r0");
                        asm volatile("" : "+r"(y_r2));
                        burst_sprite = (void *)CreateBattleAngledProjectileSprite(group, 2, 0,
                            burst_x, y_r2, ({ zero_r0 = state_address_or_emission_parity; asm volatile("" : "+r"(zero_r0)); zero_r0; }), zero_r0, speed_fixed8,
                            zero_r0);
                    }
                }
                burst_slot_offset = inherited_sprite_slot_index + 0x18;
                burst_slot_offset <<= 2;
                asm volatile("add %0, %1" : "+r"(burst_sprite_slot_address) : "r"(burst_slot_offset));
                *(void **)burst_sprite_slot_address = burst_sprite;
                {
                    register s32 guard_r0 asm("r0");
                    register s32 guard_r1 asm("r1");
                    asm volatile("" : "=r"(guard_r0), "=r"(guard_r1));
                    *sprite_flags_slot_view = saved_sprite_flags;
                    asm volatile("" : "+r"(guard_r0), "+r"(guard_r1));
                }
            }
            {
                register s32 *burst_updates_view asm("r1") = burst_updates_slot;
                asm volatile("" : "+r"(burst_updates_view));
                if (*burst_updates_view == 0) {
                    SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
                    PlayBattleAnimationSound(1);
                }
            }
        }
        {
            register s32 *burst_updates_store asm("r2") = burst_updates_slot;
            asm volatile("" : "+r"(burst_updates_store));
            next_burst_update = *burst_updates_store + 1;
            *burst_updates_store = next_burst_update;
        }
        if (next_burst_update == BATTLE_THIN_BEAM_IMPACT_BURST_UPDATES) {
            goto increment_state;
        }
        return;
    }

    increment_state:
        state_or_sprite_slots = (s32 *)((u8 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state)));
increment_value:
        *state_or_sprite_slots += 1;
        return;

    case BATTLE_THIN_BEAM_IMPACT_WAIT_FOR_BURSTS:
        inherited_sprite_slot_index = 0;
        if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0) {
            state_or_sprite_slots = (s32 *)((u8 *)((u8 *)group + BATTLE_ANIMATION_OFFSET(sprites[0])));
scan_burst_sprites:
            {
                register s32 next_r0 asm("r0") = inherited_sprite_slot_index + 1;
                asm volatile("" : "+r"(next_r0));
                inherited_sprite_slot_index = (u8) next_r0;
            }
            if ((u32) inherited_sprite_slot_index <= 0x1FU) {
                register u32 burst_wait_slot_address asm("r0") = inherited_sprite_slot_index * 4;
                asm volatile("add %0, %1, %0"
                    : "+r"(burst_wait_slot_address) : "r"(state_or_sprite_slots));
                if (*(void **)burst_wait_slot_address == 0) {
                    goto scan_burst_sprites;
                }
            }
        }
        if (inherited_sprite_slot_index == 0x20) {
            DestroySpriteGroup(group);
        }
        break;
    }
}
