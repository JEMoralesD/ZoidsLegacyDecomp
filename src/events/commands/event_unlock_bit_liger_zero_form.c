#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
extern struct PlayerZoidRecordView gPlayerZoidRecords[] asm("D_020218E8");
extern s32 gPlayerCatalogFlags[] asm("D_020217B4");

extern u8 GetZoidFormId(void *, u8) asm("func_080E5320");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");

s32 EventUnlockBitLigerZeroForm(u8 script_slot, struct EventBitLigerFormIndexCommand **script_cursor) asm("func_080A64C8");

s32 EventUnlockBitLigerZeroForm(u8 script_slot, struct EventBitLigerFormIndexCommand **script_cursor) {
    u8 storage_slot = 1;
    struct PlayerZoidRecordView *zoid_records = gPlayerZoidRecords;
    u8 *player_state = (u8 *)zoid_records - 4;
    s32 form_flag_bit = 1;
    s32 *catalog_words = gPlayerCatalogFlags;
    for (; storage_slot <= 0xCE; storage_slot++) {
        struct PlayerZoidRecordView *zoid_record = (struct PlayerZoidRecordView *)(storage_slot * 0x70 + (s32)zoid_records);
        u8 pilot_id = player_state[(zoid_record->pilot_slot << 6) + 0x5A94];
        if ((pilot_id == BIT_PILOT_ID || pilot_id == BIT_ALTERNATE_PILOT_ID) && (u8)(zoid_record->model_id - 0x19) <= 5) {
            u32 model_id_bits;
            u32 model_catalog_bit_index_bits;
            register s32 *catalog_word_address asm("r3");
            zoid_record->form_flags = (form_flag_bit << (*script_cursor)->form_index) | zoid_record->form_flags;
            model_id_bits = GetZoidFormId(zoid_record, (*script_cursor)->form_index) << 0x18;
            catalog_word_address = &catalog_words[model_id_bits >> 0x1D];
            model_catalog_bit_index_bits = 0x1F000000;
            model_catalog_bit_index_bits &= model_id_bits;
            *catalog_word_address |= form_flag_bit << (model_catalog_bit_index_bits >> 0x18);
            break;
        }
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
