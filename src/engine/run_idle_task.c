#include "m2c_prelude.h"
#include "tasks.h"
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

void RunIdleTask(void) asm("func_08092D80");

void RunIdleTask(void) {
loop_1:
    YieldTaskForUpdates(1);
    goto loop_1;
}
