#include "m2c_prelude.h"
M2C_UNK BiosCpuSet() asm("func_80ECD2C");
void sub_08099FEC(void) {
    s32 sp0 = 0;
    BiosCpuSet(&sp0, 0x020217F4, 0x0500001E);
}
