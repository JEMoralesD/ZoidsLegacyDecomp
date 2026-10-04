#include "m2c_prelude.h"
#include "player_state.h"
M2C_UNK BiosCpuSet() asm("func_80ECD2C");
void ClearPlayerItemInventory(void) asm("func_08099FEC");

void ClearPlayerItemInventory(void) {
    s32 clear_source = 0;
    BiosCpuSet(&clear_source, PLAYER_ITEM_INVENTORY_RAM, PLAYER_ITEM_INVENTORY_CLEAR_CONTROL);
}
