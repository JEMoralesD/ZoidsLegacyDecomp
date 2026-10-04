#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s32 gRandomNumberCallback asm("D_03000010");

s16 Sin256(s32) asm("func_08092A90");
s16 Cos256(s32) asm("func_08092ADC");
void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32) asm("func_080D2660");
s32 CallFunctionR0(s32) asm("func_080ECD5C");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");

void SpawnBattleInwardChargeParticle(struct BattleAnimationGroup *group, s32 speed_scale, s32 unused_argument) asm("func_080DCDC4");

void SpawnBattleInwardChargeParticle(struct BattleAnimationGroup *group, s32 speed_scale, s32 unused_argument)
{
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    volatile s32 saved_speed_scale;
    volatile void **saved_sprite_slot;
    register void *group_bytes asm("r6");
    register u32 sprite_slot_index asm("r2");
    register s32 *random_callback_slot asm("r4");
    register volatile s32 *outgoing asm("sp");

    asm("" : "=m"(reserve0), "=m"(reserve1), "=m"(reserve2),
             "=m"(reserve3), "=m"(reserve4));

    group_bytes = group;
    speed_scale <<= 24;
    saved_speed_scale = (u32)speed_scale >> 24;
    sprite_slot_index = BATTLE_CHARGE_PARTICLE_FIRST_SLOT;
    random_callback_slot = &gRandomNumberCallback;
    do {
        register u32 sprite_slot_offset asm("r1");
        register u8 *sprite_slots_base asm("r0");
        void **free_sprite_slot;
        register void *previous_sprite asm("r9");

        sprite_slot_offset = sprite_slot_index << 2;
        sprite_slots_base = group_bytes;
        sprite_slots_base += BATTLE_ANIMATION_OFFSET(sprites[0]);
        free_sprite_slot = (void **)(sprite_slots_base + sprite_slot_offset);
        saved_sprite_slot = free_sprite_slot;
        previous_sprite = *free_sprite_slot;
        if (previous_sprite == 0) {
            register u16 radius_pixels asm("r5");
            register u16 saved_radius_pixels asm("sl");
            register u32 spawn_angle asm("r4");
            register u32 saved_spawn_angle asm("r8");
            s32 spawn_x;
            s32 speed_fixed8;

            {
                register u32 radius_or_angle_random asm("r1");

                radius_or_angle_random = (u32)(CallFunctionR0(*random_callback_slot) * BATTLE_CHARGE_PARTICLE_RADIUS_RANDOM_RANGE) >> 15;
                radius_or_angle_random += BATTLE_CHARGE_PARTICLE_RADIUS_MIN;
                radius_or_angle_random <<= 16;
                radius_pixels = radius_or_angle_random >> 16;
                saved_radius_pixels = radius_pixels;
            }
            {
                register u32 radius_or_angle_random asm("r0");

                radius_or_angle_random = (u32)(CallFunctionR0(*random_callback_slot) * BATTLE_CHARGE_PARTICLE_ANGLE_RANDOM_RANGE) >> 15;
                radius_or_angle_random += BATTLE_CHARGE_PARTICLE_ANGLE_MIN;
                radius_or_angle_random <<= 24;
                spawn_angle = radius_or_angle_random >> 24;
                saved_spawn_angle = spawn_angle;
            }
            {
                register s32 coordinate_work asm("r0");
                s32 origin_coordinate;

                coordinate_work = Cos256(spawn_angle);
                origin_coordinate = *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(x));
                coordinate_work = (s16)coordinate_work;
                coordinate_work *= radius_pixels;
                if (coordinate_work < 0) {
                    coordinate_work += 0xFF;
                }
                coordinate_work >>= 8;
                coordinate_work = origin_coordinate + coordinate_work;
                coordinate_work <<= 16;
                spawn_x = coordinate_work >> 16;
            }
            {
                register s32 coordinate_work asm("r0");
                s32 origin_coordinate;

                coordinate_work = Sin256(spawn_angle);
                origin_coordinate = *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(y));
                coordinate_work = (s16)coordinate_work;
                coordinate_work *= radius_pixels;
                if (coordinate_work < 0) {
                    coordinate_work += 0xFF;
                }
                coordinate_work >>= 8;
                coordinate_work = origin_coordinate + coordinate_work;
                coordinate_work <<= 16;
                outgoing[0] = coordinate_work >> 16;
            }
            outgoing[1] = BATTLE_SPRITE_SEMITRANSPARENT;
            outgoing[2] = saved_spawn_angle + 0x80;
            {
                register s32 speed_scale_view asm("r1") = saved_speed_scale;
                register s32 speed_scale_fixed8 asm("r0");
                register s32 scaled_radius_fixed8 asm("r1");
                register s32 scaled_radius_argument asm("r0");

                speed_scale_fixed8 = speed_scale_view << 8;
                scaled_radius_fixed8 = saved_radius_pixels;
                scaled_radius_fixed8 *= speed_scale_fixed8;
                scaled_radius_argument = scaled_radius_fixed8;
                speed_fixed8 = DivideSigned32(scaled_radius_argument, BATTLE_CHARGE_PARTICLE_RADIUS_MIN);
            }
            outgoing[3] = speed_fixed8;
            outgoing[4] = (s32)previous_sprite;
            {
                register void *call_group asm("r0") = group_bytes;
                register s32 call_resource_slot asm("r1") = 1;
                register s32 call_animation_id asm("r2") = 0;
                register s32 call_x asm("r3");

                asm volatile("" : "+r"(call_group), "+r"(call_resource_slot),
                                  "+r"(call_animation_id));
                call_x = spawn_x;
                *saved_sprite_slot = CreateBattleAngledProjectileSprite(call_group, call_resource_slot, call_animation_id,
                                            call_x);
            }
            return;
        }
        {
            register u32 next_index_bits asm("r0") = sprite_slot_index + 1;
            sprite_slot_index = (u8)next_index_bits;
        }
    } while (sprite_slot_index < BATTLE_ANIMATION_GROUP_SPRITE_COUNT);
}
