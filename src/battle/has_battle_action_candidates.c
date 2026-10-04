#include "m2c_prelude.h"
#include "battle.h"
u32 HasBattleActionCandidates(void) asm("func_080CA560");

u32 HasBattleActionCandidates(void) {
    u8 candidate_count;

    candidate_count = *(u8 *)0x0203EFA8;
    return (u32) ((0 - candidate_count) | candidate_count) >> 0x1F;
}
