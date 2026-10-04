#include "../../field/field_actor.h"
#include "../event_script.h"

extern struct FieldActorCommand gFieldActorCommandQueues[][FIELD_ACTOR_COMMAND_COUNT] asm("D_02030668");
extern u8 gEventMapId;

u8 FindFieldActorSlot(u8) asm("func_080A9EF0");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");

s32 EventQueueFieldActorCommand(s32 script_slot, struct EventFieldActorCommand **cursor) asm("func_080A1EB0");

s32 EventQueueFieldActorCommand(s32 script_slot, struct EventFieldActorCommand **cursor) {
    register struct EventFieldActorCommand * volatile *command_slot asm("r5");
    register s32 saved_script_slot asm("r8");
    register s32 actor_slot asm("r1");
    register s32 found_actor_slot asm("r0");

    command_slot = (struct EventFieldActorCommand * volatile *)cursor;
    script_slot <<= 24;
    saved_script_slot = (u32)script_slot >> 24;
    found_actor_slot = FindFieldActorSlot((*command_slot)->actor_id);
    found_actor_slot <<= 24;
    actor_slot = (u32)found_actor_slot >> 24;
    if (actor_slot != FIELD_ACTOR_SLOT_NOT_FOUND) {
        register u32 command_index asm("r3");
        s32 queue_base_address;
        s32 actor_queue_offset;
        register struct FieldActorCommand *actor_queue asm("r4");
        register s32 queue_base_source asm("r0");
        register s32 actor_offset_source asm("r1");
        register u32 first_opcode asm("r2");

        command_index = 0;
        queue_base_source = (s32)gFieldActorCommandQueues;
        actor_offset_source = actor_slot << 8;
        actor_queue = (struct FieldActorCommand *)(actor_offset_source + queue_base_source);
        first_opcode = actor_queue->opcode;
        queue_base_address = queue_base_source;
        actor_queue_offset = actor_offset_source;
        if (first_opcode != 0) {
            register struct FieldActorCommand *queue_scan asm("r1");

            queue_scan = actor_queue;
            do {
                queue_scan += 1;
                command_index += 1;
                if (command_index > FIELD_ACTOR_COMMAND_COUNT - 1) {
                    break;
                }
            } while (queue_scan->opcode != 0);
        }
        if (command_index != FIELD_ACTOR_COMMAND_COUNT) {
            register s32 command_offset asm("r2");
            register s32 queue_command_offset asm("r3");
            register struct FieldActorCommand *queued_command asm("r1");
            struct EventFieldActorCommand *command;

            command_offset = command_index * FIELD_ACTOR_COMMAND_BYTES;
            queue_command_offset = command_offset + actor_queue_offset;
            queued_command = (struct FieldActorCommand *)(queue_command_offset + queue_base_address);
            queued_command->opcode = (*command_slot)->opcode;
            command = *command_slot;
            if ((u8)(command->opcode - FIELD_ACTOR_CMD_MOVE_TO_HALF_SPEED) <= 2 && gEventMapId == FIELD_MAP_PACKED_CELLS) {
                register s32 address asm("r1");

                address = queue_base_address + 2;
                address = queue_command_offset + address;
                *(u16 *)address = command->argument0 * 2;
                address = queue_base_address + 4;
                address = queue_command_offset + address;
                *(u16 *)address = (*command_slot)->argument1 * 2;
            } else {
                register s32 queue_offset asm("r2");
                register s32 address asm("r0");
                register s32 argument0 asm("r1");

                queue_offset = command_offset;
                queue_offset += actor_queue_offset;
                address = queue_base_address + 2;
                address = queue_offset + address;
                argument0 = (*command_slot)->argument0;
                *(u16 *)address = argument0;
                address = queue_base_address + 4;
                queue_offset += address;
                *(u16 *)queue_offset = (*command_slot)->argument1;
            }
        }
    }
    {
        register s32 next_command_selector asm("r1");

        next_command_selector = EVENT_SCAN_NEXT;
        SeekEventCommand(saved_script_slot, next_command_selector, 0);
    }
    return EVENT_CONTINUE;
}
