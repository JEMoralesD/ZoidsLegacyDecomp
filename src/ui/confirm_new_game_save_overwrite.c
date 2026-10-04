#include "m2c_prelude.h"
#include "title_menu.h"
#include "../graphics/screen_effects.h"
M2C_UNK ClearSpritePools() asm("func_08094330");
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete() asm("func_0809669C");
M2C_UNK InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");
M2C_UNK LoadMenuGradientBackground(s32, s32, s32, s32, s32) asm("func_0809AB44");
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 ConfirmNewGameSaveOverwrite(void) asm("func_0809B8C4");

s32 ConfirmNewGameSaveOverwrite(void) {
    *(s16 *)0x0300004C = 0x1840;
    InitializeWindowGraphics(3, 1, 0, 0x35C, 0x3C0, 0, 0xE, 0, 0x3E6, 0xF);
    LoadMenuGradientBackground(2, 3, 0, 0, 1);
    ClearSpritePools();
    RunMenuScript(0x08000934);
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0);
    RunMenuScript(0x08000AE0);
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_CONCEAL, 0);
    goto check_transition;

wait_for_transition:
    YieldTaskForUpdates(1);
check_transition:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_for_transition;
    }
    if ((*(u8 *)0x0200A882 == 1) && (*(u8 *)0x0200A880 == 0)) {
        return 1;
    }
    return 0;
}
