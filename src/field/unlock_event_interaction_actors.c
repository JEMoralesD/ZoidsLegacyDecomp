#include "field_actor.h"
extern u8 gFieldActors asm("D_020325A0");
extern u8 gInteractedFieldActorId asm("D_02032998");
void UnlockEventInteractionActors(void) asm("func_080A0114");

void UnlockEventInteractionActors(void) {
    s32 actor_offset;
    s32 actor_flags;
    u8 actor_slot;
    void *actor;

    if (*(u8 *)((u32)&gInteractedFieldActorId) != 0xFF) {
        actor_slot = 0;
        do {
            actor_offset = actor_slot * FIELD_ACTOR_RECORD_BYTES;
            actor = actor_offset + ((u32)&gFieldActors);
            actor_flags = M2C_FIELD(actor_offset, s32 *, ((u32)&gFieldActors));
            if ((1 & actor_flags) && ((M2C_FIELD(actor, u16 *, FIELD_ACTOR_OFFSET(behavior)) == 0) || (M2C_FIELD(actor, u8 *, FIELD_ACTOR_OFFSET(actor_id)) == *(u8 *)((u32)&gInteractedFieldActorId)))) {
                M2C_FIELD(actor_offset, s32 *, ((u32)&gFieldActors)) = (s32) (actor_flags & ~FIELD_ACTOR_INPUT_LOCKED);
            }
            actor_slot += 1;
        } while ((u32) actor_slot <= FIELD_ACTOR_MAX_SLOT);
        *(u8 *)((u32)&gInteractedFieldActorId) = 0xFF;
    }
}
