#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern s32 IsBattleAnimationSpriteAtFacingPosition() asm("func_080D2754");

void UpdateBattleTargetedYellowOrbTrailSprite(void *sprite) asm("func_080D98C0");

void UpdateBattleTargetedYellowOrbTrailSprite(void *sprite) {
    void *group = BATTLE_SPRITE_FIELD(sprite, void *, user_data.projectile_trail.group);
    s32 next_x;
    s32 trail_x;
    s32 impact_x;

    if (!(*(s32 *)sprite & BATTLE_SPRITE_FLIP_X)) {
        next_x = BATTLE_SPRITE_FIELD(sprite, u16, x) - 8;
    } else {
        next_x = BATTLE_SPRITE_FIELD(sprite, u16, x) + 8;
    }
    BATTLE_SPRITE_FIELD(sprite, u16, x) = next_x;
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) & 1)) {
        s32 *sprite_slots;
        s32 trail_slot_index;
        s32 trail_sprite;
        s32 trail_slot_offset;
        s32 current_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
        if (!(*(s32 *)sprite & BATTLE_SPRITE_FLIP_X)) {
            trail_x = (s16) (current_x + 0x10);
        } else {
            trail_x = (s16) (current_x - 0x10);
        }
        trail_sprite = CreateBattleAngledProjectileSprite(group, 1, 0, trail_x,
                (s32) BATTLE_SPRITE_FIELD(sprite, s16, y), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0x80, 0x80,
                (BATTLE_ANIMATION_SPRITE_REVERSE_FACING | BATTLE_ANIMATION_SPRITE_KEEP_SCREEN_X));
        trail_slot_index = (BATTLE_SPRITE_FIELD(sprite, u32, user_data.projectile_trail.elapsed_frames) >> 1) + 0x10;
        trail_slot_offset = trail_slot_index << 2;
        sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
        /* The original slot calculation can overwrite group state at slot32. */
        *(s32 *)((char *)sprite_slots + trail_slot_offset) = trail_sprite;
    }
    impact_x = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.impact_x);
    if (impact_x == 0) {
        if (!(*(s32 *)sprite & BATTLE_SPRITE_FLIP_X)) {
            if (BATTLE_SPRITE_FIELD(sprite, s16, x) >= 0) {
                goto advance_trail_time;
            }
            goto offscreen_orb;
        }
        if (BATTLE_SPRITE_FIELD(sprite, s16, x) > 0xEF) {
        offscreen_orb:
            if (impact_x != 0) {
                goto check_impact_position;
            }
            goto hide_orb;
        }
        goto advance_trail_time;
    }
check_impact_position:
    if ((IsBattleAnimationSpriteAtFacingPosition(sprite, impact_x - 0x10, BATTLE_SPRITE_FIELD(sprite, s16, y)) << 0x18) == 0) {
    advance_trail_time:
        BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) + 1;
        return;
    }
hide_orb:
    *(s32 *)sprite = *(s32 *)sprite | BATTLE_SPRITE_HIDDEN;
    BATTLE_SPRITE_FIELD(sprite, s32, update_callback) = 0;
}
