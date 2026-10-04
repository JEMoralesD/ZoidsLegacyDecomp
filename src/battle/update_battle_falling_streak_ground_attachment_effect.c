#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s16 Sin256(s32) asm("func_08092A90");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");
extern struct SpriteBackgroundScrollOffsets gSpriteBackgroundScroll[] asm("D_03000054");

void UpdateBattleFallingStreakGroundAttachmentEffect(struct BattleAnimationGroup *group) asm("func_080DC290");

void UpdateBattleFallingStreakGroundAttachmentEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;

    switch (*(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state))) {
    case BATTLE_FALLING_STREAK_GROUND_LAUNCH: {
        s32 x;
        s32 sine_fixed8;
        s32 launch_y_work;
        s32 y;

        x = (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) + ((CallFunctionR0(gRandomNumberCallback) * 0x41) >> 15) - 0x20);
        sine_fixed8 = (s16)Sin256(0x36);
        launch_y_work = sine_fixed8 << 1;
        launch_y_work += sine_fixed8;
        launch_y_work <<= 6;
        if (launch_y_work < 0) {
            launch_y_work += 0xFF;
        }
        launch_y_work >>= 8;
        launch_y_work = 0x80 - launch_y_work;
        if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.falling_streak_ground.depth_variant)) != 0) {
            y = (s16)(launch_y_work - 0x10);
        } else {
            y = (s16)launch_y_work;
        }
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAngledProjectileSprite(group_bytes, 0, 0, x, y, (BATTLE_SPRITE_LOOP_ANIMATION | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0x4A, 0xC00, 1);
        *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state)) += 1;
        break;
    }
    case BATTLE_FALLING_STREAK_GROUND_ATTACH_TO_BACKGROUND: {
        s32 *elapsed_updates_slot = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.falling_streak_ground.elapsed_updates));
        char *streak_sprite;
        s32 x;
        s32 no_callback;

        *elapsed_updates_slot += 1;
        if (*elapsed_updates_slot != BATTLE_FALLING_STREAK_FLIGHT_UPDATES) {
            break;
        }
        if ((BATTLE_ANIMATION_FIELD(group_bytes, s32, flags) & BATTLE_ANIMATION_GROUP_MIRRORED) == 0) {
            streak_sprite = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            x = BATTLE_SPRITE_FIELD(streak_sprite, s16, x);
        } else {
            register char *mirrored_streak_sprite asm("r2") = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            x = (s16)(0xF0 - BATTLE_SPRITE_FIELD(mirrored_streak_sprite, u16, x));
            streak_sprite = mirrored_streak_sprite;
            asm volatile("" : "+&r"(streak_sprite) : "r"(mirrored_streak_sprite));
        }
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, x, BATTLE_SPRITE_FIELD(streak_sprite, s16, y), BATTLE_SPRITE_SEMITRANSPARENT, 0xC0, 0x100, (no_callback = 0));
        {
            char *streak_x_view = *(char * volatile *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            BATTLE_SPRITE_FIELD(streak_x_view, u16, x) = gSpriteBackgroundScroll[2].x_fixed8 / 0x100 + BATTLE_SPRITE_FIELD(streak_x_view, u16, x);
        }
        {
            char *streak_y_view = *(char * volatile *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            BATTLE_SPRITE_FIELD(streak_y_view, u16, y) = gSpriteBackgroundScroll[2].y_fixed8 / 0x100 + BATTLE_SPRITE_FIELD(streak_y_view, u16, y);
        }
        {
            char *streak_callback_view = *(char * volatile *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            BATTLE_SPRITE_FIELD(streak_callback_view, s32, flags) |= (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_BACKGROUND_2);
            BATTLE_SPRITE_FIELD(streak_callback_view, s32, update_callback) = no_callback;
        }
        *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state)) += 1;
        break;
    }
    case BATTLE_FALLING_STREAK_GROUND_WAIT_FOR_SCREEN_EXIT: {
        char *streak_sprite = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));

        if (streak_sprite == 0) {
            goto destroy_group;
        }
        if ((BATTLE_ANIMATION_FIELD(group_bytes, s32, flags) & BATTLE_ANIMATION_GROUP_MIRRORED) == 0) {
            if (BATTLE_SPRITE_FIELD(streak_sprite, s16, x) - gSpriteBackgroundScroll[2].x_fixed8 / 0x100 > 0x100) {
                goto destroy_group;
            }
            break;
        } else {
            if (BATTLE_SPRITE_FIELD(streak_sprite, s16, x) - gSpriteBackgroundScroll[2].x_fixed8 / 0x100 >= -0x10) {
                break;
            }
        }
    destroy_group:
        DestroySpriteGroup(group_bytes);
        break;
    }
    }
}
