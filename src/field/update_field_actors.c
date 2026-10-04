#include "field_actor.h"

extern struct FieldActor gFieldActors[] asm("D_020325A0");
extern void StartFieldActorTargetMovement(struct FieldActor *) asm("func_080AAE80");
extern void UpdatePlayerFieldActorControl(struct FieldActor *) asm("func_080AA1E4");
extern void UpdateFieldActorWandering(struct FieldActor *) asm("func_080AA4E0");
extern void UpdateFieldActorFacingAndVelocity(struct FieldActor *) asm("func_080AA0B8");
extern void UpdateFieldActorCommands(u8, struct FieldActor *) asm("func_080AA5C0");
extern void UpdateFieldFollowerPositionHistory(struct FieldActor *) asm("func_080AABCC");
extern s32 UpdateFieldFollowerCatchUp(struct FieldActor *) asm("func_080AAD34");
extern void ResolveFieldMapCollisions(struct FieldActor *) asm("func_080AB2EC");
extern void ResolveFieldActorCollisions(struct FieldActor *) asm("func_080AB06C");
extern void AdvanceFieldActorPosition(struct FieldActor *) asm("func_080ABE3C");
extern void UpdateFieldActorMapCell(struct FieldActor *) asm("func_080A98C4");
extern void UpdateFieldActorTargetMovement(struct FieldActor *) asm("func_080AAF80");
extern void UpdateFieldCameraFollow(struct FieldActor *) asm("func_080ABE70");
extern void UpdateFieldActorSpritePositions(void) asm("func_080ABFCC");

void UpdateFieldActors(void) asm("func_080A9F8C");

void UpdateFieldActors(void) {
    u8 slot_index;
    u32 flags;
    struct FieldActor *actor;

    slot_index = 0;
    do {
        actor = &gFieldActors[slot_index];
        flags = actor->flags;
        if (flags & FIELD_ACTOR_ACTIVE) {
            if (flags & FIELD_ACTOR_TARGET_MOVEMENT_PENDING) {
                StartFieldActorTargetMovement(actor);
            }
        }
        slot_index++;
    } while (slot_index <= FIELD_ACTOR_MAX_SLOT);

    slot_index = 0;
    do {
        actor = &gFieldActors[slot_index];
        if (actor->flags & FIELD_ACTOR_ACTIVE) {
            if (actor->behavior != FIELD_ACTOR_NO_REGULAR_UPDATES && actor->behavior != FIELD_ACTOR_STATIONARY &&
                actor->behavior != FIELD_ACTOR_DIRECTION_OFFSET && actor->behavior != FIELD_ACTOR_MAP_INTERACTION) {
                if (!(actor->flags & FIELD_ACTOR_TARGET_MOVEMENT_ACTIVE)) {
                    switch (actor->behavior) {
                    case FIELD_ACTOR_PLAYER_CONTROLLED:
                        UpdatePlayerFieldActorControl(actor);
                        UpdateFieldActorFacingAndVelocity(actor);
                        goto update;
                    case FIELD_ACTOR_WANDERING:
                        UpdateFieldActorWandering(actor);
                        UpdateFieldActorFacingAndVelocity(actor);
                        goto update;
                    case FIELD_ACTOR_TURN_ONLY:
                        UpdateFieldActorFacingAndVelocity(actor);
                        goto next_actor;
                    case FIELD_ACTOR_SCRIPTED_CAMERA_FOLLOW:
                    case FIELD_ACTOR_SCRIPTED:
                        UpdateFieldActorCommands(slot_index, actor);
                        goto update;
                    case FIELD_ACTOR_FOLLOWER:
                        UpdateFieldFollowerPositionHistory(actor);
                        goto update;
                    case FIELD_ACTOR_FOLLOWER_CATCH_UP:
                        if ((UpdateFieldFollowerCatchUp(actor) << 24) != 0) {
                            goto next_actor;
                        }
                    default:
update:
                        ResolveFieldMapCollisions(actor);
                        ResolveFieldActorCollisions(actor);
                        AdvanceFieldActorPosition(actor);
                        actor->previous_map_cell_value = actor->map_cell_value;
                        UpdateFieldActorMapCell(actor);
                        if (actor->previous_map_cell_value == actor->map_cell_value) {
                            actor->map_cell_change = 0;
                        } else {
                            actor->map_cell_change = actor->map_cell_value;
                        }
                        break;
                    }
                } else {
                    UpdateFieldActorTargetMovement(actor);
                }
                UpdateFieldCameraFollow(actor);
            }
        }
next_actor:
        slot_index++;
    } while (slot_index <= FIELD_ACTOR_MAX_SLOT);
    UpdateFieldActorSpritePositions();
}
