#include "m2c_prelude.h"
#include "player_state.h"
s32 SubtractEquipmentFromInventory(u16 equipment_id, u8 quantity) asm("func_080E5D38");

s32 SubtractEquipmentFromInventory(u16 equipment_id, u8 quantity) {
    extern u8 gPlayerStateBytes asm("D_020218E4");
    extern u8 gPlayerEquipmentQuantitiesOffset asm("D_off_6934");
    u16 item_id;
    u8 current_quantity;
    register u8 removed_quantity asm("r2");
    u32 quantity_table_address;
    u8 *quantity_slot;

    item_id = equipment_id;
    removed_quantity = quantity;
    quantity_table_address = (u32)&gPlayerStateBytes + (u32)&gPlayerEquipmentQuantitiesOffset;
    quantity_slot = (u8 *)(item_id + quantity_table_address);
    current_quantity = *quantity_slot;
    if ((u32)current_quantity > (u32)removed_quantity) {
        register s32 remaining_quantity asm("r0");
        remaining_quantity = current_quantity;
        asm volatile("" : "+r"(remaining_quantity));
        remaining_quantity -= removed_quantity;
        asm volatile("" : "+r"(remaining_quantity));
        *quantity_slot = remaining_quantity;
        return PLAYER_STORAGE_SUBTRACT_REMAINS;
    }
    {
        register s32 zero asm("r0");
        zero = 0;
        asm volatile("" : "+r"(zero));
        *quantity_slot = zero;
        return zero;
    }
}
