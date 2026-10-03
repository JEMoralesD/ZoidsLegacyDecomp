#include "m2c_prelude.h"
M2C_UNK BiosCpuSet(M2C_UNK, M2C_UNK, M2C_UNK) asm("func_80ECD2C");       /* extern */

void sub_080962D8(void) {
    *(s8 *)0x03005F70 = 0;
    BiosCpuSet(0x08000700, 0x03005F7C, 0x04000029);
    *(s8 *)0x03005F7B = 0;
}
