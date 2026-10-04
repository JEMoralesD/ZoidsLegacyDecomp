#include "field_actor.h"
extern struct FieldActor gFieldActors[] asm("D_020325A0");

u32 FindFieldActorSlot(u8 actor_id) asm("func_080A9EF0");

u32 FindFieldActorSlot(u8 actor_id) {
    u8 slot_index = 0;
    if (!(gFieldActors[slot_index].flags & FIELD_ACTOR_ACTIVE) || gFieldActors[slot_index].actor_id != actor_id) {
    next_slot:
        slot_index++;
        if ((u32)slot_index <= FIELD_ACTOR_MAX_SLOT) {
            if ((gFieldActors[slot_index].flags & FIELD_ACTOR_ACTIVE) && gFieldActors[slot_index].actor_id == actor_id) {
                goto found_actor;
            }
            goto next_slot;
        }
        goto not_found;
    }
found_actor:
    if ((u32)slot_index > FIELD_ACTOR_MAX_SLOT) {
    not_found:
        return FIELD_ACTOR_SLOT_NOT_FOUND;
    }
    return (u32)slot_index;
}
