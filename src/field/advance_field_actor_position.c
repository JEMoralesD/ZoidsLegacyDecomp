#include "field_actor.h"

extern u16 gCurrentMapId asm("D_0202ECF4");
void AdvanceFieldActorPosition(struct FieldActor *actor) asm("func_080ABE3C");

void AdvanceFieldActorPosition(struct FieldActor *actor) {
    s32 next_y_fixed8;
    s32 next_x_fixed8;

    next_x_fixed8 = FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8) + FIELD_ACTOR_FIELD(actor, s32 *, velocity_x_fixed8);
    FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8) = next_x_fixed8;
    next_y_fixed8 = FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8) + FIELD_ACTOR_FIELD(actor, s32 *, velocity_y_fixed8);
    FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8) = next_y_fixed8;
    if (gCurrentMapId == FIELD_MAP_PACKED_CELLS) {
        FIELD_ACTOR_FIELD(actor, s32 *, world_x_fixed8) = (s32) (next_x_fixed8 & 0xFFFFF);
        FIELD_ACTOR_FIELD(actor, s32 *, world_y_fixed8) = (s32) (next_y_fixed8 & 0xFFFFF);
    }
}
