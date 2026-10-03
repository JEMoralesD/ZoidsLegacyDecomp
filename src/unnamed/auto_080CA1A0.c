#include "m2c_prelude.h"
M2C_UNK ResetBattleActionSelection() asm("func_080CA184");                                /* extern */
M2C_UNK func_80CA238();                                /* extern */
M2C_UNK func_80CA650();                                /* extern */

void sub_080CA1A0(void) {
    ResetBattleActionSelection();
    func_80CA238();
    func_80CA650();
}
