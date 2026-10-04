#include "m2c_prelude.h"
#include "../game/player_state.h"
#include "battle.h"
extern s32 gBattleRuleFlags asm("D_0202F090");
void ClearBattleRules(void) asm("func_080E6684");

void ClearBattleRules(void) {
    gBattleRuleFlags = 0;
}
