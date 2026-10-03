#include "m2c_prelude.h"
#include "../event_script.h"
extern void func_0809F850(void);
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void func_080ED17C(s32);
extern u8 D_02030664;
extern s32 gGameMode;
extern u8 D_02030666;
extern s32 D_02031744;
extern u8 gBattleSetup[];
extern u8 D_02031748;
extern u8 D_020317D8;
extern u8 D_020317D9;
extern u8 D_02033F34;

s32 EventEnterBattleScene(u8 script_slot, u8 **cursor) {
    s32 *p690;
    u8 *p666;
    s32 *p744;
    int zero;
    D_02030664 = 1;
    p690 = &gGameMode;
    if (*p690 != 0xA) {
        p666 = &D_02030666;
        *p666 = 0;
        p744 = &D_02031744;
        zero = 0;
        *p744 = zero;
        func_0809F850();
        gBattleSetup[2] = (*cursor)[1];
        D_02031748 = (*cursor)[2];
        D_020317D8 = zero;
        D_020317D9 = zero;
        *p690 = 0xA;
        D_02033F34 = zero;
        func_080ED17C(1);
        *p666 = zero;
        *p744 = zero;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
