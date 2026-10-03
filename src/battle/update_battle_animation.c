#include "m2c_prelude.h"
#include "battle_animation.h"
extern u8 gBattleAnimationState asm("D_02033FD0");
extern u8 gBattleAnimationFinishDelay asm("D_02034050");
s32 IsZoidEquipmentAnimationReady() asm("func_080D0AE4");                                    /* extern */
M2C_UNK UpdateBattleAnimationScript() asm("func_080D1090");                                /* extern */
M2C_UNK SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");                        /* extern */
s32 IsBattleAnimationCameraReady() asm("func_080D18CC");                                    /* extern */
M2C_UNK UpdateBattleSpeedLineBackground() asm("func_080D18F0");                                /* extern */
s32 IsBattleSpeedLineBackgroundReady() asm("func_080D1A24");                                    /* extern */
M2C_UNK UpdateBattleBackgroundSlide() asm("func_080D1A44");                                /* extern */
s32 IsBattleBackgroundSlideFinished() asm("func_080D1B58");                                    /* extern */

void UpdateBattleAnimation(void) asm("func_080D1B78");

void UpdateBattleAnimation(void) {
    s32 finish_delay;

    UpdateBattleSpeedLineBackground();
    UpdateBattleBackgroundSlide();
    if (M2C_FIELD((void *)((u32)&gBattleAnimationState), u8 *, 0) != 0) {
        if (((IsBattleSpeedLineBackgroundReady() << 0x18) != 0) && ((IsBattleBackgroundSlideFinished() << 0x18) != 0) && ((IsZoidEquipmentAnimationReady() << 0x18) != 0)) {
            if ((M2C_FIELD((void *)((u32)&gBattleAnimationState), s32 *, 0xC) & 0xFFFF00) == (BATTLE_ANIMATION_NO_IMPACT << 8)) {
                SetBattleAnimationCameraMode(0xD, 0);
            }
            UpdateBattleAnimationScript();
            if ((M2C_FIELD((void *)((u32)&gBattleAnimationState), u8 *, 0) == BATTLE_ANIMATION_SCRIPT_FINISHED) && ((IsBattleAnimationCameraReady() << 0x18) != 0)) {
                finish_delay = *(s32 *)((u32)&gBattleAnimationFinishDelay) + 1;
                *(s32 *)((u32)&gBattleAnimationFinishDelay) = finish_delay;
                if (finish_delay == BATTLE_ANIMATION_FINISH_DELAY_FRAMES) {
                    M2C_FIELD((void *)((u32)&gBattleAnimationState), u8 *, 0) = BATTLE_ANIMATION_IDLE;
                }
            }
        }
        if ((*(u8 *)0x02030664 != 1) && (2 & *(u16 *)0x0300000C)) {
            M2C_FIELD((void *)((u32)&gBattleAnimationState), u8 *, 0) = BATTLE_ANIMATION_SKIPPED;
        }
    }
}
