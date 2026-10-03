#include "m2c_prelude.h"
extern int SeekEventCommand(int, int, int) asm("func_80A016C");
extern u8 D_0202ECF4[];

int sub_080A60D0(u8 arg) {
    D_0202ECF4[0x21] |= 1;
    SeekEventCommand(arg, -1, 0);
    return 0;
}
