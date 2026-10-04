#include "m2c_prelude.h"
#include "window.h"
#include "scrolling_text.h"
#include "../battle/battle_display.h"

enum ZoidViewerDisplayControl {
    ZOID_VIEWER_BG3_ENABLE = 0x800
};

extern s32 gScrollingTextArrowSprites[] asm("D_02032E68");
M2C_UNK DestroySprite(s32) asm("func_8094554");
M2C_UNK UpdateBattleAnimationCamera(void) asm("func_080D12DC");
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");

void DestroyScrollingTextWindowArrows(void) asm("func_080E2F30");

void DestroyScrollingTextWindowArrows(void) {
    DestroySprite(gScrollingTextArrowSprites[SCROLLING_TEXT_UP_ARROW]);
    DestroySprite(gScrollingTextArrowSprites[SCROLLING_TEXT_DOWN_ARROW]);
}

void RunBattleCameraUpdateTask(void) asm("func_080E2F4C");

void RunBattleCameraUpdateTask(void) {
next_camera_update:
    UpdateBattleAnimationCamera();
    YieldTaskForUpdates(1);
    goto next_camera_update;
}

void EnableBg3(void) asm("func_080E2F5C");

void EnableBg3(void) {
    *(u16 *)0x04000000 |= ZOID_VIEWER_BG3_ENABLE;
}
