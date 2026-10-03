#include "m2c_prelude.h"
u8 TransferSaveBlock(s32, s32, s32) asm("func_8093EC8");                        /* extern */

u8 WriteSaveBlock4(void) {
    return TransferSaveBlock(0, 4, 0);
}
