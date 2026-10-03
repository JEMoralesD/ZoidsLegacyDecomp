#include "m2c_prelude.h"
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");
M2C_UNK func_080E2DCC(s32, s32);
M2C_UNK func_080E2EA4();
M2C_UNK func_080E2F30();
M2C_UNK func_080ED17C(s32);
extern s32 D_087EF200[];

void sub_080C0A9C(u8 arg0) {
    RunMenuScript(0x080234E0);
    func_080E2DCC(6, D_087EF200[arg0]);
    if (!(3 & *(u16 *)0x0300000E)) {
        do {
            func_080E2EA4();
            func_080ED17C(1);
        } while (!(3 & *(u16 *)0x0300000E));
    }
    RunMenuScript(0x080234E8);
    func_080E2F30();
}
