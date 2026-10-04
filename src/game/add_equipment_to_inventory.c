#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerCatalogFlagsBytes[] asm("D_020217B4");
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u8 gPlayerEquipmentQuantitiesOffset[] asm("D_off_6934");
s32 AddEquipmentToInventory(u16 equipment_id, s32 quantity) asm("func_080E5CE4");

s32 AddEquipmentToInventory(u16 equipment_id, s32 quantity) {
    u16 item_id; s32 updated_quantity; u32 equipment_id_bits;
    s32 catalog_flags_address, catalog_word_offset, quantity_table_address; s32 *equipment_catalog_words, *catalog_word; u8 *quantity_slot; u32 catalog_bit_index;
    equipment_id_bits = equipment_id << 0x10;
    item_id = equipment_id_bits >> 0x10;
    quantity = quantity << 0x18;
    catalog_flags_address = (s32)gPlayerCatalogFlagsBytes;
    catalog_word_offset = (equipment_id_bits >> 0x15) * 4;
    equipment_catalog_words = (s32 *)(catalog_flags_address + PLAYER_CATALOG_OFFSET(equipment));
    catalog_word = (s32 *)((s32)equipment_catalog_words + catalog_word_offset);
    catalog_bit_index = 0x1F; catalog_bit_index &= item_id;
    *catalog_word |= 1 << catalog_bit_index;
    quantity_table_address = (s32)gPlayerStateBytes;
    quantity_table_address += (s32)gPlayerEquipmentQuantitiesOffset;
    quantity_slot = (u8 *)(item_id + quantity_table_address);
    updated_quantity = ((u32)quantity >> 0x18) + *quantity_slot;
    if (updated_quantity <= PLAYER_INVENTORY_QUANTITY_LIMIT) { *quantity_slot = (u8)updated_quantity; return PLAYER_STORAGE_ADD_WITHIN_LIMIT; }
    *quantity_slot = PLAYER_INVENTORY_QUANTITY_LIMIT;
    return PLAYER_STORAGE_ADD_CAPPED;
}
