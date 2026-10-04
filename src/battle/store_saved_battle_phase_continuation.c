#include "m2c_prelude.h"
__attribute__((naked)) void StoreSavedBattlePhaseContinuation(void) asm("func_080BDDE0");

__attribute__((naked)) void StoreSavedBattlePhaseContinuation(void) {
    asm("ldr r0, =0x02030558");
    asm("str r5, [r0]");
    asm("b func_80BE3D6");
}
