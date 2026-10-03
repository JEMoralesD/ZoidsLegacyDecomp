#include "m2c_prelude.h"
extern u32 D_02031744;
extern void DestroySprite(u32) asm("func_8094554");
extern int SeekEventCommand(u8, int, int) asm("func_80A016C");

int sub_080A16B4(u8 arg) {
    if (D_02031744 != 0) {
        DestroySprite(D_02031744);
        D_02031744 = 0;
    }
    SeekEventCommand(arg, -1, 0);
    return 0;
}
