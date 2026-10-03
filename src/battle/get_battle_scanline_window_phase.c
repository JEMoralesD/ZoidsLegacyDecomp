#include "m2c_prelude.h"
#include "battle_animation.h"
s32 GetBattleScanlineWindowPhase(void) asm("func_080D1E38");

s32 GetBattleScanlineWindowPhase(void) {
    register s32 phase asm("r0");
    register u8 window_mode asm("r1");

    window_mode = *(u8 *)0x02034863;
    if (window_mode != BATTLE_SCANLINE_WINDOW_DISABLED) {
        phase = BATTLE_SCANLINE_WINDOW_PHASE_TRANSITION;
        if (window_mode == BATTLE_SCANLINE_WINDOW_ACTIVE) {
            return BATTLE_SCANLINE_WINDOW_PHASE_ACTIVE;
        }
        return phase;
    }
    phase = BATTLE_SCANLINE_WINDOW_PHASE_DISABLED;
    return phase;
}
