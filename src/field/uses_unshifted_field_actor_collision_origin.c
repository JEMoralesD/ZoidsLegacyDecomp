#include "field_actor.h"
s32 UsesUnshiftedFieldActorCollisionOrigin(struct FieldActor *actor) asm("func_080AC098");

s32 UsesUnshiftedFieldActorCollisionOrigin(struct FieldActor *actor) {
    s32 uses_unshifted_origin;
    u8 model_id;

    uses_unshifted_origin = 0;
    model_id = FIELD_ACTOR_FIELD(actor, u8 *, model_id);
    if ((model_id == 0x69) || (model_id == 0x6E) || (model_id == 0x6F) || (model_id == 0x6A) || (model_id == 0x6B) || (model_id == 0x6C) || (model_id == 0x8D) || (model_id == 0x8E)) {
        uses_unshifted_origin = 1;
    }
    return uses_unshifted_origin;
}
