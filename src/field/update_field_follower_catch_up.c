#include "field_actor.h"

extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern struct FieldActorPositionHistory gFieldActorPositionHistory asm("D_020329AC");
extern u8 gFieldActorDirectionHistory[] asm("D_02032A6C");
extern void SetSpriteAnimation(s32, u16) asm("func_08094564");
extern u16 GetFieldActorAnimationIndex(void *) asm("func_080A9A54");
extern u8 FindFieldActorSlot(s32) asm("func_080A9EF0");
extern void RemoveFieldActor(u8) asm("func_080A9F40");

s32 UpdateFieldFollowerCatchUp(struct FieldActor *follower) asm("func_080AAD34");

s32 UpdateFieldFollowerCatchUp(struct FieldActor *follower)
{
    u8 old_direction;
    struct FieldActor *leader;
    u8 history_index;

    old_direction = follower->current_direction;
    leader = &gFieldActors[FindFieldActorSlot(0)];
    if (leader->flags & FIELD_ACTOR_ACTIVE) {
        follower->movement_mode = FIELD_ACTOR_NORMAL_SPEED;
        follower->world_x_fixed8 = gFieldActorPositionHistory.positions[0].world_x_fixed8;
        follower->world_y_fixed8 = gFieldActorPositionHistory.positions[0].world_y_fixed8;
        follower->current_direction = gFieldActorDirectionHistory[0];
        follower->requested_direction = gFieldActorDirectionHistory[0];
        history_index = 0;
        do {
            gFieldActorPositionHistory.positions[history_index].world_x_fixed8 = gFieldActorPositionHistory.positions[history_index + 1].world_x_fixed8;
            gFieldActorPositionHistory.positions[history_index].world_y_fixed8 = gFieldActorPositionHistory.positions[history_index + 1].world_y_fixed8;
            gFieldActorDirectionHistory[history_index] = gFieldActorDirectionHistory[history_index + 1];
            history_index++;
        } while (history_index <= FIELD_ACTOR_HISTORY_SHIFT_LAST);
        gFieldActorPositionHistory.positions[FIELD_ACTOR_HISTORY_LAST].world_x_fixed8 = leader->world_x_fixed8;
        gFieldActorPositionHistory.positions[FIELD_ACTOR_HISTORY_LAST].world_y_fixed8 = leader->world_y_fixed8;
        gFieldActorDirectionHistory[FIELD_ACTOR_HISTORY_LAST] = leader->current_direction;
    }

    {
        register s32 movement_mode_r0 asm("r0");
        movement_mode_r0 = follower->movement_mode;
        asm volatile("" : "+r"(movement_mode_r0));
        if (movement_mode_r0 == 0) goto idle_animation_check;
        if (movement_mode_r0 < 0) goto done_callback;
        if (movement_mode_r0 > 3) goto done_callback;
    }
    if (old_direction != follower->current_direction) goto moving_animation;
    if (follower->previous_movement_mode != 0) goto done_callback;
    goto moving_animation;

idle_animation_check:
    if (old_direction != follower->current_direction) goto idle_animation;
    if (follower->previous_movement_mode == 0) goto done_callback;

idle_animation:
    SetSpriteAnimation((s32)follower->sprite, GetFieldActorAnimationIndex(follower));
    goto done_callback;

moving_animation:
    SetSpriteAnimation((s32)follower->sprite, GetFieldActorAnimationIndex(follower));
done_callback:
    follower->previous_movement_mode = follower->movement_mode;
    if ((follower->world_x_fixed8 == leader->world_x_fixed8) && (follower->world_y_fixed8 == leader->world_y_fixed8)) {
        RemoveFieldActor(follower->actor_id);
        return 1;
    }
    return 0;
}
