#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

s32 CreateBattleAngledProjectileSprite(s32, s32, s32, s16, s32, s32, s32, s32, s32) asm("func_080D2660");
int IsBattleAnimationSpriteAtFacingPosition(void *, s32, s16) asm("func_080D2754");

void UpdateBattleTargetedFallingMissileTrailSprite(struct BattleDisplaySprite *sprite) asm("func_080D6E6C");

void UpdateBattleTargetedFallingMissileTrailSprite(struct BattleDisplaySprite *sprite) {
    s32 group_address = BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.group);
    s32 impact_x;
    int next_x;
    int previous_x;
    s32 trail_sprite;
    int trail_slot_index;
    int sprite_offset;
    char *sprite_slots;

    if (!(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X)) {
        next_x = sprite->x - 0x10;
    } else {
        next_x = sprite->x + 0x10;
    }
    sprite->x = next_x;
    sprite->y = sprite->y + 8;
    if (!(BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) & 1)) {
        previous_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
        trail_sprite = CreateBattleAngledProjectileSprite(
            group_address,
            1,
            0,
            !(BATTLE_SPRITE_FIELD(sprite, s32, flags) & BATTLE_SPRITE_FLIP_X) ? (previous_x + 0x20) : (previous_x - 0x20),
            (s32)(s16)(sprite->y - 0x10),
            (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1),
            -0x13,
            0x80,
            3
        );
        trail_slot_index = ((u32)BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) >> 1) + 0x18;
        sprite_offset = trail_slot_index << 2;
        sprite_slots = (char *)(group_address + BATTLE_ANIMATION_OFFSET(sprites[0]));
        *(s32 *)(sprite_slots + sprite_offset) = trail_sprite;
    }
    impact_x = BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.impact_x);
    if (impact_x == 0) {
        if (BATTLE_SPRITE_FIELD(sprite, s16, y) <= 0x6F) {
            goto advance_trail;
        }
        goto check_impact;
    }
    if ((IsBattleAnimationSpriteAtFacingPosition(sprite, impact_x - 0x10, BATTLE_SPRITE_FIELD(sprite, s16, y)) << 0x18) == 0) {
advance_trail:
        BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) = BATTLE_SPRITE_FIELD(sprite, s32, user_data.missile_trail.elapsed_frames) + 1;
        return;
    }
check_impact:
    BATTLE_SPRITE_FIELD(sprite, s32, flags) = BATTLE_SPRITE_FIELD(sprite, s32, flags) | BATTLE_SPRITE_HIDDEN;
    sprite->update_callback = 0;
}

void InitializeBattleAcceleratingRisingMissileEffect(void *group) asm("func_080D6F34");

void InitializeBattleAcceleratingRisingMissileEffect(void *group) {
    *(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state)) = 0;
}
