#include "m2c_prelude.h"
extern void SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 D_0202ECF4[];

int sub_080A6120(u8 arg0) {
    D_0202ECF4[0x21] |= 2;
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
