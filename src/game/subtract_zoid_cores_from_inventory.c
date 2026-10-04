#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerItemInventoryBytes[] asm("D_020217F4");
s32 SubtractZoidCoresFromInventory(u8 core_id, u8 quantity) asm("func_080E5E38");

s32 SubtractZoidCoresFromInventory(u8 core_id, u8 quantity) {
    struct PlayerItemInventory *inventory = (struct PlayerItemInventory *)gPlayerItemInventoryBytes;
    u8 current_quantity = inventory->zoid_core_quantities[core_id];
    if (current_quantity > quantity) { current_quantity -= quantity; inventory->zoid_core_quantities[core_id] = current_quantity; return PLAYER_STORAGE_SUBTRACT_REMAINS; }
    inventory->zoid_core_quantities[core_id] = 0;
    return PLAYER_STORAGE_SUBTRACT_DEPLETED;
}
