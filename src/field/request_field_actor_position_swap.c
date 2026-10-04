#include "field_actor.h"
void RequestFieldActorPositionSwap(struct FieldActor *first_actor, struct FieldActor *second_actor) asm("func_080AAE60");

void RequestFieldActorPositionSwap(struct FieldActor *first_actor, struct FieldActor *second_actor) {
    FIELD_ACTOR_FIELD(first_actor, s32 *, behavior_state.pending_target.world_x_fixed8) = (s32) FIELD_ACTOR_FIELD(second_actor, s32 *, world_x_fixed8);
    FIELD_ACTOR_FIELD(first_actor, s32 *, behavior_state.pending_target.world_y_fixed8) = (s32) FIELD_ACTOR_FIELD(second_actor, s32 *, world_y_fixed8);
    FIELD_ACTOR_FIELD(second_actor, s32 *, behavior_state.pending_target.world_x_fixed8) = (s32) FIELD_ACTOR_FIELD(first_actor, s32 *, world_x_fixed8);
    FIELD_ACTOR_FIELD(second_actor, s32 *, behavior_state.pending_target.world_y_fixed8) = (s32) FIELD_ACTOR_FIELD(first_actor, s32 *, world_y_fixed8);
    FIELD_ACTOR_FIELD(first_actor, s32 *, flags) = (s32) (FIELD_ACTOR_FIELD(first_actor, s32 *, flags) | FIELD_ACTOR_TARGET_MOVEMENT_PENDING);
    FIELD_ACTOR_FIELD(second_actor, s32 *, flags) = (s32) (FIELD_ACTOR_FIELD(second_actor, s32 *, flags) | FIELD_ACTOR_TARGET_MOVEMENT_PENDING);
}
