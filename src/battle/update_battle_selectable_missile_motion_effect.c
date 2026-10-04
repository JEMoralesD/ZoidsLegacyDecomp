#include "m2c_prelude.h"
extern void UpdateBattleAcceleratingRisingMissileSprite(void *) asm("func_080D6BDC");
extern void UpdateBattleAcceleratingMissileSprite(void *) asm("func_080D624C");
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");

void UpdateBattleSelectableMissileMotionEffect(struct BattleAnimationGroup *group) asm("func_080DDE0C");

void UpdateBattleSelectableMissileMotionEffect(struct BattleAnimationGroup *group) {
    s32 phase = *(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
    void *missile_sprite_view;
    u32 trail_elapsed_updates;
    u32 trail_scan_bound;
    u8 trail_slot_index;
    s32 trail_slot_offset;

    if (phase == BATTLE_CONTACT_EFFECT_CREATE) {
        if (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.missile_motion_variant.use_rising_motion)) == 0) {
            void *missile_sprite = CreateBattleAnimationSprite(group, 3, 0, *(s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(x)),
                (s32) *(s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(y)), 0x20, (s32)UpdateBattleAcceleratingMissileSprite, phase);
            *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.accelerating_missile.group)) = group;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.accelerating_missile.elapsed_frames)) = phase;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.accelerating_missile.trail_resource_slot_base)) = 3;
        } else {
            void *missile_sprite = CreateBattleAnimationSprite(group, 0, 0, *(s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(x)),
                (s32) *(s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(y)), 0x20, (s32)UpdateBattleAcceleratingRisingMissileSprite, phase);
            *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile_sprite;
            *(void **)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.accelerating_rising_missile.group)) = group;
            *(s32 *)((char *)missile_sprite + BATTLE_SPRITE_OFFSET(user_data.accelerating_rising_missile.elapsed_frames)) = phase;
        }
        PlayBattleAnimationSound(0);
        *(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state)) = *(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state)) + 1;
        return;
    }
    {
    register void *primary_sprite asm("r0") = *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
    s32 hidden_flag = *(s32 *)primary_sprite & BATTLE_SPRITE_HIDDEN;
    missile_sprite_view = primary_sprite;
    if (hidden_flag) {
        trail_elapsed_updates = *(u32 *)((char *)missile_sprite_view + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames));
        if (trail_elapsed_updates <= 0xFU) {
            trail_slot_index = 1;
            trail_scan_bound = (trail_elapsed_updates >> 2) + 2;
            if (((u32) trail_slot_index < trail_scan_bound) && (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0)) {
                u32 saved_trail_scan_bound = trail_scan_bound;
                s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    trail_slot_index += 1;
                } while (((u32) trail_slot_index < saved_trail_scan_bound) && ((trail_slot_offset = trail_slot_index << 2, *(s32 *)((char *)sprite_slots + trail_slot_offset)) == 0));
            }
            if (trail_slot_index == ((*(volatile u32 *)((char *)missile_sprite_view + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames)) >> 2) + 2)) {
                DestroySpriteGroup(group);
            }
        } else {
            trail_slot_index = 1;
            trail_scan_bound = ((trail_elapsed_updates - 0x10) >> 1) + 6;
            if (((u32) trail_slot_index < trail_scan_bound) && (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0)) {
                u32 saved_trail_scan_bound = trail_scan_bound;
                s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    trail_slot_index += 1;
                } while (((u32) trail_slot_index < saved_trail_scan_bound) && ((trail_slot_offset = trail_slot_index << 2, *(s32 *)((char *)sprite_slots + trail_slot_offset)) == 0));
            }
            if (trail_slot_index == (((*(volatile u32 *)((char *)missile_sprite_view + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames)) - 0x10) >> 1) + 6)) {
                DestroySpriteGroup(group);
            }
        }
    }
    }
}
