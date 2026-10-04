#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedRisingMissileTrailSprite(void *) asm("func_080D6D84");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleRisingMissileImpactEffect(void *group) asm("func_080D706C");

void UpdateBattleRisingMissileImpactEffect(void *group)
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
        void *child;
        s32 x;

        x = *x_slot - 0x100;
        child = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
            (s16)(*(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact.y)) + 0x80),
            0x20, (s32)UpdateBattleTargetedRisingMissileTrailSprite, 1);
        *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = child;
        BATTLE_SPRITE_FIELD(child, void *, user_data.missile_trail.group) = group_bytes;
        {
            register s32 zero asm("r1") = 0;
            register s32 value asm("r1");

            BATTLE_SPRITE_FIELD(child, s32, user_data.missile_trail.elapsed_frames) = zero;
            value = *x_slot;
            BATTLE_SPRITE_FIELD(child, s32, user_data.missile_trail.impact_x) = value;
        }
    }
    phase = 0;
    goto play_sound_and_advance;

create_impact:
    {
        void *child = *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));

        if ((*(u32 *)child & BATTLE_SPRITE_HIDDEN) == 0) {
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
        u8 index = 0x18;
        register void *child asm("r1") = *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 elapsed_trail_frames asm("r0") = BATTLE_SPRITE_FIELD(child, u32, user_data.missile_trail.elapsed_frames);
        register u32 trail_slot_end asm("r3");

        elapsed_trail_frames >>= 1;
        asm volatile("" : "+r"(elapsed_trail_frames));
        trail_slot_end = elapsed_trail_frames;
        trail_slot_end += 0x19;
        if ((u32)index < trail_slot_end && *(s32 *)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
            register u32 trail_scan_end asm("r4") = trail_slot_end;
            register u8 *slots asm("r3") = (u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);

            do {
                index += 1;
            } while ((u32)index < trail_scan_end &&
                *(void **)(slots + (index << 2)) == 0);
        }
        if (index == ((*(volatile u32 *)((u8 *)child + 0x2C) >> 1)
                + 0x19) && *(void **)((u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
            DestroySpriteGroup(group_bytes);
        }
    }
}
