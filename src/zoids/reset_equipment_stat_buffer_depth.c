#include "m2c_prelude.h"
#include "../game/player_state.h"
extern s8 gEquipmentStatBufferDepth asm("D_02030554");
void ResetEquipmentStatBufferDepth(void) asm("func_080E6690");

void ResetEquipmentStatBufferDepth(void) {
    gEquipmentStatBufferDepth = 0;
}
