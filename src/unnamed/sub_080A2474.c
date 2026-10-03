#include "m2c_prelude.h"
M2C_UNK PlayOrContinueSong(u16) asm("func_8092E74");
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");
extern u8 D_02030667;

int sub_080A2474(u8 arg0, u8 **arg1) {
    D_02030667 = (*arg1)[1];
    PlayOrContinueSong(D_02030667);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
