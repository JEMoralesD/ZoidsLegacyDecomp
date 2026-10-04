#include "m2c_prelude.h"
extern u16 gLinkBattleResults[] asm("D_0202F08C");
void ResetLinkBattleResults(void) asm("func_0809A0A0");

void ResetLinkBattleResults(void) {
    gLinkBattleResults[1] = 0;
    gLinkBattleResults[0] = 0;
}
