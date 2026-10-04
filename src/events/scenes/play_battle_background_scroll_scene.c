#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"
M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK DisableDisplayWindows() asm("func_0809534C");                            /* extern */
M2C_UNK ConfigureDisplayWindows(s32, s32, M2C_UNK, s32, s32, s32, s32, s32) asm("func_0809538C"); /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK LoadSceneBackgroundGraphics(s32, s32, s32, s32, s32) asm("func_0809A5B4");     /* extern */
M2C_UNK ResetBattleAnimation(s32) asm("func_080D0AF0");                         /* extern */
M2C_UNK SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");                    /* extern */
M2C_UNK BiosCpuSet(M2C_UNK, M2C_UNK, M2C_UNK) asm("func_080ECD2C");   /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

void PlayBattleBackgroundScrollScene(void) asm("func_080A7968");

void PlayBattleBackgroundScrollScene(void) {
    u32 elapsed_updates;

    *(s32 *)0x02021690 = GAME_MODE_BATTLE_SCENE;
    YieldTaskForUpdates(1);
    ResetBattleAnimation(0);
    SetBattleAnimationCameraMode(0, 0);
    *(s16 *)0x0300004C = 0x1140;
    *(s16 *)0x04000008 = 0x87;
    *(s32 *)0x03000054 = 0;
    LoadSceneBackgroundGraphics(0, 0, 1, 0, 1);
    *(s16 *)0x05000000 = 0;
    *(s8 *)0x0203198D = 0;
    *(u8 *)0x03000074 |= 8;
    *(s8 *)0x0203198C = 1;
    BiosCpuSet(0x08000844, 0x030060FC, 0x04000014);
    StartTask(4, 0x080A7915);
    ConfigureDisplayWindows(1, 0xF0, 0x148C, 0, 0, 0, 0x3F, 0);
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    goto loop_2;
loop_1:
    YieldTaskForUpdates(1);
loop_2:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto loop_1;
    }
    elapsed_updates = 0;
    do {
        YieldTaskForUpdates(1);
        elapsed_updates += 1;
    } while (elapsed_updates <= 0x12BU);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    goto loop_4;
loop_3:
    YieldTaskForUpdates(1);
loop_4:
loop_8:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto loop_3;
    }
    StopTask(4);
    *(s8 *)0x0203198C = 3;
    DisableDisplayWindows();
    *(s32 *)0x02021690 = -1;
    YieldTaskForUpdates(1);
}
