#include "field_actor.h"

extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern s32 UsesUnshiftedFieldActorCollisionOrigin(struct FieldActor *) asm("func_080AC098");

void ResolveFieldActorCollisions(struct FieldActor *actor) asm("func_080AB06C");

void ResolveFieldActorCollisions(struct FieldActor *actor) {
    u8 slot_index;
    struct FieldActor *other;
    s32 actor_y_fixed8;
    s32 actor_collision_y_fixed8;
    s32 other_collision_y_fixed8;
    s32 dx;
    register s32 dy asm("r0");
    s32 abs_dx;
    register s32 abs_dy asm("r4");

    if ((u16)(actor->behavior - FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW) <= 1) {
        return;
    }
    if (actor->behavior == FIELD_ACTOR_NO_REGULAR_UPDATES) {
        return;
    }
    if (actor->behavior == FIELD_ACTOR_FOLLOWER) {
        return;
    }
    if (actor->flags & FIELD_ACTOR_COLLISION_DISABLED) {
        return;
    }

    slot_index = 0;
    do {
        other = &gFieldActors[slot_index];
        if ((other->flags & FIELD_ACTOR_ACTIVE) && other != actor && other->behavior != FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW &&
            other->behavior != FIELD_ACTOR_SCRIPTED && other->behavior != FIELD_ACTOR_NO_REGULAR_UPDATES && other->behavior != FIELD_ACTOR_FOLLOWER &&
            !(other->flags & FIELD_ACTOR_COLLISION_DISABLED)) {
            actor_y_fixed8 = actor->world_y_fixed8;
            if ((UsesUnshiftedFieldActorCollisionOrigin(actor) << 24) == 0) {
                actor_collision_y_fixed8 = actor_y_fixed8 + 0x800;
            } else {
                actor_collision_y_fixed8 = actor_y_fixed8;
            }
            other_collision_y_fixed8 = other->world_y_fixed8;
            if ((UsesUnshiftedFieldActorCollisionOrigin(other) << 24) == 0) {
                other_collision_y_fixed8 += 0x800;
            }

            {
                register s32 near_dy asm("r0");
                s32 near_dx;

                near_dx = (actor->world_x_fixed8 + actor->velocity_x_fixed8) - other->world_x_fixed8;
                near_dy = (actor_collision_y_fixed8 + actor->velocity_y_fixed8) - other_collision_y_fixed8;
                if (near_dx < 0) {
                    near_dx = -near_dx;
                }
                if (near_dy < 0) {
                    near_dy = -near_dy;
                }
                asm volatile("" :: "r"(near_dx));
                if (near_dx <= 0xFFF && near_dy <= 0xFFF) {
                    abs_dx = actor->world_x_fixed8 - other->world_x_fixed8;
                    dx = abs_dx;
                    abs_dy = actor_collision_y_fixed8 - other_collision_y_fixed8;
                    dy = abs_dy;
                    if (abs_dx < 0) {
                        abs_dx = -abs_dx;
                    }
                    if (abs_dy < 0) {
                        abs_dy = -abs_dy;
                    }
                    if (abs_dx >= abs_dy) {
                        if (dx >= 0) {
                            actor->velocity_x_fixed8 = 0x1000 - abs_dx;
                        } else {
                            actor->velocity_x_fixed8 = abs_dx - 0x1000;
                        }
                    } else if (dy >= 0) {
                        actor->velocity_y_fixed8 = 0x1000 - abs_dy;
                    } else {
                        dy = abs_dy - 0x1000;
                        actor->velocity_y_fixed8 = dy;
                    }
                }
            }
        }
        slot_index++;
    } while (slot_index <= FIELD_ACTOR_MAX_SLOT);
}
