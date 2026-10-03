#include "m2c_prelude.h"
M2C_UNK UpdateKeyRepeat() asm("func_8096F50");                                /* extern */

void ReadKeys(void) {
    u16 held_keys;

    held_keys = ~*(u16 *)0x04000130;
    *(s16 *)0x0300000E = held_keys & ~*(u16 *)0x0300000C;
    *(u16 *)0x0300000C = held_keys;
    UpdateKeyRepeat();
}
