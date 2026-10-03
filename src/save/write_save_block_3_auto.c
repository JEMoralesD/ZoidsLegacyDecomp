#include "m2c_prelude.h"
u8 TransferSaveBlock(s32, s32, s32) asm("func_8093EC8");                        /* extern */

u8 WriteSaveBlock3(void) {
    return TransferSaveBlock(0, 3, 0);
}
