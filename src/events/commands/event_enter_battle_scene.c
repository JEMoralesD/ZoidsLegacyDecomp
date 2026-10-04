#include "m2c_prelude.h"
#include "../event_script.h"
extern void InitializeEventSpritePool(void) asm("func_0809F850");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");
extern u8 gFieldEventActive asm("D_02030664");
extern s32 gGameMode;
extern u8 D_02030666;
extern s32 D_02031744;
extern u8 gBattleSetup[];
extern u8 D_02031748;
extern u8 D_020317D8;
extern u8 gBattleSpeedLinesPending asm("D_020317D9");
extern u8 D_02033F34;

s32 EventEnterBattleScene(u8 script_slot, u8 **cursor) {
    s32 *p690;
    u8 *p666;
    s32 *p744;
    int zero;
    gFieldEventActive = 1;
    p690 = &gGameMode;
    if (*p690 != 0xA) {
        p666 = &D_02030666;
        *p666 = 0;
        p744 = &D_02031744;
        zero = 0;
        *p744 = zero;
        InitializeEventSpritePool();
        gBattleSetup[2] = (*cursor)[1];
        D_02031748 = (*cursor)[2];
        D_020317D8 = zero;
        gBattleSpeedLinesPending = zero;
        *p690 = 0xA;
        D_02033F34 = zero;
        YieldTaskForUpdates(1);
        *p666 = zero;
        *p744 = zero;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
