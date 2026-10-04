#include "field_actor.h"

extern struct FieldActorSpriteDefinition gFieldActorSpriteDefinitions[] asm("D_087AD208");
extern u8 gFieldActorFixedFacingFrames[] asm("D_0827A1BC");
extern u8 gFieldActorAlternateFixedFacingFrames[] asm("D_0827A1E4");
extern u8 gFieldActorTwoViewFrames[] asm("D_0827A224");
extern u8 gFieldActorEightViewFrames[] asm("D_08279A64");
extern u8 gFieldActorSharedEightViewFrames[] asm("D_0827A0C4");
extern u8 gFieldActorIdleAndMovingFrames[] asm("D_0827A28C");
extern u8 gFieldActorTwoWayAnimationIndices[] asm("D_087A1BE8");
extern u8 gFieldActorFourWayAnimationIndices[] asm("D_087A1BF0");
extern u8 gFieldActorEightWayAnimationIndices[] asm("D_087A1BF8");
extern u8 gFieldActorSingleDirectionAnimationIndices[] asm("D_087A1C00");

s32 GetFieldActorAnimationIndex(struct FieldActor *actor) asm("func_080A9A54");

s32 GetFieldActorAnimationIndex(struct FieldActor *actor)
{
    register struct FieldActorSpriteDefinition *definitions asm("r0") =
        gFieldActorSpriteDefinitions;
    void *frame_table;
    u32 result;
    u8 *table;

    asm volatile("" : "+r"(definitions));
    frame_table = definitions[FIELD_ACTOR_FIELD(actor, u8 *, model_id)].frame_table;
    if (frame_table == (void *)gFieldActorFixedFacingFrames) {
        goto zero;
    }
    if (frame_table != (void *)gFieldActorAlternateFixedFacingFrames) {
        goto check_simple;
    }
zero:
    result = 0;
    goto done;

check_simple:
    if (frame_table != (void *)gFieldActorTwoViewFrames) {
        goto check_pair;
    }
    table = gFieldActorTwoWayAnimationIndices;
    asm volatile("" : "+r"(table));
simple_lookup:
    {
        register u32 address asm("r0") = FIELD_ACTOR_FIELD(actor, u8 *, current_direction);
        asm volatile("" : "+r"(address));
        address += (u32)table;
        result = *(u8 *)address;
    }
    goto done;

check_pair:
    if (frame_table == (void *)gFieldActorEightViewFrames) {
        goto pair_lookup;
    }
    if (frame_table != (void *)gFieldActorSharedEightViewFrames) {
        goto check_single;
    }
pair_lookup:
    table = gFieldActorEightWayAnimationIndices;
    asm volatile("" : "+r"(table));
    goto lookup_plus_four;

check_single:
    if (frame_table != (void *)gFieldActorIdleAndMovingFrames) {
        goto default_lookup;
    }
    table = gFieldActorSingleDirectionAnimationIndices;
    asm volatile("" : "+r"(table));
    {
        register u32 address asm("r0") = FIELD_ACTOR_FIELD(actor, u8 *, current_direction);
        asm volatile("" : "+r"(address));
        address += (u32)table;
        result = *(u8 *)address;
    }
    if ((u8)(FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) - 1) <= 2) {
        result += 1;
    }
    goto done;

default_lookup:
    table = gFieldActorFourWayAnimationIndices;
    asm volatile("" : "+r"(table));
lookup_plus_four:
    {
        register u32 address asm("r0") = FIELD_ACTOR_FIELD(actor, u8 *, current_direction);
        asm volatile("" : "+r"(address));
        address += (u32)table;
        result = *(u8 *)address;
    }
    if ((u8)(FIELD_ACTOR_FIELD(actor, u8 *, movement_mode) - 1) <= 2) {
        result += 4;
    }

done:
    return result;
}
