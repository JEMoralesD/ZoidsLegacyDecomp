#include "m2c_prelude.h"
#include "tasks.h"

/* The ARM scheduler requires this exact target and instruction alignment. */
void RunScheduledTasks(void) asm("func_080ED174");

__attribute__((naked)) void RunScheduledTasks(void) {
    asm("bx pc\n"
        "nop\n"
        ".arm\n"
        "b 0x0800024C\n"
        ".thumb");
}
