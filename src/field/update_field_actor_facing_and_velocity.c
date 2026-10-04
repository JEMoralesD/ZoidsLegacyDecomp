#include "field_actor.h"

extern s32 gFieldActorDirectionVectors[][2] asm("D_087A1B98");
extern u16 GetFieldActorAnimationIndex(struct FieldActor *) asm("func_080A9A54");
extern void SetSpriteAnimation(void *, u16) asm("func_08094564");

void UpdateFieldActorFacingAndVelocity(struct FieldActor *actor) asm("func_080AA0B8");

void UpdateFieldActorFacingAndVelocity(struct FieldActor *actor)
{
    u8 old_direction = actor->current_direction;

    if (actor->requested_direction != old_direction) {
        if (++actor->turn_timer != FIELD_ACTOR_TURN_INTERVAL) goto timer_done;
        {
            if (actor->requested_direction > actor->current_direction) {
                if (actor->requested_direction - actor->current_direction <= 3) {
                    actor->current_direction = actor->current_direction + 1;
                } else {
                    actor->current_direction = (actor->current_direction - 1) & 7;
                }
            } else {
                if (actor->current_direction - actor->requested_direction <= 4) {
                    actor->current_direction = actor->current_direction - 1;
                } else {
                    actor->current_direction = (actor->current_direction + 1) & 7;
                }
            }
        }
    }
    actor->turn_timer = 0;
timer_done:

    {
        register s32 mode_r0 asm("r0");
        mode_r0 = actor->movement_mode;
        asm volatile("" : "+r"(mode_r0));
        if (mode_r0 == 0) goto mode_zero;
        if (mode_r0 < 0) goto mode_done;
        if (mode_r0 > 3) goto mode_done;
    }
    if (old_direction != actor->current_direction) goto moving_animation;
    if (actor->previous_movement_mode != 0) goto mode_switch;
    goto moving_animation;

mode_zero:
    if (old_direction != actor->current_direction) goto idle_animation;
    if (actor->previous_movement_mode == 0) goto zero_finish;
idle_animation:
    SetSpriteAnimation(actor->sprite, GetFieldActorAnimationIndex(actor));
zero_finish:
    actor->velocity_y_fixed8 = 0;
    actor->velocity_x_fixed8 = 0;
    goto mode_done;

moving_animation:
    SetSpriteAnimation(actor->sprite, GetFieldActorAnimationIndex(actor));
mode_switch:
    switch (actor->movement_mode) {
    case FIELD_ACTOR_HALF_SPEED:
        actor->velocity_x_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][0] / 2;
        actor->velocity_y_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][1] / 2;
        break;
    case FIELD_ACTOR_NORMAL_SPEED:
        actor->velocity_x_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][0];
        actor->velocity_y_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][1];
        break;
    case FIELD_ACTOR_DOUBLE_SPEED:
        actor->velocity_x_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][0] * 2;
        actor->velocity_y_fixed8 = gFieldActorDirectionVectors[actor->requested_direction][1] * 2;
        break;
    }
mode_done:
    actor->previous_movement_mode = actor->movement_mode;
}
