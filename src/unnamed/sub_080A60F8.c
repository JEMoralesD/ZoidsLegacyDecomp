#include "m2c_prelude.h"
extern void SeekEventCommand(int, int, int) asm("func_080A016C");
extern u8 D_0202ECF4[];

u8 sub_080A60F8(u8 arg0) {
    D_0202ECF4[0x21] &= 0xFD;
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
