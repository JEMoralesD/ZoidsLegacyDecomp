#include "player_selection.h"

extern u8 gPlayerZoidSelectionCount asm("D_02032272");
extern u8 gPlayerZoidSelectionSlots[] asm("D_020321A4");
extern u8 gPlayerState[] asm("D_020218E4");
extern u8 gPlayerZoidRecords[] asm("D_020218E8");

u8 DoesPlayerZoidMatchModelGroup(void *, s32) asm("func_080B6160");

void BuildFilteredPlayerZoidSelection(u16 required_flags, u16 excluded_flags, s32 model_id_or_group_id) asm("func_080B61C8");

void BuildFilteredPlayerZoidSelection(u16 required_flags, u16 excluded_flags, s32 model_id_or_group_id)
{
    register u16 excluded_flags_r8 asm("r8") = excluded_flags;
    s32 model_filter;
    s16 team_selection_count;
    register s32 allocation_guard_r7 asm("r7");

    model_filter = (u8)model_id_or_group_id;
    asm volatile("" :: "r"(model_filter));
    asm volatile("" : "=r"(allocation_guard_r7));
    gPlayerZoidSelectionCount = 0;
    {
        register u8 *player_state asm("r9");
        register u8 *zoid_records asm("r10");
        s16 i;
        u8 *selection_count_address;

        i = 0;
        player_state = gPlayerState;
        zoid_records = player_state + 4;
        selection_count_address = &gPlayerZoidSelectionCount;
        do {
            register s32 signed_i asm("r0");
            register u8 *team_slots asm("r1");
            register u8 *team_slot_address asm("r4");
            u8 storage_slot;
            register u32 record_offset asm("r0");
            u8 *zoid_record_base;
            register u8 *zoid_record asm("r6");
            u16 zoid_flags;
            u16 required_flag_bits;

            signed_i = (s16)i;
            team_slots = player_state + PLAYER_STATE_OFFSET(team_zoid_slots);
            team_slot_address = (u8 *)((u32)signed_i + (u32)team_slots);
            storage_slot = *team_slot_address;
            record_offset = storage_slot * 7;
            record_offset <<= 4;
            zoid_record_base = zoid_records;
            zoid_record = (u8 *)(record_offset + (u32)zoid_record_base);
            asm volatile("" : "+r"(i));

            if (storage_slot != 0 &&
                (model_filter == 0 || DoesPlayerZoidMatchModelGroup(zoid_record, model_filter))) {
                zoid_flags = *(u16 *)(zoid_record + 4);
                required_flag_bits = required_flags;
                required_flag_bits &= zoid_flags;
                if (required_flag_bits == required_flags &&
                    (zoid_flags & excluded_flags_r8) == 0) {
                    gPlayerZoidSelectionSlots[*selection_count_address] = *team_slot_address;
                    (*selection_count_address)++;
                }
            }
            i++;
        } while (i <= 5);
    }

    {
        register u8 *count_base asm("r0") = &gPlayerZoidSelectionCount;

        team_selection_count = *count_base;
        {
            s16 i;
            register u8 *selection_count_address asm("r4");
            u32 next2;

            i = 1;
            selection_count_address = count_base;
            do {
                register u32 i_shift_low asm("r2");
                register u32 i_shift asm("r10");
                register s32 signed_i2 asm("r1");
                register u32 record_offset2 asm("r0");
                register u8 *record_base2 asm("r1");
                register u8 *zoid_record asm("r6");
                u8 model_id;
                u16 zoid_flags;
                u16 required_flag_bits_reloaded;

                i_shift_low = (u16)i << 16;
                signed_i2 = (s32)i_shift_low >> 16;
                record_offset2 = signed_i2 * 7;
                record_offset2 <<= 4;
                record_base2 = gPlayerZoidRecords;
                asm volatile("" : "+r"(record_base2));
                zoid_record = (u8 *)(record_offset2 + (u32)record_base2);
                model_id = zoid_record[0];
                i_shift = i_shift_low;

                if (model_id != 0) {
                    zoid_flags = *(u16 *)(zoid_record + 4);
                    if (!(zoid_flags & 4) &&
                        (model_filter == 0 || DoesPlayerZoidMatchModelGroup(zoid_record, model_filter))) {
                        zoid_flags = *(u16 *)(zoid_record + 4);
                        required_flag_bits_reloaded = required_flags;
                        required_flag_bits_reloaded &= zoid_flags;
                        if (required_flag_bits_reloaded == required_flags &&
                            (zoid_flags & excluded_flags_r8) == 0) {
                            gPlayerZoidSelectionSlots[*selection_count_address] = i;
                            (*selection_count_address)++;
                        }
                    }
                }
                next2 = i_shift + 0x10000;
                i = (u16)(next2 >> 16);
            } while ((s32)next2 >> 16 <= 206);
        }
    }

    {
        u32 i;
        u32 first_shift;
        register u32 first_seed asm("r1");
        u32 initial_next;
        u32 next;
        register u32 outer_shift asm("r2");
        register s32 outer asm("r1");

        first_seed = (u16)team_selection_count;
        first_shift = (u16)first_seed;
        first_shift <<= 16;
        asm volatile("" :: "r"(first_shift));
        asm volatile("" ::
            "r"((s32)(s16)team_selection_count),
            "r"((s32)(s16)team_selection_count),
            "r"((s32)(s16)team_selection_count),
            "r"((s32)(s16)team_selection_count));
        initial_next = first_shift + 0x10000;
        {
            register s32 i_guard_r4 asm("r4");
            asm volatile("" : "=r"(i_guard_r4));
            i = (u16)(initial_next >> 16);
            asm volatile("" :: "r"(i_guard_r4));
        }
        outer_shift = (u32)i << 16;
        asm volatile("" : "+r"(outer_shift));
        if ((s32)i < gPlayerZoidSelectionCount) {
          do {
            u8 storage_slot;
            u8 *zoid_record;
            s16 j;
            u32 current_record_offset;
            register u8 *current_record_base asm("r7");
            register u32 retained_shift asm("r10");
            register s32 index_guard_r8 asm("r8");
            register s32 index_guard_r9 asm("r9");

            outer = (s32)outer_shift >> 16;
            asm volatile("" : "=r"(index_guard_r8), "=r"(index_guard_r9));
            storage_slot = gPlayerZoidSelectionSlots[outer];
            current_record_offset = storage_slot * 0x70;
            current_record_base = gPlayerZoidRecords;
            zoid_record = (u8 *)(current_record_offset + (u32)current_record_base);
            asm volatile("" :: "r"(index_guard_r8), "r"(index_guard_r9));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            asm volatile("" :: "r"(zoid_record));
            j = outer - 1;
            asm volatile("" :: "r"((s32)(s16)j));
            retained_shift = (u32)i << 16;

            while (j >= (s16)team_selection_count) {
                register s32 previous_storage_slot asm("r4") = gPlayerZoidSelectionSlots[j];
                u8 *previous_zoid = &gPlayerZoidRecords[previous_storage_slot * 0x70];
                asm volatile("" :: "r"(previous_zoid));

                if ((M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(movement_flags)) & 0x3F) < (M2C_FIELD(previous_zoid, u8 *, PLAYER_ZOID_OFFSET(movement_flags)) & 0x3F) ||
                    ((M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(movement_flags)) & 0x3F) ==
                         (M2C_FIELD(previous_zoid, u8 *, PLAYER_ZOID_OFFSET(movement_flags)) & 0x3F) &&
                     zoid_record[0x37] < previous_zoid[0x37])) {

                } else {
                    break;
                }
                gPlayerZoidSelectionSlots[j + 1] = previous_storage_slot;
                j--;
            }
            gPlayerZoidSelectionSlots[j + 1] = storage_slot;
            next = retained_shift + 0x10000;
            i = (u16)(next >> 16);
            outer_shift = (u16)i;
            outer_shift <<= 16;
            asm volatile("" : "+r"(outer_shift));
          } while ((s32)outer_shift >> 16 < gPlayerZoidSelectionCount);
        }
    }
}
