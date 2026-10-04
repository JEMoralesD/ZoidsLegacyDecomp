#include "m2c_prelude.h"
u8 IsBattleCameraTransitionComplete(void) asm("func_080BB654");

u8 IsBattleCameraTransitionComplete(void) {
    return *(u8 *)0x02032F62;
}
