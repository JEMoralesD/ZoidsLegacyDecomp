#include "m2c_prelude.h"
#include "../graphics/screen_effects.h"
#include "../game/game_state.h"
extern void RunMainLoop(void) asm("func_080921D4");
extern void WaitForFrameUpdates(u8) asm("func_0809223C");
extern int InitializeEngine() asm("func_08092634");
extern void StartTask() asm("func_08092D8C");
extern void InitializeScreenTransitions(void) asm("func_080962D8");
extern void StartScreenTransition(int, int) asm("func_08096308");
extern int IsScreenTransitionComplete(void) asm("func_0809669C");
extern void BuildScreenTransitionFrame(void) asm("func_08096774");
extern void ResetMenuKeyRepeat(void) asm("func_08096F3C");
extern void UpdateSoundMixer(void) asm("func_080EB748");
extern void InitSramAccess(void) asm("func_080ECC48");

extern s32 gGameMode;
extern s8 D_02021694;

void InitializeGame(void) asm("func_080920EC");

void InitializeGame(void) {
    register int z4 asm("r4");
    volatile u32 *dma;
    volatile u16 *disp;
    volatile u16 *shadow;
    u32 v;

    *(volatile u16 *)0x04000204 = *(volatile u16 *)0x087A0A10;

    v = 0;
    dma = (volatile u32 *)0x040000D4;
    dma[0] = (u32)&v;
    dma[1] = 0x02000000;
    dma[2] = 0x85010000;
    dma[2];
    v = 0;
    dma[0] = (u32)&v;
    dma[1] = 0x03000000;
    dma[2] = 0x85001EC0;
    InitializeEngine(dma[2]);

    *(volatile u16 *)0x04000208 = 1;
    *(volatile u16 *)0x04000200 = 0x2001;
    *(volatile u16 *)0x04000004 = 8;
    shadow = (volatile u16 *)0x0300004C;
    disp = (volatile u16 *)0x04000000;
    z4 = 0;
    *disp = 0;
    *shadow = *disp;
    UpdateSoundMixer();
    InitSramAccess();
    *(volatile s8 *)0x03000074 = z4;
    InitializeScreenTransitions();
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_WHITE, 8);
    while ((IsScreenTransitionComplete() << 0x18) == 0) {
        BuildScreenTransitionFrame();
        WaitForFrameUpdates(*(volatile u8 *)0x03000075);
    }
    ResetMenuKeyRepeat();
    D_02021694 = 0;
    gGameMode = GAME_MODE_INTRO;
    StartTask(1, 0x0809A0B1);
    RunMainLoop();
}
