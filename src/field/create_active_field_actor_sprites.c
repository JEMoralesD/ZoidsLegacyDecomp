#include "field_actor.h"
M2C_UNK InitializeFieldActorSprites(s32) asm("func_080A9AFC");                             /* extern */

void CreateActiveFieldActorSprites(void) asm("func_080A9EBC");

void CreateActiveFieldActorSprites(void) {
    s32 slot_offset;
    u8 slot_index;

    slot_index = 0;
    do {
        slot_offset = slot_index * FIELD_ACTOR_RECORD_BYTES;
        if (M2C_FIELD(slot_offset, s32 *, FIELD_ACTORS_RAM) & FIELD_ACTOR_ACTIVE) {
            InitializeFieldActorSprites(slot_offset + FIELD_ACTORS_RAM);
        }
        slot_index += 1;
    } while ((u32) slot_index <= (u32)FIELD_ACTOR_MAX_SLOT);
}
