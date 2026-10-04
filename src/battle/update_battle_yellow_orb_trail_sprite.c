#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

s32 CreateBattleAngledProjectileSprite(s32, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_080D2660");
int IsBattleAnimationSpriteAtFacingPosition(void *, s32, s16) asm("func_080D2754");

void UpdateBattleYellowOrbTrailSprite(struct BattleDisplaySprite *sprite) asm("func_080D980C");

void UpdateBattleYellowOrbTrailSprite(struct BattleDisplaySprite *sprite) {
    s32 group_address = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.group);
    int next_x;
    int current_x;
    s32 trail_sprite;
    int trail_slot_index;
    int trail_slot_offset;
    char *sprite_slots_base;

    if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
        next_x = sprite->x - 8;
    } else {
        next_x = sprite->x + 8;
    }
    sprite->x = next_x;
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) & 1)) {
        current_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
        trail_sprite = CreateBattleAngledProjectileSprite(
            group_address, 1, 0,
            !(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)
                ? (current_x + 0x10) : (current_x - 0x10),
            (s32)(s16)sprite->y,
            (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1),
            0x80, 0x80, BATTLE_ANIMATION_SPRITE_KEEP_SCREEN_X
        );
        trail_slot_index = ((u32)BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) >> 1) + 5;
        trail_slot_offset = trail_slot_index << 2;
        sprite_slots_base = (char *)(group_address + BATTLE_ANIMATION_OFFSET(sprites[0]));
        /* The original slot calculation can overwrite group state at slot32. */
        *(s32 *)(sprite_slots_base + trail_slot_offset) = trail_sprite;
        if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
            if (BATTLE_SPRITE_FIELD(sprite, s16, x) < 0) {
                goto hide_orb;
            }
            goto advance_trail_time;
        }
        if (BATTLE_SPRITE_FIELD(sprite, s16, x) > 0xEF) {
hide_orb:
            BATTLE_SPRITE_FIELD(sprite, s32, flags) = BATTLE_SPRITE_FIELD(sprite, s32, flags) | BATTLE_SPRITE_HIDDEN;
            BATTLE_SPRITE_FIELD(sprite, s32, update_callback) = 0;
            return;
        }
        goto advance_trail_time;
    }
advance_trail_time:
    BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) = BATTLE_SPRITE_FIELD(sprite, s32, user_data.projectile_trail.elapsed_frames) + 1;
}
