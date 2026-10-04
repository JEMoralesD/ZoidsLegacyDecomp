#include "m2c_prelude.h"
#include "../game/player_state.h"
#include "battle.h"
extern u32 gBattleRuleFlags asm("D_0202F090");

void EnableBattleRule(u8 rule_id) asm("func_080E65B4");

void EnableBattleRule(u8 rule_id) {
    if ((u8)(rule_id - 1) <= 3) gBattleRuleFlags &= 0xFFFFFFE1;
    else if ((u8)(rule_id - 5) <= 5) gBattleRuleFlags &= 0xFFFFF81F;
    else if ((u8)(rule_id - 11) <= 5) gBattleRuleFlags &= 0xFFFE07FF;
    else if (rule_id == BATTLE_RULE_MELEE_ONLY) gBattleRuleFlags &= 0xFFF7FFFF;
    else if (rule_id == BATTLE_RULE_NO_MELEE) gBattleRuleFlags &= 0xFFFBFFFF;
    gBattleRuleFlags |= 1 << rule_id;
}
