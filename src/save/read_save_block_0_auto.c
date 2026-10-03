#include "m2c_prelude.h"
u8 TransferSaveBlock(s32, s32, s32) asm("func_8093EC8");                        /* extern */

u8 ReadSaveBlock0(void) {
    return TransferSaveBlock(1, 0, 0);
}
