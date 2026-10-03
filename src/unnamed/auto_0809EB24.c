#include "m2c_prelude.h"
M2C_UNK ResetScanlineEvents() asm("func_809258C");                                /* extern */

void sub_0809EB24(void) {
    ResetScanlineEvents();
    *(s8 *)0x020324B8 = 3;
}
