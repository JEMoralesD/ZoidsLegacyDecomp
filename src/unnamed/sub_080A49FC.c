#include "m2c_prelude.h"
M2C_UNK func_809FB78(u8);
M2C_UNK func_809F850(void);
M2C_UNK SeekEventCommand(u8, s32, s32) asm("func_80A016C");

int sub_080A49FC(u8 arg0) {
    u32 i;
    for (i = 0; i < 16; i++) {
        func_809FB78(i);
    }
    func_809F850();
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
