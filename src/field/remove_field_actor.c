#include "field_actor.h"
extern u8 FindFieldActorSlot(u8) asm("func_080A9EF0");
extern void DestroySprite(s32) asm("func_8094554");

extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern struct FieldActor *gControlledFieldActor asm("D_02032990");

void RemoveFieldActor(u8 actor_id) asm("func_080A9F40");

void RemoveFieldActor(u8 actor_id) {
    struct FieldActor *actor = &gFieldActors[FindFieldActorSlot(actor_id)];
    DestroySprite((s32)actor->sprite);
    if (actor->secondary_sprite != 0) {
        DestroySprite((s32)actor->secondary_sprite);
    }
    actor->flags = actor->flags & ~FIELD_ACTOR_ACTIVE;
    if (actor == gControlledFieldActor) {
        gControlledFieldActor = 0;
    }
}
