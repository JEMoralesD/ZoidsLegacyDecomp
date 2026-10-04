#include "m2c_prelude.h"
extern void UpdateBattleTargetedFallingMissileTrailSprite(void *) asm("func_080D6E6C");
#include "battle_animation.h"
#include "battle_display.h"
extern void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleRandomOffsetSelectableFallingMissileGroundImpactEffect(struct BattleAnimationGroup *group) asm("func_080DE3B4");

void UpdateBattleRandomOffsetSelectableFallingMissileGroundImpactEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r4") = group;

    if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.use_diagonal_motion)) == 0) {
        register s32 *effect_state_slot asm("r6") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        register u32 phase asm("r5") = *effect_state_slot;

        if (phase == 0) {
            register void *missile_sprite asm("r0");
            missile_sprite = CreateBattleAnimationSprite(group_bytes, 2, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
                (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels)) + 0x50),
                0x20, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.group)) = group_bytes;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) = phase;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.impact_x)) = phase;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.trail_resource_slot_base)) = 2;
            PlayBattleAnimationSound(0);
            *effect_state_slot = *effect_state_slot + 1;
            return;
        }
        {
            register void *missile_sprite_view asm("r0") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            register u32 missile_flags asm("r1") = *(u32 *)missile_sprite_view;
            register u32 hidden_mask asm("r2") = BATTLE_SPRITE_HIDDEN;
            register void *missile_sprite asm("r5");
            missile_flags &= hidden_mask;
            missile_sprite = missile_sprite_view;
            if (missile_flags == 0) {
                return;
            }
            {
                register s32 trail_slot_index asm("r2") = 0x18;
                register u32 trail_elapsed_updates asm("r0") =
                    *(u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames));
                register u32 trail_scan_bound asm("r1");
                trail_elapsed_updates >>= 1;
                trail_scan_bound = trail_elapsed_updates + 0x19;
                if ((u32)trail_slot_index < trail_scan_bound && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
                    register u32 saved_trail_scan_bound asm("r3") = trail_scan_bound;
                    register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    do {
                        register s32 next_index_bits asm("r0") = trail_slot_index + 1;
                        next_index_bits <<= 24;
                        trail_slot_index = (u32)next_index_bits >> 24;
                    } while ((u32)trail_slot_index < saved_trail_scan_bound &&
                        *(s32 *)(sprite_slots + (trail_slot_index << 2)) == 0);
                }
                if (trail_slot_index ==
                        ((*(volatile u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) >> 1)
                         + 0x19)) {
                    DestroySpriteGroup(group_bytes);
                }
            }
        }
        return;
    }

    {
        register s32 *effect_state_slot asm("r5") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        register u32 phase asm("r0") = *effect_state_slot;

        switch (phase) {
        case BATTLE_MISSILE_IMPACT_LAUNCH: {
            register void *missile_sprite asm("r0");
            register s32 x asm("r3") = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
            x += *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels)) * 2;
            missile_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, (s16)x,
                *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), 0x20, (s32)UpdateBattleTargetedFallingMissileTrailSprite, 1);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.group)) = group_bytes;
            {
                register s32 zero asm("r1") = 0;
                *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) = zero;
                *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.impact_x)) = zero;
            }
            phase = 0;
            goto advance;
        }
        case BATTLE_MISSILE_IMPACT_CREATE:
            if ((**(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) == 0) {
                return;
            }
            {
                register s32 x asm("r3") = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
                x += *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels)) * 2;
                x += 0xF0;
                *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(group_bytes, 4, 0,
                    (s16)x, 0x70, 0, 0, 0);
            }
            phase = 1;
advance:
            PlayBattleAnimationSound(phase);
            *effect_state_slot = *effect_state_slot + 1;
            return;
        case BATTLE_MISSILE_IMPACT_WAIT_FOR_SPRITES: {
            register s32 trail_slot_index asm("r2") = 0x18;
            register void *missile_sprite_view asm("r1") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            register u32 trail_elapsed_updates asm("r0") =
                *(u32 *)((char *)missile_sprite_view + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames));
            register u32 trail_scan_bound asm("r3");
            register void *missile_sprite asm("r5");
            trail_elapsed_updates >>= 1;
            asm volatile("" : "+r"(trail_elapsed_updates));
            trail_scan_bound = trail_elapsed_updates + 0x19;
            missile_sprite = missile_sprite_view;
            if ((u32)trail_slot_index < trail_scan_bound && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
                register u32 saved_trail_scan_bound asm("r3") = trail_scan_bound;
                register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    register s32 next_index_bits asm("r0") = trail_slot_index + 1;
                    next_index_bits <<= 24;
                    trail_slot_index = (u32)next_index_bits >> 24;
                } while ((u32)trail_slot_index < saved_trail_scan_bound &&
                    *(s32 *)(sprite_slots + (trail_slot_index << 2)) == 0);
            }
            if (trail_slot_index ==
                    ((*(u32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) >> 1) + 0x19) &&
                    *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                DestroySpriteGroup(group_bytes);
            }
            break;
        }
        }
    }
}
