#include "m2c_prelude.h"
#include "tasks.h"

extern u8 gTaskContexts[] asm("D_03000A84");

void StartTask(s32, s32) asm("func_08092D8C");

void InitializeTasks(void) asm("func_08092D20");

void InitializeTasks(void) {
    s32 index;
    register s32 idle_entry_address asm("r8");
    u8 *contexts;
    register s32 context_stride asm("r12");
    s32 one;
    s32 minus_one;
    s32 *resume_pc_base;
    s32 *saved_sp_base;

    index = 0;
    idle_entry_address = 0x08092D81;
    asm volatile("" : "+r"(idle_entry_address));
    contexts = gTaskContexts;
    context_stride = TASK_CONTEXT_BYTES;
    one = 1;
    minus_one = -1;
    resume_pc_base = &TASK_CONTEXT_FIELD(contexts, s32, resume_pc);
    saved_sp_base = &TASK_CONTEXT_FIELD(contexts, s32, saved_sp);
    do {
        register s32 offset asm("r1");
        u8 *context;

        offset = context_stride * index;
        asm volatile("" : "+r"(offset));
        context = (u8 *)(offset + (s32)contexts);
        index++;
        TASK_CONTEXT_FIELD(context, u8, one_based_index) = (u8)index;
        TASK_CONTEXT_FIELD(context, u8, remaining_updates) = one;
        *(s32 *)(offset + (s32)saved_sp_base) = minus_one;
        *(s32 *)(offset + (s32)resume_pc_base) = minus_one;
        asm volatile(
            "lsl %0, %0, #24\n\t"
            "lsr %0, %0, #24"
            : "+r"(index));
    } while ((u32)index < TASK_CONTEXT_COUNT);
    StartTask(TASK_IDLE_SLOT, idle_entry_address);
    *(s32 *)0x03000E64 = (s32)gTaskContexts;
}
