#include "m2c_prelude.h"

/*
 * The CPU runs this reset vector before the C run-time environment exists.
 * The exact ARM branch must stay in assembly.
 */
void RomResetVector(void) asm("func_08000000");

__attribute__((naked)) void RomResetVector(void) {
    asm(".arm\n"
        "b 0x080000C0\n"
        ".thumb");
}
