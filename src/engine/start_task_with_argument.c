#include "m2c_prelude.h"
#include "tasks.h"

extern u8 gTaskContexts[] asm("D_03000A84");
extern s32 gTaskFunctions[] asm("D_03000E14");
extern s32 gTaskArguments[] asm("D_03000E3C");

void StartTaskWithArgument(s32 task_slot, s32 function, s32 argument) asm("func_08092D9C");

void StartTaskWithArgument(s32 task_slot, s32 function, s32 argument) {
    register s32 slot asm("r4");
    register u8 *contexts asm("r5");
    register s32 normalized_slot asm("r3");
    s32 saved_function;
    s32 saved_argument;
    s32 offset;

    saved_function = function;
    saved_argument = argument;
    normalized_slot = (u8)task_slot;
    slot = normalized_slot;
    contexts = gTaskContexts;

    if (normalized_slot != 0) {
        register s32 *field4 asm("r0");
        register s32 *destination asm("r2");
        register s32 table_offset asm("r0");
        register s32 *table asm("r1");

        offset = normalized_slot * TASK_CONTEXT_BYTES;
        field4 = &TASK_CONTEXT_FIELD(contexts, s32, saved_sp);
        destination = (s32 *)(offset + (s32)field4);
        asm volatile("" : "+r"(destination));
        *destination = normalized_slot * TASK_STACK_BYTES + 0x03000E68;
        table = gTaskFunctions;
        table_offset = (normalized_slot - 1) * 4;
        asm volatile("" : "+r"(table), "+r"(table_offset));
        *(s32 *)(table_offset + (s32)table) = saved_function;
        table = gTaskArguments;
        asm volatile("" : "+r"(table), "+r"(table_offset));
        *(s32 *)(table_offset + (s32)table) = saved_argument;
    }

    {
        register s32 *destination asm("r1");
        register s32 *entry_points asm("r3");
        register u8 *context asm("r2");
        s32 entry_address;

        offset = slot * TASK_CONTEXT_BYTES;
        destination = &TASK_CONTEXT_FIELD(contexts, s32, resume_pc);
        destination = (s32 *)(offset + (s32)destination);
        asm volatile("" : "+r"(destination), "+r"(offset), "+r"(contexts));
        entry_points = (s32 *)0x087A0A14;
        asm volatile("" : "+r"(entry_points));
        entry_address = *(s32 *)(slot * 4 + (s32)entry_points);
        asm volatile("" : : "r"(slot));
        *destination = entry_address;
        context = (u8 *)(offset + (s32)contexts);
        asm volatile("" : "+r"(context));
        TASK_CONTEXT_FIELD(context, u8, remaining_updates) = 1;
    }
}
