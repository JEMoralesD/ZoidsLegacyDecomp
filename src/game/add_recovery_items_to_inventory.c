#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerRecoveryItemQuantities[] asm("D_020217F4");
s32 AddRecoveryItemsToInventory(u8 item_id, u8 quantity) asm("func_080E5D6C");

s32 AddRecoveryItemsToInventory(u8 item_id, u8 quantity) {
    s32 updated_quantity = quantity + gPlayerRecoveryItemQuantities[item_id];
    if (updated_quantity <= PLAYER_INVENTORY_QUANTITY_LIMIT) {
        gPlayerRecoveryItemQuantities[item_id] = (u8)updated_quantity;
        return PLAYER_STORAGE_ADD_WITHIN_LIMIT;
    }
    gPlayerRecoveryItemQuantities[item_id] = PLAYER_INVENTORY_QUANTITY_LIMIT;
    return PLAYER_STORAGE_ADD_CAPPED;
}
