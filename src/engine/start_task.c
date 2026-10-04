#include "m2c_prelude.h"
#include "tasks.h"
M2C_UNK StartTaskWithArgument(u8, M2C_UNK, s32) asm("func_08092D9C");                /* extern */

void StartTask(u8 task_slot, M2C_UNK function) asm("func_08092D8C");

void StartTask(u8 task_slot, M2C_UNK function) {
    StartTaskWithArgument(task_slot, function, 0);
}
