#include "m2c_prelude.h"
void func_8096FBC(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void BiosLz77ToVram(s32, s32) asm("func_80ECD34");
void func_809AB44(s32, s32, s32, s32, s32);
void ClearSpritePools(void) asm("func_8094330");
void RunMenuScript(s32) asm("func_8098BB4");
void func_809D094(void);
u8 *GetWindow(s32) asm("func_809716C");
void func_8096308(s32, s32);

void sub_0809D200(void) {
    *(s16 *)0x0300004C = 0x1140;
    func_8096FBC(0, 1, 0, 0x3C0, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    BiosLz77ToVram(0x081046A8, 0x06015840);
    func_809AB44(2, 3, 0, 0, 1);
    ClearSpritePools();
    RunMenuScript(0x08000B77);
    func_809D094();
    GetWindow(0)[0x16] = *(u8 *)0x020216F4;
    func_8096308(0xF, 0);
}
