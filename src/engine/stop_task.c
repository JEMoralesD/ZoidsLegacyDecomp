#include "m2c_prelude.h"
#include "tasks.h"
extern s32 gTaskContexts[] asm("D_03000A84");
void StopTask(u8 task_slot) asm("func_08092E0C");

void StopTask(u8 task_slot) {
    s32 contexts = (s32)gTaskContexts;
    s32 context_offset;
    s32 *saved_sp_base, *resume_pc_base;
    context_offset = TASK_CONTEXT_BYTES;
    context_offset *= task_slot;
    saved_sp_base = &TASK_CONTEXT_FIELD(contexts, s32, saved_sp);
    *(s32 *)(context_offset + (s32)saved_sp_base) = -1;
    resume_pc_base = &TASK_CONTEXT_FIELD(contexts, s32, resume_pc);
    *(s32 *)(context_offset + (s32)resume_pc_base) = -1;
}
