#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern s32 IsBattleAnimationSpriteAtFacingPosition() asm("func_080D2754");

void UpdateBattleTargetedRisingMissileTrailSprite(void *sprite) asm("func_080D6D84");

void UpdateBattleTargetedRisingMissileTrailSprite(void *sprite) {
    void *group_bytes = BATTLE_SPRITE_FIELD(sprite, void *, user_data.missile_trail.group);
    s32 next_x;
    s32 trail_x;
    s32 impact_x;

    if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
        next_x = BATTLE_SPRITE_FIELD(sprite, u16, x) - 0x10;
    } else {
        next_x = BATTLE_SPRITE_FIELD(sprite, u16, x) + 0x10;
    }
    BATTLE_SPRITE_FIELD(sprite, u16, x) = next_x;
    BATTLE_SPRITE_FIELD(sprite, u16, y) = BATTLE_SPRITE_FIELD(sprite, u16, y) - 8;
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) & 1)) {
        s32 *sprite_slots;
        s32 trail_slot_index;
        s32 trail_sprite;
        s32 sprite_offset;
        s32 previous_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
        if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
            trail_x = (s16) (previous_x + 0x20);
        } else {
            trail_x = (s16) (previous_x - 0x20);
        }
        trail_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, trail_x,
                (s16) (BATTLE_SPRITE_FIELD(sprite, u16, y) + 0x10), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0x13, 0x80, 3);
        trail_slot_index = (BATTLE_SPRITE_FIELD(sprite, u32, user_data.missile_trail.elapsed_frames) >> 1) + 0x18;
        sprite_offset = trail_slot_index << 2;
        sprite_slots = (s32 *)((char *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        *(s32 *)((char *)sprite_slots + sprite_offset) = trail_sprite;
    }
    impact_x = BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.impact_x);
    if (impact_x == 0) {
        if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
            if (BATTLE_SPRITE_FIELD(sprite, s16, x) >= 0) {
                goto check_y;
            }
            goto check_impact;
        }
        if (BATTLE_SPRITE_FIELD(sprite, s16, x) > 0xEF) {
            goto check_impact;
        }
    check_y:
        if (BATTLE_SPRITE_FIELD(sprite, s16, y) >= 0) {
            goto advance_trail;
        }
    }
check_impact:
    if (impact_x != 0) {
        goto check_target_position;
    }
    goto hide_missile;
check_target_position:
    if ((IsBattleAnimationSpriteAtFacingPosition(sprite, impact_x - 0x10, BATTLE_SPRITE_FIELD(sprite, s16, y)) << 0x18) == 0) {
    advance_trail:
        BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) = BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) + 1;
        return;
    }
hide_missile:
    BATTLE_SPRITE_FIELD(sprite, s32, flags) = BATTLE_SPRITE_FIELD(sprite, s32, flags) | BATTLE_SPRITE_HIDDEN;
    BATTLE_SPRITE_FIELD(sprite, s32, update_callback) = 0;
}
