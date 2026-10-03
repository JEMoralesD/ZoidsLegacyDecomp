#include "m2c_prelude.h"
#include "../game_state.h"

extern volatile u16 D_0300004C;
extern s32 gGameMode;
extern u8 D_0200A880;
extern u8 D_0200A882;
extern u8 D_02032272;
extern u8 D_02030664;
extern u8 D_020218E4[];

void func_08096FBC(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void RunMenuScript(s32) asm("func_08098BB4");
void BiosLz77ToVram(s32, s32) asm("func_080ECD34");
void func_0809AB44(s32, s32, s32, s32, s32);
void ClearSpritePools(void) asm("func_08094330");
void func_08096308(s32, s32);
s32 CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
u8 *GetWindow(s32) asm("func_0809716C");
void func_0809844C(s32, s32, s32, s32, s32, s32, s32);
void func_080B61C8(s32, s32, s32);
void func_080B65E4(s32);
void func_080B7DC0(void);
void func_080B8280(void);
void func_080B89C8(void);
void func_080B9174(void);
void func_080B9ED0(void);
void DestroySprite(s32) asm("func_08094554");
u8 func_080BA540(void);
u8 func_0809669C(void);
void func_080ED17C(s32);

void RunZoidsLab(void)
{
    s32 state;
    u8 choice;

    D_0300004C = 0x1840;
    func_08096FBC(3, 1, 0, 0x3C0, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    RunMenuScript(0x08004EB9);
    BiosLz77ToVram(0x081046A8, 0x06015840);
    func_0809AB44(2, 0, 0x1EF, 4, 4);
    ClearSpritePools();
    func_08096308(0xF, 0);

    choice = 0;
    state = 0;
    if (gGameMode == GAME_MODE_ZOIDS_LAB) {
        do {
            switch (state) {
            case 0:
                RunMenuScript(0x08004EC1);
                state = 5;
                break;

            case 5:
                *(s32 *)0x02032B94 = CreateSprite(
                    0x08105724, 0x08105758, 0, 0xA0, 0x6C,
                    0x2EA, 0xF, 0x20, 0);
                state = 0x10;
                break;

            case 0x10: {
                u8 *record;
                u8 response;
                s32 offset;
                u8 *base;
                s32 value;
                s32 zero;
                s32 one;

                record = GetWindow(2);
                zero = 0;
                record[0x16] = choice;
                base = D_020218E4;
                offset = 0x6A04;
                value = *(s32 *)(base + offset);
                one = 1;
                func_0809844C(value, 7, zero, 2, one, one, zero);
                RunMenuScript(0x08004F94);
                RunMenuScript(0x08004F89);
                if (D_0200A882 == 1) {
                    response = D_0200A880;
                    choice = response;
                    switch (choice) {
                    case 0:
                        func_080B61C8(8, 0, 0);
                        if (D_02032272 == 0) {
                            RunMenuScript(0x08004FAE);
                        } else {
                            state = 0x1000;
                        }
                        break;

                    case 1:
                        func_080B65E4(1);
                        if (*(u8 *)0x02032411 == 0) {
                            RunMenuScript(0x08004FE4);
                        } else {
                            state = 0x2000;
                        }
                        break;

                    case 2:
                        func_080B65E4(2);
                        if (*(u8 *)0x02032411 == 0) {
                            RunMenuScript(0x08005010);
                        } else {
                            state = 0x3000;
                        }
                        break;

                    case 3:
                        state = 0x4000;
                        break;

                    case 4:
                        state = 0x5000;
                        break;
                    }
                } else {
                    if (func_080BA540()) {
                        RunMenuScript(0x0800503C);
                    }
                    gGameMode = GAME_MODE_FIELD;
                    func_08096308(0x10, 0);
                }
                break;
            }

            case 0x1000:
                func_080B7DC0();
                state = 0x10;
                break;

            case 0x2000:
                DestroySprite(*(s32 *)0x02032B94);
                RunMenuScript(0x08004F8D);
                func_080B8280();
                state = 0;
                break;

            case 0x3000:
                DestroySprite(*(s32 *)0x02032B94);
                RunMenuScript(0x08004F8D);
                func_080B89C8();
                state = 0;
                break;

            case 0x4000:
                func_080B9174();
                state = 0x10;
                break;

            case 0x5000:
                DestroySprite(*(s32 *)0x02032B94);
                func_080B9ED0();
                state = 5;
                break;
            }
        } while (gGameMode == GAME_MODE_ZOIDS_LAB);
    }

    while (!func_0809669C()) {
        func_080ED17C(1);
    }
    if (D_02030664 == 2) {
        D_02030664 = 1;
        func_080ED17C(1);
    }
}
