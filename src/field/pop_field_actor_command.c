#include "field_actor.h"
extern struct FieldActorCommandQueue gFieldActorCommandQueues[] asm("D_02030668");
void PopFieldActorCommand(s32 actor_slot) asm("func_080AA550");

void PopFieldActorCommand(s32 actor_slot) {
    register s32 queue_offset asm("r4");
    register s32 next_command_offset asm("r5");
    register s32 command_offset asm("r6");
    register s32 command_index asm("r1");
    register s32 next_command_index_shifted24 asm("r0");
    s32 base, next_command_index, actor_slot_shifted24;
    u8 halfword_index;
    s32 halfword_offset;
    actor_slot_shifted24 = actor_slot << 0x18;
    command_index = 0;
    queue_offset = (u32)actor_slot_shifted24 >> 0x10;
    base = (s32)gFieldActorCommandQueues;
loop_1:
    halfword_index = 0;
    next_command_index = command_index + 1;
    command_offset = command_index * 8;
    next_command_offset = next_command_index * 8;
    do {
        halfword_offset = halfword_index * 2;
        *(u16 *)(halfword_offset + command_offset + queue_offset + base) = *(u16 *)(halfword_offset + next_command_offset + queue_offset + base);
        halfword_index = halfword_index + 1;
    } while ((u32) halfword_index <= 3U);
    if (*(u16 *)(next_command_index * 8 + queue_offset + base) != 0) {
        next_command_index_shifted24 = next_command_index << 0x18;
        command_index = (u32)next_command_index_shifted24 >> 0x18;
        if ((u32) command_index <= (u32)FIELD_ACTOR_COMMAND_SHIFT_LAST) {
            goto loop_1;
        }
    }
}
