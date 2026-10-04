#include "m2c_prelude.h"
#include "screen_effects.h"
s32 IsScreenTransitionComplete(void) asm("func_0809669C");

s32 IsScreenTransitionComplete(void) {
    s32 var_r1;

    var_r1 = 0;
    if (*(u8 *)SCREEN_TRANSITION_ADDRESS(flags) == 0) {
        var_r1 = 1;
    }
    return var_r1;
}
