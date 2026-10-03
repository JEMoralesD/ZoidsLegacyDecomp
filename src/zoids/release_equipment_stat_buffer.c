#include "m2c_prelude.h"
#include "../battle/battle.h"
void ReleaseEquipmentStatBuffer(void) asm("func_080E66B8");

void ReleaseEquipmentStatBuffer(void) {
    *(u8 *)0x02030554 -= 1;
}
