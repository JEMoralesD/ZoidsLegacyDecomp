#include "m2c_prelude.h"
extern u8 D_0202ECF4[];
extern void *D_020314A0;
extern void DestroySprite(void *) asm("func_8094554");
extern void SeekEventCommand(int, int, int) asm("func_080A016C");

u8 sub_080A6490(u8 arg0) {
    D_0202ECF4[30] = 0;
    if (D_020314A0 != 0) {
        DestroySprite(D_020314A0);
        D_020314A0 = 0;
    }
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
