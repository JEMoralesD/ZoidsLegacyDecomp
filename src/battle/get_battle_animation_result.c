#include "m2c_prelude.h"
#include "battle_animation.h"
s32 GetBattleAnimationResult(void) asm("func_080D1C18");

s32 GetBattleAnimationResult(void) {
    register s32 result asm("r0");
    register u8 status asm("r1");

    status = *(u8 *)0x02033FD0;
    if (status != BATTLE_ANIMATION_IDLE) {
        result = BATTLE_ANIMATION_RESULT_WAITING;
        if (status == BATTLE_ANIMATION_SKIPPED) {
            return BATTLE_ANIMATION_RESULT_SKIPPED;
        }
        return result;
    }
    result = BATTLE_ANIMATION_RESULT_FINISHED;
    return result;
}
