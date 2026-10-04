#include "m2c_prelude.h"
void ReadNextMusicCommandByteIntoR3WithAddressGuard(void) asm("func_080EAEF4");

__attribute__((naked)) void ReadNextMusicCommandByteIntoR3WithAddressGuard(void) {
    asm(".syntax unified");
    asm("ldr r2, [r1, #0x40]");
    asm("adds r3, r2, #1");
    asm("str r3, [r1, #0x40]");
    asm("ldrb r3, [r2]");
    asm("b func_80EAEDA");
    asm(".syntax divided");
}
