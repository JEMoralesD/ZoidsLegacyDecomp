#include "m2c_prelude.h"
M2C_UNK BiosCpuFastSet(u16 *, u16 *, u32) asm("func_80ECD28");               /* extern */
M2C_UNK BiosLz77ToWram(void *, u16 *) asm("func_80ECD38");                   /* extern */

void sub_0809A1BC(void *arg0, u16 *arg1, u16 *arg2) {
    BiosLz77ToWram(arg0, arg2);
    if (arg1 == (u16 *)0x05000000) {
        *arg2 = *arg1;
    }
    BiosCpuFastSet(arg2, arg1, (u32) ((M2C_FIELD(arg0, u8 *, 1) | (M2C_FIELD(arg0, u8 *, 2) << 8) | (M2C_FIELD(arg0, u8 *, 3) << 0x10)) << 9) >> 0xB);
}
