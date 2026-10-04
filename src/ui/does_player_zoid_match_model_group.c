#include "player_selection.h"

s32 DoesPlayerZoidMatchModelGroup(u8 *zoid_record, s32 model_id_or_group_id) asm("func_080B6160");

s32 DoesPlayerZoidMatchModelGroup(u8 *zoid_record, s32 model_id_or_group_id) {
    s32 group_index_bits;
    s32 group_row_offset;
    u8 group_member_index;
    register s32 record_address_or_model_id asm("r6");
    register u8 *model_groups asm("r5");
    register s32 group_index_times_two asm("r3");
    register u8 *model_group_table asm("r2");

    record_address_or_model_id = (s32)zoid_record;
    model_id_or_group_id <<= 24;
    model_id_or_group_id = (u32)model_id_or_group_id >> 24;
    if ((u32)model_id_or_group_id <= 0xC7) {
        s32 matches_model;

        matches_model = 0;
        record_address_or_model_id = *(u8 *)record_address_or_model_id;
        if (model_id_or_group_id == record_address_or_model_id) {
            matches_model = 1;
        }
        return matches_model;
    }

    group_index_bits = model_id_or_group_id;
    group_index_bits += 0x38;
    model_id_or_group_id = (u8)group_index_bits;
    group_member_index = 0;
    model_group_table = (u8 *)PLAYER_ZOID_MODEL_GROUP_TABLE_ROM;
    group_index_times_two = model_id_or_group_id << 1;
    group_row_offset = (group_index_times_two + model_id_or_group_id) << 1;
    if (*(u8 *)(group_row_offset + (s32)model_group_table) != 0) {
        s32 group_row_offset_reloaded;

        model_groups = model_group_table;
        record_address_or_model_id = *(u8 *)record_address_or_model_id;
        do {
            asm volatile("" : "+r"(group_index_times_two));
            group_row_offset_reloaded = (group_index_times_two + model_id_or_group_id) << 1;
            if (record_address_or_model_id == *(u8 *)(group_member_index + group_row_offset_reloaded + (s32)model_groups)) {
                return 1;
            }
            group_member_index += 1;
            if ((u32)group_member_index > 5) {
                break;
            }
        } while (*(u8 *)(group_member_index + group_row_offset_reloaded + (s32)model_groups) != 0);
    }
    return 0;
}
