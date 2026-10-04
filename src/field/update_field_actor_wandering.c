#include "field_actor.h"
u32 CallFunctionR0(s32) asm("func_80ECD5C");                                 /* extern */

void UpdateFieldActorWandering(struct FieldActor *actor) asm("func_080AA4E0");

void UpdateFieldActorWandering(struct FieldActor *actor) {
    s32 move_timer;
    s32 next_timer;

    if (!(FIELD_ACTOR_FIELD(actor, s32 *, flags) & FIELD_ACTOR_INPUT_LOCKED) && (*(s32 *)0x02032990 != 0)) {
        if (FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) == 0) {
            if ((CallFunctionR0(*(s32 *)0x03000010) >> 8) == 0) {
                FIELD_ACTOR_FIELD(actor, s8 *, requested_direction) = (s8) ((CallFunctionR0(*(s32 *)0x03000010) >> 0xD) * 2);
                FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = FIELD_ACTOR_NORMAL_SPEED;
                next_timer = (CallFunctionR0(*(s32 *)0x03000010) >> 8) + 0x40;
                goto save_move_timer;
            }
            return;
        }
        if (!((FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8) == 0) && (FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8) == 0))) {
            move_timer = FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.wandering.timer);
            if (move_timer != 0) {
                goto decrement;
            }
        }
    }
stop_wandering:
    FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) = FIELD_ACTOR_IDLE;
    return;
decrement:
    next_timer = move_timer - 1;
save_move_timer:
    FIELD_ACTOR_FIELD(actor, s32 *, behavior_state.wandering.timer) = next_timer;
}
