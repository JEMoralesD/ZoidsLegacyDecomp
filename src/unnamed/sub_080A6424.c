#include "m2c_prelude.h"
void SetSpriteAnimation(s32 *, s32) asm("func_08094564");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
void func_080ED17C(s32);

s32 sub_080A6424(u8 arg0) {
    s32 *temp_r0;
    s32 *temp_r0_2;
    u8 var_r4;

    *(u8 *)0x02030664 = 1;
    temp_r0 = *(s32 **)0x020314A0;
    *temp_r0 = (*temp_r0 & ~0x38) | 0x20;
    SetSpriteAnimation(temp_r0, 2);
    var_r4 = 0;
    do {
        var_r4 += 1;
        func_080ED17C(1);
    } while ((u32) var_r4 <= 0x3B);
    temp_r0_2 = *(s32 **)0x020314A0;
    *temp_r0_2 = (*temp_r0_2 & ~0x30) | 8;
    SetSpriteAnimation(temp_r0_2, 1);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
