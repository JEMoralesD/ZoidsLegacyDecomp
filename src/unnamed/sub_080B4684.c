#include "m2c_prelude.h"

extern u8 D_02032B90[];
extern u8 gBattleState[];
extern u8 gBattleSetup[];
extern u32 gZoidNameTable[];
extern void D_081062D4;
extern void D_081061A4;
extern void D_081062D8;
extern void D_08105BE8;
extern void D_08105C24;
extern void D_0810651C;
extern void D_08106380;
extern void D_08106534;
extern void D_0810654C;

void ClearWindow(s32) asm("func_080986B4");
void DestroySprite(void) asm("func_08094554");
s32 func_080981F0(void *, s32, s32, s32, s32);
void func_08098248(void *, s32, s32);
void func_080984C4(s32, s32, s32, s32, s32);
void func_0809844C(s16, s32, s32, s32, s32, s32, s32);
void *CreateSprite(void *, void *, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
M2C_UNK func_080E7B64(s32);
s32 func_080ECD98(s16, s32);

s32 sub_080B4684(u8 index, u8 selection)
{
    ClearWindow(5);
    ClearWindow(6);
    if (*(void **)D_02032B90 != 0) {
        DestroySprite();
        *(void **)D_02032B90 = 0;
    }

    if (index != 0xFF) {
        u8 *record;
        u8 *item;
        s32 value;
        s32 five;
        s32 other_value;
        s32 compare;
        s32 flag;

        record = &gBattleState[index * 0x270];
        item = &gBattleState[index * 0x270 + 0x70];
        func_080981F0((void *)gZoidNameTable[record[0]], 0, 5, 0, 0);
        func_080981F0(&D_081062D4, 0, 5, 0, 2);

        if (gBattleSetup[0] != 1) {
            value = *(s16 *)(record + 6);
            flag = 0;
            compare = func_080ECD98(*(s16 *)(record + 0x3A), 10);
            {
                s32 compare_value = value;
                asm volatile("" : "+&r"(compare_value) : "r"(value));
                if (compare_value < (s16)compare)
                    flag = 1;
            }
            func_080984C4(value, 4, flag, 10, 5);
        } else {
            s32 else_value;
            register s32 reserve_r2 asm("r2");
            asm volatile("" : "=r"(reserve_r2));
            else_value = *(s16 *)(record + 0x3A);
            asm volatile("" :: "r"(reserve_r2));
            func_080984C4(else_value, 4, 0, 10, 5);
        }

        func_08098248(&D_081061A4, 0, 5);
        {
            register s32 reserve_r1 asm("r1");
            asm volatile("" : "=r"(reserve_r1));
            other_value = *(s16 *)(record + 0x3A);
            asm volatile("" :: "r"(reserve_r1));
        }
        five = 5;
        func_080984C4(other_value, 4, 0, 10, five);
        func_080981F0(&D_081062D8, 0, 5, 0, 3);
        func_0809844C(*(s16 *)(record + 0x3E), 4, 0, 10, five, 8, 3);
        *(void **)D_02032B90 = CreateSprite(&D_08105BE8, &D_08105C24,
            record[0x38], 108, 132, 0x355, 15, 8, 0);
        func_08098248(func_080E7B64(item[0]), 0, 6);
    } else {
        func_080981F0(&D_0810651C, 0, 5, 0, 0);
        switch (selection) {
        case 0:
            func_080981F0(&D_08106380, 2, 5, 0, 2);
            break;
        case 1:
            func_080981F0(&D_08106534, 1, 5, 0, 2);
            break;
        case 2:
            func_080981F0(&D_0810654C, 1, 5, 0, selection);
            break;
        }
        func_08098248(func_080E7B64(0), 0, 6);
    }
}
