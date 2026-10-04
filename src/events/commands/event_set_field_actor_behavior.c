#include "../../field/field_actor.h"
#include "../event_script.h"
#include "../../game/player_state.h"

void SeekEventCommand(s32, s32, s32) asm("func_080A016C");
void *CreateFieldActor(u8, u8, s32, s32, s32, s32, s32, s32) asm("func_080A9D78");
u8 FindFieldActorSlot(u8) asm("func_080A9EF0");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventSetFieldActorBehavior(s32 script_slot, struct EventFieldActorBehaviorCommand **script_cursor) asm("func_080A0788");

s32 EventSetFieldActorBehavior(s32 script_slot, struct EventFieldActorBehaviorCommand **script_cursor)
{
    register struct EventFieldActorBehaviorCommand **saved_script_cursor asm("r5") = script_cursor;
    register u32 saved_script_slot;
    register u32 follower_replacement_needed asm("r6");
    register u32 actor_slot asm("r2");
    register struct FieldActor *actor asm("r4");

    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    follower_replacement_needed = 0;
    {
        register u32 result asm("r0");

        result = FindFieldActorSlot((*saved_script_cursor)->actor_id);
        result <<= 24;
        actor_slot = result >> 24;
    }
    if (actor_slot != 0xFF) {
        register u32 offset asm("r0");
        register struct FieldActor *base asm("r1") =
            (struct FieldActor *)0x020325A0;
        register u32 previous_behavior asm("r3");
        register struct EventFieldActorBehaviorCommand *command asm("r0");

        offset = actor_slot << 3;
        offset += actor_slot;
        offset <<= 3;
        actor = (struct FieldActor *)(offset + (u32)base);
        command = *saved_script_cursor;
        previous_behavior = actor->behavior;
        if (previous_behavior != command->behavior) {
            actor->flags &= ~FIELD_ACTOR_INPUT_LOCKED;
            if (previous_behavior == FIELD_ACTOR_PLAYER_CONTROLLED) {
                *(u32 *)0x02032990 = follower_replacement_needed;
                if (actor->model_id == 1) {
                    register u32 test asm("r1");
                    register u32 nonzero asm("r0");

                    nonzero = FindFieldActorSlot(0xD);
                    nonzero <<= 24;
                    actor_slot = nonzero >> 24;
                    test = 0xFF;
                    test ^= actor_slot;
                    nonzero = 0 - test;
                    nonzero |= test;
                    follower_replacement_needed = nonzero >> 31;
                }
            }

            actor->behavior = (*saved_script_cursor)->behavior;
            actor->velocity_y_fixed8 = 0;
            actor->velocity_x_fixed8 = 0;
            if (actor->movement_mode != FIELD_ACTOR_SPECIAL_SPRITE_ANIMATION) {
                actor->movement_mode = FIELD_ACTOR_IDLE;
            }

            if (follower_replacement_needed != 0) {
                register struct FieldActor *follower_actor asm("r2");
                register u32 offset asm("r0");
                register struct FieldActor *base asm("r1") =
                    (struct FieldActor *)0x020325A0;

                asm volatile("" : "+r"(base));
                offset = actor_slot << 3;
                offset += actor_slot;
                offset <<= 3;
                follower_actor = (struct FieldActor *)(offset + (u32)base);
                follower_actor->behavior = FIELD_ACTOR_FOLLOWER_CATCH_UP;
                *(u8 *)0x02030664 = 1;
                if (follower_actor->flags & 1) {
                    register struct FieldActor *follower_to_wait_for asm("r5") =
                        follower_actor;

                    do {
                        YieldTaskForUpdates(1);
                    } while (follower_to_wait_for->flags & 1);
                }
            }

            if (actor->behavior == FIELD_ACTOR_PLAYER_CONTROLLED) {
                *(struct FieldActor **)0x02032990 = actor;
                if (actor->model_id == 1) {
                    register u32 pilot_slot asm("r2") = 1;
                    register u8 *base asm("r3") = (u8 *)0x020218E4;
                    register s32 zero asm("r5") = 0;

scan_next:
                    {
                        register u32 offset asm("r0") = pilot_slot << 6;
                        register u8 *entry asm("r1") =
                            (u8 *)((u32)offset + (u32)base);
                        register u32 field_offset asm("r6") = 0x5A94;
                        register u8 *field asm("r0");

                        field = entry + field_offset;
                        if (*field == 1) {
                            field_offset += 0x31;
                            field = entry + field_offset;
                            if (*field == 1) {
                                register u32 result asm("r0");

                                result = FindFieldActorSlot(0xD);
                                result <<= 24;
                                pilot_slot = result >> 24;
                                if (pilot_slot == 0xFF) {
                                    *(void **)0x02032994 = CreateFieldActor(
                                        0x4B, 0xD, actor->world_x_fixed8,
                                        actor->world_y_fixed8, actor->requested_direction,
                                        zero, 8, zero);
                                }
                                goto finished;
                            }
                        }
                    }
                    pilot_slot += 1;
                    if (pilot_slot <= 0x34) {
                        goto scan_next;
                    }
                }
            }
        }
    }

finished:
    {
        register u32 final_script_slot asm("r0");
        register s32 minus_one asm("r1");

        minus_one = 1;
        minus_one = -minus_one;
        asm volatile("" : "+r"(minus_one));
        final_script_slot = saved_script_slot;
        SeekEventCommand(final_script_slot, minus_one, 0);
    }
    return 0;
}
