#include "m2c_prelude.h"
#include "battle_animation.h"
extern u8 gBattleBackgroundShakeState asm("D_02034869");
extern u8 gBattleBackgroundShakeFrame asm("D_0203486A");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
s16 Sin256(s32) asm("func_08092A90");
void StopBattleBackgroundShake(void) asm("func_080D220C");

void UpdateBattleBackgroundShake(void) asm("func_080D2244");

void UpdateBattleBackgroundShake(void) {
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r3;
    s32 var_r0;
    s32 phase;
    s32 *background_scroll;
    u8 *shake_phase;

    if (gBattleBackgroundShakeState == BATTLE_BACKGROUND_SHAKE_ACTIVE) {
        goto block_6;
    }
    if (gBattleBackgroundShakeState == BATTLE_BACKGROUND_SHAKE_STOPPING) {
        phase = gBattleBackgroundShakeFrame;
        temp_r1 = 0x7F;
        temp_r1 &= phase;
        if ((temp_r1 == 0) || (temp_r1 == 0x40)) {
            StopBattleBackgroundShake();
            return;
        }
block_6:
        background_scroll = gFieldCameraScrollOffsets;
        shake_phase = &gBattleBackgroundShakeFrame;
        temp_r2 = (s16)Sin256(*shake_phase * 2);
        temp_r3 = background_scroll[0];
        if (*shake_phase & 1) {
            var_r0 = temp_r3 - (temp_r2 * 8);
        } else {
            var_r0 = temp_r3 + (temp_r2 * 8);
        }
        background_scroll[2] = var_r0;
        gBattleBackgroundShakeFrame++;
    }
}
