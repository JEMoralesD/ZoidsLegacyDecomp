#include "m2c_prelude.h"
#include "screen_effects.h"
void DisableDisplayWindows(void) asm("func_0809534C");

void DisableDisplayWindows(void) {
    *(s8 *)SCANLINE_WINDOW_ADDRESS(flags) = 0;
    *(u16 *)0x0300004C &= 0x9FFF;
}
