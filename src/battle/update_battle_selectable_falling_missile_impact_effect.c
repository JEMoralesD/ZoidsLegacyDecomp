#include "m2c_prelude.h"
extern void UpdateBattleTargetedFallingMissileTrailSprite(void *) asm("func_080D6E6C");
#include "battle_animation.h"
#include "battle_display.h"
extern void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleSelectableFallingMissileImpactEffect(struct BattleAnimationGroup *group) asm("func_080DE0F8");

void UpdateBattleSelectableFallingMissileImpactEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r5") = group;
    register s32 *effect_state_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 phase asm("r0") = *effect_state_slot;

    switch (phase) {
    case BATTLE_MISSILE_IMPACT_LAUNCH: {
        register s32 use_falling_motion asm("r6") = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact_variant.use_diagonal_motion));
        register s32 *impact_x_slot asm("r4");
        register void *missile_sprite asm("r0");

        if (use_falling_motion == 0) {
            impact_x_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact_variant.x));
            missile_sprite = CreateBattleAnimationSprite(group_bytes, 2, 0,
                (s16)(*impact_x_slot - 0x100), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact_variant.y)),
                0x20, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.group)) = group_bytes;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) = use_falling_motion;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.impact_x)) = *impact_x_slot;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.trail_resource_slot_base)) = 2;
        } else {
            impact_x_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact_variant.x));
            missile_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0,
                (s16)(*impact_x_slot - 0x100),
                (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact_variant.y)) - 0x80),
                0x20, (s32)UpdateBattleTargetedFallingMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.group)) = group_bytes;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) = 0;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.impact_x)) = *impact_x_slot;
        }
        PlayBattleAnimationSound(0);
        {
            register s32 *advance_state_slot asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
            *advance_state_slot = *advance_state_slot + 1;
        }
        break;
    }
    case BATTLE_MISSILE_IMPACT_CREATE:
        if ((**(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) == 0) {
            break;
        }
        {
            register char *impact_x_view_or_y_value asm("r0") = group_bytes + BATTLE_ANIMATION_OFFSET(effect.missile_impact_variant.x);
            register s32 coordinate_load_offset asm("r1");
            register s32 impact_x asm("r3");

            asm volatile(
                "mov %1, #0\n\t"
                "ldrsh %2, [%0, %1]\n\t"
                "add %0, #4\n\t"
                "mov %1, #0\n\t"
                "ldrsh %0, [%0, %1]"
                : "+r"(impact_x_view_or_y_value), "=r"(coordinate_load_offset), "=r"(impact_x));
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(group_bytes, 4, 0,
                impact_x, (s32)impact_x_view_or_y_value, 0, 0, 0);
        }
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
        PlayBattleAnimationSound(1);
        *effect_state_slot = *effect_state_slot + 1;
        break;
    case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES: {
        register s32 trail_slot_index asm("r2") = 0x18;
        register void *missile_sprite asm("r1") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 trail_elapsed_updates asm("r0") = *(u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames));
        register u32 trail_scan_bound asm("r3");

        trail_elapsed_updates >>= 1;
        trail_scan_bound = trail_elapsed_updates + 0x19;
        if (trail_slot_index < trail_scan_bound && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
            register u32 saved_trail_scan_bound asm("r4") = trail_scan_bound;
            register char *sprite_slots asm("r3") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register s32 next_index_bits asm("r0") = trail_slot_index + 1;
                next_index_bits <<= 24;
                trail_slot_index = (u32)next_index_bits >> 24;
            } while (trail_slot_index < saved_trail_scan_bound &&
                *(s32 *)(sprite_slots + (trail_slot_index << 2)) == 0);
        }
        if (trail_slot_index == ((*(u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) >> 1) + 0x19)
                && *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
    }
}
