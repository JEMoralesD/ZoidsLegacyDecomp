#include "m2c_prelude.h"

extern u16 D_0300004C;
extern s32 D_03000054[];

void ClearSpritePools(void) asm("func_08094330");
void BiosLz77ToVram(s32, s32) asm("func_080ECD34");
void CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
void func_0809538C(s32, s32, s32, s32, s32, s32, s32, s32);

void sub_0809B6DC(u8 arg0) {
    D_0300004C = 0x1040;
    *(u16 *)0x04000008 = 0x1E83;
    ClearSpritePools();
    D_03000054[0] = 0;
    if (arg0 == 0) {
        D_03000054[1] = -0x1000;
    } else {
        D_0300004C |= 0x100;
        D_03000054[1] = 0x7000;
    }
    BiosLz77ToVram(0x080F3758, 0x06010000);
    BiosLz77ToVram(0x080F629C, 0x05000200);
    BiosLz77ToVram(0x080F6638, 0x06000000);
    BiosLz77ToVram(0x08102118, 0x05000000);
    BiosLz77ToVram(0x081022D8, 0x0600F000);
    *(u16 *)0x05000000 = 0;
    BiosLz77ToVram(0x08102B3C, 0x06016700);
    BiosLz77ToVram(0x08102C10, 0x05000380);
    BiosLz77ToVram(0x08102C78, 0x06016820);
    BiosLz77ToVram(0x08102E3C, 0x050003A0);
    BiosLz77ToVram(0x08102EE0, 0x06016FA0);
    BiosLz77ToVram(0x0810319C, 0x050003C0);
    BiosLz77ToVram(0x081037A0, 0x06017AA0);
    BiosLz77ToVram(0x08103890, 0x05000360);
    BiosLz77ToVram(0x080ED3A4, 0x06017C20);
    BiosLz77ToVram(0x080ED4D8, 0x050003E0);
    if (arg0 == 0) {
        func_0809538C(1, 0xF0, 0x10, 1, 0xF0, 0x90A0, 0x2020, 0x31);
    } else {
        CreateSprite(0x080F6608, 0x080F662C, 2, 0x78, 0x30, 0, 0, 0x48, 0);
        CreateSprite(0x08102ED0, 0x08102EDC, 0, 0x78, 0x98, 0x341, 0xD, 0x48, 0);
        func_0809538C(1, 0xF0, 0x10, 1, 0xF0, 0x90A0, 0x3030, 0x31);
    }
}
