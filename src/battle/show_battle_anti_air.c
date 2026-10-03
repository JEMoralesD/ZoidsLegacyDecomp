#include "m2c_prelude.h"
extern void func_80C8F48();
extern u8 D_081081C8;

void ShowBattleAntiAir(int display_state) {
    func_80C8F48(&D_081081C8, display_state, 0);
    *(u32 *)(display_state + 0x8c) = 0;
}
