#include "m2c_prelude.h"
extern int func_80E65B4(int);
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int sub_080A1344(u8 arg0, u8 **arg1) {
    func_80E65B4((*arg1)[1]);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
