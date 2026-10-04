#include "m2c_prelude.h"
#include "screen_effects.h"
extern u8 gScanlineWindowFlags asm("D_03005EE8");

s32 AreDisplayWindowsDisabled(void) asm("func_0809536C");

s32 AreDisplayWindowsDisabled(void) {
    if (7 & gScanlineWindowFlags) {
        return 0;
    }
    return 1;
}
