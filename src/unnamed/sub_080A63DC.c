#include "m2c_prelude.h"
extern u8 D_0202ECF4[];
extern u8 gEventMapId;
extern void func_80A6148(void);
extern int SeekEventCommand(int, int, int) asm("func_80A016C");

s32 sub_080A63DC(u8 arg0, u8 **arg1) {
    D_0202ECF4[30] = (*arg1)[1] + 1;
    D_0202ECF4[31] = (*arg1)[2];
    D_0202ECF4[32] = (*arg1)[3];
    if (gEventMapId == 0)
        func_80A6148();
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
