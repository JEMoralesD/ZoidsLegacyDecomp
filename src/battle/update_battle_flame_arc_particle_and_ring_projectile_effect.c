#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
#define gBattleFlameArcParticleLaunchXOffsets ((const u8 *)0x087A2D08)
extern void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 gBattleZoidScrollX asm("D_02034034");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFlameArcParticleAndRingProjectileEffect(struct BattleAnimationGroup *group) asm("func_080E0134");

void UpdateBattleFlameArcParticleAndRingProjectileEffect(struct BattleAnimationGroup *group)
{
    register char *group_bytes asm("r6") = group;
    register s32 *effect_state_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state_slot;

    asm volatile("" ::: "r7");

    switch (phase) {
    case BATTLE_FLAME_ARC_PARTICLE_RING_CREATE_ARC: {
        register s32 scroll asm("r0") = gBattleZoidScrollX;
        register s32 x asm("r3");

        if (scroll < 0) {
            scroll += 0xFF;
        }
        scroll >>= 8;
        x = 0x80;
        x -= scroll;
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
            8, BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
        PlayBattleAnimationSound(0);
        *effect_state_slot = *effect_state_slot + 1;
        break;
    }
    case BATTLE_FLAME_ARC_PARTICLE_RING_EMIT_PARTICLES: {
        register u32 sprite_slot_index asm("r1") = 1;
        register char *sprite_slots asm("r2") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
        register u32 *random_callback_slot asm("r3") = &gRandomNumberCallback;
        u32 *saved_random_callback_slot;
        s32 *sprite_slot;
        register s32 empty_sprite_slot asm("r4");

        asm volatile("" : "=m"(saved_random_callback_slot));

find_free_particle_slot:
            sprite_slot = (s32 *)(sprite_slots + (sprite_slot_index << 2));
            empty_sprite_slot = *sprite_slot;
            if (empty_sprite_slot == 0) {
                register u32 launch_position_index asm("r5");
                register u32 random_bits asm("r0");
                register u32 launch_position_index_bits asm("r1");
                register s32 particle_speed_fixed8 asm("r1");
                register s32 scroll asm("r0");
                register u32 launch_x_offset_address asm("r0");
                register u32 launch_x_offset asm("r0");
                register s32 launch_x_base asm("r2");
                register s32 x asm("r3");
                register s32 y asm("r0");
                register u32 random_callback asm("r0") = *random_callback_slot;

                saved_random_callback_slot = random_callback_slot;
                random_bits = CallFunctionR0(random_callback);
                launch_position_index_bits = random_bits << 4;
                launch_position_index_bits += random_bits;
                launch_position_index_bits >>= 15;
                launch_position_index_bits <<= 24;
                launch_position_index_bits >>= 24;
                launch_position_index = launch_position_index_bits;
                asm volatile("" : "+r"(launch_position_index_bits));
                random_callback_slot = saved_random_callback_slot;
                random_bits = CallFunctionR0(*random_callback_slot);
                particle_speed_fixed8 = (u32)(random_bits * 0x101) >> 15;
                particle_speed_fixed8 += 0x200;
                launch_x_offset_address = (u32)gBattleFlameArcParticleLaunchXOffsets;
                asm volatile("add %0, %1, %0"
                    : "+r"(launch_x_offset_address) : "r"(launch_position_index));
                launch_x_offset = *(u8 *)launch_x_offset_address;
                launch_x_base = launch_x_offset;
                asm volatile("" : "+r"(launch_x_offset));
                launch_x_base += 0x80;
                scroll = gBattleZoidScrollX;
                if (scroll < 0) {
                    scroll += 0xFF;
                }
                x = scroll >> 8;
                x = launch_x_base - x;
                x = (s16)x;
                asm volatile("" : "+r"(x));
                y = launch_position_index;
                y += 0x10;
                *sprite_slot = (s32)CreateBattleAngledProjectileSprite(group_bytes, 1, 0, x,
                    y, (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), empty_sprite_slot, particle_speed_fixed8, empty_sprite_slot);
                goto check_flame_arc_finished;
            }
            {
                register u32 next_index_bits asm("r0") = sprite_slot_index + 1;
                next_index_bits <<= 24;
                sprite_slot_index = next_index_bits >> 24;
            }
            if (sprite_slot_index < BATTLE_ANIMATION_GROUP_SPRITE_COUNT) {
                goto find_free_particle_slot;
            }

check_flame_arc_finished:
        {
            register void *primary_sprite asm("r1") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            if (primary_sprite == 0) {
                register void *effect_sprite asm("r0");
                register s32 origin_x_offset asm("r2") = 4;
                register s32 x asm("r3");

                asm volatile("ldrsh %0, [%1, %2]"
                    : "=r"(x) : "r"(group_bytes), "r"(origin_x_offset));
                effect_sprite = CreateBattleAnimationSprite(group_bytes, 2, 0, x,
                    *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_LOOP_ANIMATION), (s32)UpdateBattleHorizontalProjectileSprite, primary_sprite);
                *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = effect_sprite;
                *(s32 *)((char *)effect_sprite + BATTLE_SPRITE_OFFSET(user_data.horizontal_projectile.speed)) = BATTLE_FLAME_ARC_PARTICLE_RING_SPEED_PIXELS;
                PlayBattleAnimationSound(1);
                {
                    register s32 *effect_state_slot_view asm("r1") =
                        (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
                    *effect_state_slot_view = *effect_state_slot_view + 1;
                }
            }
        }
        break;
    }
    case BATTLE_FLAME_ARC_PARTICLE_RING_WAIT_FOR_SPRITES: {
        register u32 sprite_slot_index asm("r1") = 0;
        if (*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *sprite_slots asm("r2") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register u32 next_index_bits asm("r0") = sprite_slot_index + 1;
                next_index_bits <<= 24;
                sprite_slot_index = next_index_bits >> 24;
            } while ((u32)sprite_slot_index < BATTLE_ANIMATION_GROUP_SPRITE_COUNT &&
                *(s32 *)(sprite_slots + (sprite_slot_index << 2)) == 0);
        }
        if (sprite_slot_index == BATTLE_ANIMATION_GROUP_SPRITE_COUNT) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
