#include "m2c_prelude.h"
#include "screen_effects.h"
M2C_UNK BiosCpuSet(M2C_UNK, M2C_UNK, M2C_UNK) asm("func_80ECD2C");       /* extern */

void InitializeScreenTransitions(void) asm("func_080962D8");

void InitializeScreenTransitions(void) {
    *(s8 *)SCREEN_TRANSITION_ADDRESS(flags) = 0;
    BiosCpuSet(0x08000700, SCREEN_TRANSITION_ADDRESS(hblank_callback[0]), 0x04000029);
    *(s8 *)SCREEN_TRANSITION_ADDRESS(hblank_action) = 0;
}
