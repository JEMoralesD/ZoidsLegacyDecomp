#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleMissileImpactEffect(void *group) asm("func_080D65E0");

void UpdateBattleMissileImpactEffect(void *group)
{
    register void *group_bytes asm("r5") = group;
    register s32 *phase_slot asm("r6") = (s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *phase_slot;

    switch (phase) {
    case BATTLE_MISSILE_IMPACT_LAUNCH:
        goto launch_missile;
    case BATTLE_MISSILE_IMPACT_CREATE:
        goto create_impact;
    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES:
        goto wait_for_sprites;
    default:
        return;
    }

launch_missile:
    {
        register s32 *x_slot asm("r4") = (s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x));
        void *missile;
        s32 x;

        x = *x_slot - 0x100;
        missile = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
            *(s16 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
        *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile;
        BATTLE_SPRITE_FIELD(missile, void *, user_data.missile_trail.group) = group_bytes;
        {
            register s32 zero asm("r2") = 0;
            register s32 value asm("r1");

            BATTLE_SPRITE_FIELD(missile, s32, user_data.missile_trail.elapsed_frames) = zero;
            value = *x_slot;
            BATTLE_SPRITE_FIELD(missile, s32, user_data.missile_trail.impact_x) = value;
            BATTLE_SPRITE_FIELD(missile, s32, user_data.missile_trail.trail_resource_slot_base) = zero;
        }
    }
    phase = 0;
    goto play_sound_and_advance;

create_impact:
    {
        void *missile = *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));

        if ((*(u32 *)missile & BATTLE_SPRITE_HIDDEN) == 0) {
            return;
        }
        *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(group_bytes, 2, 0,
            *(s16 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.x)),
            *(s16 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y)), 0, 0, 0);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
    }
    phase = 1;

play_sound_and_advance:
    PlayBattleAnimationSound(phase);
    *phase_slot += 1;
    return;

wait_for_sprites:
    {
        u8 live_trail_index = 0x18;
        register void *missile asm("r1") = *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 elapsed_frames asm("r0") = BATTLE_SPRITE_FIELD(missile, u32, user_data.missile_trail.elapsed_frames);
        register u32 trail_slot_end asm("r3");

        elapsed_frames >>= 1;
        asm volatile("" : "+r"(elapsed_frames));
        trail_slot_end = elapsed_frames;
        trail_slot_end += 0x19;
        if ((u32)live_trail_index < trail_slot_end && *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
            register u32 trail_scan_end asm("r4") = trail_slot_end;
            register u8 *sprite_slots asm("r3") = (u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);

            do {
                live_trail_index += 1;
            } while ((u32)live_trail_index < trail_scan_end &&
                *(void **)(sprite_slots + (live_trail_index << 2)) == 0);
        }
        if (live_trail_index == ((*(volatile u32 *)((u8 *)missile + (s32)&((struct BattleDisplaySprite *)0)->user_data.missile_trail.elapsed_frames) >> 1)
                + 0x19) && *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
            DestroySpriteGroup(group_bytes);
        }
    }
}
