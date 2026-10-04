#include "field_actor.h"

extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern struct FieldActorPositionHistory gFieldActorPositionHistory asm("D_020329AC");
extern u8 gFieldActorDirectionHistory[] asm("D_02032A6C");
extern void SetSpriteAnimation(s32, u16) asm("func_08094564");
extern u16 GetFieldActorAnimationIndex(void *) asm("func_080A9A54");
extern u8 FindFieldActorSlot(s32) asm("func_080A9EF0");

void UpdateFieldFollowerPositionHistory(struct FieldActor *follower) asm("func_080AABCC");

void UpdateFieldFollowerPositionHistory(struct FieldActor *follower) {
    u8 old_direction;
    struct FieldActor *leader;
    u8 history_index;

    old_direction = follower->current_direction;
    leader = &gFieldActors[FindFieldActorSlot(0)];
    if (leader->flags & FIELD_ACTOR_ACTIVE) {
        if (leader->movement_mode == 0) {
            follower->movement_mode = 0;
        } else {
            if ((follower->world_x_fixed8 != gFieldActorPositionHistory.positions[0].world_x_fixed8) ||
                (follower->world_y_fixed8 != gFieldActorPositionHistory.positions[0].world_y_fixed8)) {
                follower->movement_mode = 2;
            } else {
                follower->movement_mode = 0;
            }
            follower->world_x_fixed8 = gFieldActorPositionHistory.positions[0].world_x_fixed8;
            follower->world_y_fixed8 = gFieldActorPositionHistory.positions[0].world_y_fixed8;
            follower->current_direction = gFieldActorDirectionHistory[0];
            follower->requested_direction = gFieldActorDirectionHistory[0];
            asm volatile("" : : "r"(follower));
            asm volatile("" : : "r"(follower));
            history_index = 0;
            do {
                gFieldActorPositionHistory.positions[history_index].world_x_fixed8 = gFieldActorPositionHistory.positions[history_index + 1].world_x_fixed8;
                gFieldActorPositionHistory.positions[history_index].world_y_fixed8 = gFieldActorPositionHistory.positions[history_index + 1].world_y_fixed8;
                gFieldActorDirectionHistory[history_index] = gFieldActorDirectionHistory[history_index + 1];
                history_index++;
            } while (history_index <= FIELD_ACTOR_HISTORY_SHIFT_LAST);
            if (!(leader->flags & FIELD_ACTOR_TARGET_MOVEMENT_ACTIVE)) {
                gFieldActorPositionHistory.positions[FIELD_ACTOR_HISTORY_LAST].world_x_fixed8 = leader->world_x_fixed8;
                gFieldActorPositionHistory.positions[FIELD_ACTOR_HISTORY_LAST].world_y_fixed8 = leader->world_y_fixed8;
            } else {
                gFieldActorPositionHistory.positions[FIELD_ACTOR_HISTORY_LAST].world_x_fixed8 = leader->world_x_fixed8 +
                    (FIELD_ACTOR_SPRITE_FIELD(leader->sprite, s16 *, offset_x) << 8);
                gFieldActorPositionHistory.positions[FIELD_ACTOR_HISTORY_LAST].world_y_fixed8 = leader->world_y_fixed8 +
                    ((FIELD_ACTOR_SPRITE_FIELD(leader->sprite, s16 *, offset_y) + 2) << 8);
            }
            gFieldActorDirectionHistory[FIELD_ACTOR_HISTORY_LAST] = leader->current_direction;
        }
    }
    {
        register s32 movement_mode_r0 asm("r0");
        movement_mode_r0 = follower->movement_mode;
        asm volatile("" : "+r"(movement_mode_r0));
        if (movement_mode_r0 == 0) goto idle_animation_check;
        if (movement_mode_r0 < 0) goto done;
        if (movement_mode_r0 > 3) goto done;
    }
    if (old_direction != follower->current_direction) goto moving_animation;
    if (follower->previous_movement_mode != 0) goto done;
    goto moving_animation;

idle_animation_check:
    if (old_direction != follower->current_direction) goto idle_animation;
    if (follower->previous_movement_mode == 0) goto done;
idle_animation:
    SetSpriteAnimation((s32)follower->sprite, GetFieldActorAnimationIndex(follower));
    goto done;

moving_animation:
    SetSpriteAnimation((s32)follower->sprite, GetFieldActorAnimationIndex(follower));
done:
    follower->previous_movement_mode = follower->movement_mode;
}
