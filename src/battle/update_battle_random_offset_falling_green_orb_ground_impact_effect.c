#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedFallingGreenOrbTrailSprite(void *) asm("func_080D8D20");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleRandomOffsetFallingGreenOrbGroundImpactEffect(void *group) asm("func_080D9718");

void UpdateBattleRandomOffsetFallingGreenOrbGroundImpactEffect(void *group)
{
    register void *group_bytes asm("r4") = group;
    register s32 *phase_slot asm("r5") = (s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *phase_slot;

    switch (phase) {
    case BATTLE_ORB_CLOUD_IMPACT_LAUNCH:
        goto launch_orb;
    case BATTLE_ORB_CLOUD_IMPACT_CREATE_CLOUDS:
        goto create_ground_cloud;
    case BATTLE_ORB_CLOUD_IMPACT_WAIT_FOR_SPRITES:
        goto wait_for_sprites;
    default:
        return;
    }

launch_orb:
    {
        void *projectile;
        s32 x;

        x = *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(x));
        x += *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.random_diagonal_launch.launch_x_offset_half)) * 2;
        projectile = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
            *(s16 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedFallingGreenOrbTrailSprite, 1);
        *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = projectile;
        BATTLE_SPRITE_FIELD(projectile, void *, user_data.projectile_trail.group) = group_bytes;
        {
            register s32 zero asm("r1") = 0;

            BATTLE_SPRITE_FIELD(projectile, s32, user_data.projectile_trail.elapsed_frames) = zero;
            BATTLE_SPRITE_FIELD(projectile, s32, user_data.projectile_trail.impact_x) = zero;
        }
    }
    phase = 0;
    goto play_sound_and_advance;

create_ground_cloud:
    {
        void *projectile = *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));

        if ((*(u32 *)projectile & BATTLE_SPRITE_HIDDEN) == 0) {
            return;
        }
        {
            void *ground_impact_sprite;
            s32 x;

            x = *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(x));
            x += *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.random_diagonal_launch.launch_x_offset_half)) * 2;
            x += 0xF0;
            ground_impact_sprite = CreateBattleAnimationSprite(group_bytes, 2, 0, (s16)x,
                0x70, BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
            *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = ground_impact_sprite;
        }
    }
    phase = 1;

play_sound_and_advance:
    PlayBattleAnimationSound(phase);
    *phase_slot += 1;
    return;

wait_for_sprites:
    {
        u8 live_trail_index = 0x10;
        register void *projectile_init asm("r1") =
            *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 elapsed_trail_frames asm("r0") =
            BATTLE_SPRITE_FIELD(projectile_init, u32, user_data.projectile_trail.elapsed_frames);
        register u32 trail_slot_end asm("r3");
        register void *projectile asm("r5");

        elapsed_trail_frames >>= 1;
        asm volatile("" : "+r"(elapsed_trail_frames));
        trail_slot_end = elapsed_trail_frames;
        trail_slot_end += 0x11;
        projectile = projectile_init;
        if ((u32)live_trail_index < trail_slot_end && *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[16])) == 0) {
            register u32 trail_scan_end asm("r1") = trail_slot_end;
            register u8 *sprite_slots asm("r3") = (u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);

            do {
                live_trail_index += 1;
            } while ((u32)live_trail_index < trail_scan_end &&
                *(void **)(sprite_slots + (live_trail_index << 2)) == 0);
        }
        if (live_trail_index == ((*(volatile u32 *)((u8 *)projectile + 0x2C) >> 1)
                + 0x11) && *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
            DestroySpriteGroup(group_bytes);
        }
    }
}
