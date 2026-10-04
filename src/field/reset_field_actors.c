#include "field_actor.h"
extern u8 gFieldActorStorage[] asm("D_020325A0");
extern u8 gInteractedFieldActorId asm("D_02032998");
extern s32 gControlledFieldActor asm("D_02032990");
void ResetFieldActors(void) asm("func_080A9888");

void ResetFieldActors(void) {
    u8 slot_index = 0;
    do {
        *(s32 *)&gFieldActorStorage[slot_index * FIELD_ACTOR_RECORD_BYTES] = 0;
        slot_index++;
    } while (slot_index <= FIELD_ACTOR_MAX_SLOT);
    gInteractedFieldActorId = FIELD_ACTOR_SLOT_NOT_FOUND;
    gControlledFieldActor = 0;
}
