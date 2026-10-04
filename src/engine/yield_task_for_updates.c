#include "m2c_prelude.h"
#include "tasks.h"

/* The ARM scheduler requires this exact target and instruction alignment. */
void YieldTaskForUpdates(s32 updates) asm("func_080ED17C");

__attribute__((naked)) void YieldTaskForUpdates(s32 updates) {
    asm("bx pc\n"
        "nop\n"
        ".arm\n"
        "b 0x0800026C\n"
        ".thumb");
}
