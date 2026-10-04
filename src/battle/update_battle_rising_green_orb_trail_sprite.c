#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

s32 CreateBattleAngledProjectileSprite(s32, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_080D2660");
int IsBattleAnimationSpriteAtFacingPosition(void *, s32, s16) asm("func_080D2754");

void UpdateBattleRisingGreenOrbTrailSprite(struct BattleDisplaySprite *sprite) asm("func_080D8B70");

void UpdateBattleRisingGreenOrbTrailSprite(struct BattleDisplaySprite *sprite) {
    s32 group_address = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.group);
    int next_x;
    int previous_x;
    s32 trail_sprite;
    int trail_slot_index;
    int sprite_offset;
    char *sprite_slots;

    if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
        next_x = sprite->x - 8;
    } else {
        next_x = sprite->x + 8;
    }
    sprite->x = next_x;
    sprite->y = sprite->y - 4;
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) & 1)) {
        previous_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
        trail_sprite = CreateBattleAngledProjectileSprite(
            group_address,
            1,
            0,
            !(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X) ? (previous_x + 0x10) : (previous_x - 0x10),
            (s32)(s16)(sprite->y + 8),
            (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1),
            0x93,
            0x80,
            2
        );
        trail_slot_index = ((u32)BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) >> 1) + 5;
        sprite_offset = trail_slot_index << 2;
        sprite_slots = (char *)(group_address + BATTLE_ANIMATION_OFFSET(sprites[0]));
        /* Native index arithmetic can overwrite group state at sprite slot 32. */
        *(s32 *)(sprite_slots + sprite_offset) = trail_sprite;
        if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
            if (BATTLE_SPRITE_FIELD(sprite, s16, x) < 0) {
                goto check_top_bound;
            }
            goto advance_trail;
        }
        if (BATTLE_SPRITE_FIELD(sprite, s16, x) > 0xEF) {
check_top_bound:
            /* Native exit requires both an offscreen x and a negative y. */
            if (BATTLE_SPRITE_FIELD(sprite, s16, y) < 0) {
                BATTLE_SPRITE_FIELD(sprite, s32, flags) = BATTLE_SPRITE_FIELD(sprite, s32, flags) | BATTLE_SPRITE_HIDDEN;
                sprite->update_callback = 0;
                return;
            }
            goto advance_trail;
        }
        goto advance_trail;
    }
advance_trail:
    BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) + 1;
}

void UpdateBattleTargetedRisingGreenOrbTrailSprite(struct BattleDisplaySprite *sprite) asm("func_080D8C38");

void UpdateBattleTargetedRisingGreenOrbTrailSprite(struct BattleDisplaySprite *sprite) {
    s32 group_address = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.group);
    s32 impact_x;
    int next_x;
    int previous_x;
    s32 trail_sprite;
    int trail_slot_index;
    int sprite_offset;
    char *sprite_slots;

    if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
        next_x = sprite->x - 8;
    } else {
        next_x = sprite->x + 8;
    }
    sprite->x = next_x;
    sprite->y = sprite->y - 4;
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) & 1)) {
        previous_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
        trail_sprite = CreateBattleAngledProjectileSprite(
            group_address,
            1,
            0,
            !(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X) ? (previous_x + 0x10) : (previous_x - 0x10),
            (s32)(s16)(sprite->y + 8),
            (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1),
            0x93,
            0x80,
            3
        );
        trail_slot_index = ((u32)BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) >> 1) + 16;
        sprite_offset = trail_slot_index << 2;
        sprite_slots = (char *)(group_address + BATTLE_ANIMATION_OFFSET(sprites[0]));
        /* Native index arithmetic can overwrite group state at sprite slot 32. */
        *(s32 *)(sprite_slots + sprite_offset) = trail_sprite;
    }
    impact_x = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.impact_x);
    if (impact_x != 0) {
        goto check_target_position;
    }
    if (BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X) {
        goto check_right_bound;
    }
    if (BATTLE_SPRITE_FIELD(sprite, s16, x) >= 0) {
        goto check_top_bound;
    }
    goto check_impact;
check_right_bound:
    if (BATTLE_SPRITE_FIELD(sprite, s16, x) > 0xEF) {
        goto check_impact;
    }
check_top_bound:
    if (BATTLE_SPRITE_FIELD(sprite, s16, y) >= 0) {
        goto advance_trail;
    }
check_impact:
    if (impact_x == 0) {
        goto hide_orb;
    }
check_target_position:
    if ((IsBattleAnimationSpriteAtFacingPosition(sprite, impact_x - 16, BATTLE_SPRITE_FIELD(sprite, s16, y)) << 0x18) != 0) {
        goto hide_orb;
    }
advance_trail:
    BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) + 1;
    return;
hide_orb:
    BATTLE_SPRITE_FIELD(sprite, s32, flags) = BATTLE_SPRITE_FIELD(sprite, s32, flags) | BATTLE_SPRITE_HIDDEN;
    sprite->update_callback = 0;
}
