#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void UpdateBattleTargetedRisingMissileTrailSprite(void *) asm("func_080D6D84");
extern void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");

extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");

void UpdateBattleRandomOffsetSelectableRisingMissileTrailEffect(struct BattleAnimationGroup *group) asm("func_080DE2C4");

void UpdateBattleRandomOffsetSelectableRisingMissileTrailEffect(struct BattleAnimationGroup *group)
{
    s32 phase = *(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
    void *missile_sprite_view;
    u32 trail_elapsed_updates;
    u32 trail_scan_bound;
    u8 trail_slot_index;
    s32 sprite_slot_offset;

    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        if (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.use_diagonal_motion)) == 0) {
            void *missile_sprite;

            missile_sprite = CreateBattleAnimationSprite(
                group, 2, 0, *(s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(x)),
                (s32)(s16)(*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels)) + 0x50),
                0x20, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
            *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.group)) = group;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) = phase;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.impact_x)) = phase;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.trail_resource_slot_base)) = 2;
        } else {
            void *missile_sprite;

            missile_sprite = CreateBattleAnimationSprite(
                group, 0, 0,
                (s16)(*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(x)) +
                    (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.random_offset_selectable_missile.launch_offset_pixels)) * 2)),
                0x80, 0x20, (s32)UpdateBattleTargetedRisingMissileTrailSprite, 1);
            *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.group)) = group;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) = phase;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.missile_trail.impact_x)) = phase;
        }
        PlayBattleAnimationSound(0);
        {
            s32 *effect_state_slot = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
            *effect_state_slot = *effect_state_slot + 1;
        }
        return;
    }

    {
        register void *primary_sprite asm("r0") = *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
        s32 hidden_flag = *(s32 *)primary_sprite & BATTLE_SPRITE_HIDDEN;
        missile_sprite_view = primary_sprite;
        if (hidden_flag) {
            trail_slot_index = 0x18;
            asm volatile("" : "+r"(trail_slot_index));
            trail_elapsed_updates = *(u32 *)((char *)missile_sprite_view + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames));
            trail_scan_bound = (trail_elapsed_updates >> 1) + 0x19;
            if (((u32)trail_slot_index < trail_scan_bound) &&
                (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0)) {
                u32 saved_trail_scan_bound = trail_scan_bound;
                s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));

                do {
                    trail_slot_index += 1;
                } while (((u32)trail_slot_index < saved_trail_scan_bound) &&
                    ((sprite_slot_offset = trail_slot_index << 2,
                      *(s32 *)((char *)sprite_slots + sprite_slot_offset)) == 0));
            }
            if (trail_slot_index ==
                ((*(volatile u32 *)((char *)missile_sprite_view + BATTLE_SPRITE_OFFSET(user_data.missile_trail.elapsed_frames)) >> 1) + 0x19)) {
                DestroySpriteGroup(group);
            }
        }
    }
}
