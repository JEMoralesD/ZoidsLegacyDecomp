#include "m2c_prelude.h"
#include "../game/player_state.h"
#include "battle.h"

extern volatile u32 gBattleRuleFlags asm("D_0202F090");

void DisableBattleRule(u8 rule_id) asm("func_080E664C");

void DisableBattleRule(u8 rule_id)
{
    gBattleRuleFlags &= ~(1U << rule_id);
}
