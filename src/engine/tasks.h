#ifndef TASKS_H
#define TASKS_H

enum TaskLimits {
    TASK_CONTEXT_COUNT = 12,
    TASK_RUNNABLE_COUNT = 10,
    TASK_CONTEXT_BYTES = 0x4C,
    TASK_STACK_BYTES = 0x380,
    TASK_IDLE_SLOT = 0
};

struct TaskContext {
    u8 one_based_index;
    u8 remaining_updates;
    u8 data02[2];
    u32 saved_sp;
    u32 resume_pc;
    u32 saved_spsr;
    u32 saved_register_words[15];
};

/* Context 11 ends the scheduler scan. Its entry wrapper is never dispatched. */
/* The ARM restore exchanges the saved R1/R2 words. Keep their original order. */
#define TASK_CONTEXT_FIELD(context, type, field) \
    M2C_FIELD(context, type *, (s32)&((struct TaskContext *)0)->field)

#endif
