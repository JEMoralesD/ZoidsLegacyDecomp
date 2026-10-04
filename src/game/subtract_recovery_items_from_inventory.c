#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerRecoveryItemQuantities[] asm("D_020217F4");
s32 SubtractRecoveryItemsFromInventory(u8 item_id, u8 quantity) asm("func_080E5D98");

s32 SubtractRecoveryItemsFromInventory(u8 item_id, u8 quantity) {
    u8 current_quantity = gPlayerRecoveryItemQuantities[item_id];
    if (current_quantity > quantity) { current_quantity -= quantity; gPlayerRecoveryItemQuantities[item_id] = current_quantity; return PLAYER_STORAGE_SUBTRACT_REMAINS; }
    gPlayerRecoveryItemQuantities[item_id] = 0;
    return PLAYER_STORAGE_SUBTRACT_DEPLETED;
}
