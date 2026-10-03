#include "m2c_prelude.h"
extern u8 gBattleState[];
extern u8 D_000027BE[];
extern u8 D_02033F36;
extern u8 D_020317D9;
extern u8 gZoidBaseStatTable[];
struct TableEntry {
    u8 pad[0x1A];
    u16 value;
};

extern void DestroySpriteGroup(s32) asm("func_08095114");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void func_080CD110(void);
extern void StartBattleEquipmentAnimation(u8, u8, s32, u8, s32) asm("func_080D0CA0");
extern s32 GetBattleAnimationResult(void) asm("func_080D1C18");
extern void StartBattleScanlineWindowClosing(void) asm("func_080D1D44");
extern u8 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");
extern void RequestBattleBackgroundShakeStop(void) asm("func_080D2200");
extern s32 IsBattleBackgroundShakeFinished(void) asm("func_080D222C");
extern void StartBattleSpeedLineBackground(void) asm("func_080D18E4");
extern void func_080ED17C(s32);

s32 sub_080A3704(u8 arg0, void **arg1) {
    u8 *temp_r4;
    u8 *p;
    u8 *pidx;
    u8 call0;
    u8 call1;
    u8 call3;
    u16 call2;
    u8 *table;
    u32 offset;

    if (GetBattleScanlineWindowPhase() == 1) {
        StartBattleScanlineWindowClosing();
        while ((u32)GetBattleScanlineWindowPhase() <= 1U) {
            func_080ED17C(1);
        }
    }
    if ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
        RequestBattleBackgroundShakeStop();
        while ((IsBattleBackgroundShakeFinished() << 0x18) == 0) {
            func_080ED17C(1);
        }
    }
    p = gBattleState;
    pidx = &D_02033F36;
    p += (int)D_000027BE;
    if (p[*pidx] != 0) {
        DestroySpriteGroup(*(s32 *)0x020316F8);
        func_080CD110();
    }
    if (D_020317D9 != 0) {
        StartBattleSpeedLineBackground();
        D_020317D9 = 0;
    }
    call0 = *pidx;
    call1 = *(u8 *)0x020317D6;
    table = gZoidBaseStatTable;
    temp_r4 = (u8 *)*arg1;
    call3 = temp_r4[1];
    offset = call3 << 2;
    offset += call1 * 0x38;
    call2 = ((struct TableEntry *)(table + offset))->value;
    StartBattleEquipmentAnimation(call0, call1, call2, call3, 0);
    while ((GetBattleAnimationResult() << 0x18) == 0) {
        func_080ED17C(1);
    }
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
