#include "m2c_prelude.h"
extern u8 D_020317D8;
extern void SeekEventCommand(int, int, int) asm("func_080A016C");

u8 sub_080A39FC(u8 arg0, u8 **arg1) {
    D_020317D8 = **arg1;
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
