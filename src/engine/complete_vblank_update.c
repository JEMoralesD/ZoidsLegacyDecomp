#include "m2c_prelude.h"
void CompleteVBlankUpdate(void) asm("func_080923E4");

void CompleteVBlankUpdate(void) {
    *(volatile s16 *)0x04000208 = 0;
    *(volatile s16 *)0x03007FF8 = 1;
    *(volatile s16 *)0x04000208 = 1;
    *(u8 *)0x0300067D += 1;
    *(s8 *)0x0300067C = 0;
}
