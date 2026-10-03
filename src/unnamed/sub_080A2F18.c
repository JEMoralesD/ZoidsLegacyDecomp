#include "m2c_prelude.h"
extern int func_80BB224(int, int, int, int);
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

int sub_080A2F18(u8 arg) {
    func_80BB224(6, 0, 0, 0);
    SeekEventCommand(arg, -1, 0);
    return 0;
}
