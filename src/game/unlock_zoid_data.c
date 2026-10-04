#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerItemInventoryBytes[] asm("D_020217F4");
extern s32 gPlayerCatalogFlagsBytes[] asm("D_020217B4");
s32 UnlockZoidData(s32 zoid_model_id) asm("func_080E5DC4");

s32 UnlockZoidData(s32 zoid_model_id) {
    u32 model_id_bits = zoid_model_id << 0x18;
    s32 unlocked_models, model_bit, item_inventory_address, catalog_word_index;
    s32 *unlocked_data_word, *catalog_word;
    s32 catalog_word_offset;
    s32 *unlocked_data_words;
    u32 model_bit_index;
    catalog_word_index = model_id_bits >> 0x1D;
    model_bit_index = 0x1F000000;
    model_bit_index &= model_id_bits;
    model_bit_index >>= 0x18;
    model_bit = 1 << model_bit_index;
    item_inventory_address = (s32)gPlayerItemInventoryBytes;
    catalog_word_offset = catalog_word_index * 4;
    unlocked_data_words = (s32 *)(item_inventory_address + PLAYER_ITEM_INVENTORY_OFFSET(unlocked_zoid_data));
    unlocked_data_word = (s32 *)(catalog_word_offset + (s32)unlocked_data_words);
    unlocked_models = *unlocked_data_word;
    if (unlocked_models & model_bit) {
        return PLAYER_DATA_ALREADY_UNLOCKED;
    }
    *unlocked_data_word = unlocked_models | model_bit;
    catalog_word = (s32 *)(catalog_word_index * 4 + (s32)gPlayerCatalogFlagsBytes);
    *catalog_word |= model_bit;
    return PLAYER_DATA_NEWLY_UNLOCKED;
}
