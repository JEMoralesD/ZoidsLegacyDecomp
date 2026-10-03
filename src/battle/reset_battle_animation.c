#include "m2c_prelude.h"
#include "battle_animation.h"
extern void ResetZoidEquipmentAnimation(void) asm("func_080D0480");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void ResetBattleScanlineWindow(void) asm("func_080D1C38");
extern void ResetBattleBackgroundShake(void) asm("func_080D2180");
extern void ResetBattleAnimationResourceAllocation(void) asm("func_080D2328");
extern s8 gBattleAnimationState asm("D_02033FD0");
extern s32 gBattleZoidScrollX asm("D_02034034");
extern s32 gBattleTerrainScrollX asm("D_02034038");
extern s32 gBattleZoidScrollStep asm("D_0203403C");
extern s32 gBattleTerrainScrollStep asm("D_02034040");
extern s32 gBattleZoidScrollOffset asm("D_02034044");
extern s32 gBattleTerrainScrollOffset asm("D_02034048");
extern s8 gBattleSpeedLineBackgroundFrame asm("D_02034860");
extern s8 gBattleBackgroundSlideFrame asm("D_02034861");

void ResetBattleAnimation(s32 initial_scroll_x) asm("func_080D0AF0");

void ResetBattleAnimation(s32 initial_scroll_x) {
    s8 *animation_status = &gBattleAnimationState;
    s32 zero = 0;
    *animation_status = zero;
    SetBattleAnimationCameraMode(1, 0);
    gBattleZoidScrollX = initial_scroll_x << 8;
    gBattleTerrainScrollX = zero;
    { s32 *lo = &gBattleZoidScrollStep; s32 *hi = &gBattleTerrainScrollStep; *hi = zero; *lo = zero; }
    { s32 *lo = &gBattleZoidScrollOffset; s32 *hi = &gBattleTerrainScrollOffset; *hi = zero; *lo = zero; }
    ResetZoidEquipmentAnimation();
    ResetBattleAnimationResourceAllocation();
    ResetBattleScanlineWindow();
    ResetBattleBackgroundShake();
    gBattleSpeedLineBackgroundFrame = zero;
    gBattleBackgroundSlideFrame = zero;
}
