#include "m2c_prelude.h"
#include "battle.h"
M2C_UNK ResetBattleActionSelection() asm("func_080CA184");                                /* extern */
M2C_UNK BuildBattleActionCandidates() asm("func_080CA238");                                /* extern */
M2C_UNK AppendRandomBattleAction() asm("func_080CA650");                                /* extern */

void ChooseRandomBattleAction(void) asm("func_080CA1A0");

void ChooseRandomBattleAction(void) {
    ResetBattleActionSelection();
    BuildBattleActionCandidates();
    AppendRandomBattleAction();
}
