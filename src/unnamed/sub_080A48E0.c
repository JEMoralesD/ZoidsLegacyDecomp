#include "m2c_prelude.h"
void func_809F8A0(int);
void SeekEventCommand(int, int, int) asm("func_80A016C");

int sub_080A48E0(u8 arg0, u8 **arg1) {
    func_809F8A0((*arg1)[1]);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
