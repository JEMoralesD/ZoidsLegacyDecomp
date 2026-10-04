#include "m2c_prelude.h"
#include "battle.h"
s32 TestBattleRuleFlag(u8 rule_id) asm("func_080E6664");

s32 TestBattleRuleFlag(u8 rule_id) {
    s32 enabled;

    enabled = *(s32 *)0x0202F090 & (1 << rule_id);
    if (enabled != 0) {
        enabled = 1;
    }
    return enabled;
}
