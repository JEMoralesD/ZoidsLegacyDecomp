#include "player_selection.h"

extern u8 gZoidModelSelectionIds[] asm("D_0203237A");

void BuildUnlockedZoidModelSelection(u8 recipe_filter_mode) asm("func_080B65E4");

void BuildUnlockedZoidModelSelection(u8 recipe_filter_mode) {
    struct ZoidBaseRecordView *const sort_records = (struct ZoidBaseRecordView *)0x087AFCC4;
    u8 *const count = (u8 *)0x02032411;
    const u32 *unlocked_model_flags = (const u32 *)0x02021858;
    s16 index;

    *count = 0;
    index = 1;
    asm volatile("" : "+r"(unlocked_model_flags));
    for (; index <= 151; index++) {
        if ((unlocked_model_flags[index / 32] & (1 << (index % 32))) != 0) {
            struct ZoidShopRecordView *record;

            record = &((struct ZoidShopRecordView *)0x087B1E04)[index];
            asm volatile("" : "+r"(record));
            if (recipe_filter_mode == UNLOCKED_ZOID_RECIPES_ALL ||
                (recipe_filter_mode == UNLOCKED_ZOID_RECIPES_SINGLE_MODEL && record->base_model_or_series_id != 200 &&
                 record->base_model_or_series_id != 201) ||
                (recipe_filter_mode == UNLOCKED_ZOID_RECIPES_SERIES && (u8)(record->base_model_or_series_id + 56) <= 1)) {
                u8 *selected = gZoidModelSelectionIds;
                register u8 *slot asm("r1");
                asm volatile("" : "+r"(selected));
                slot = (u8 *)(u32)*count + (u32)selected;
                asm volatile("" : "+r"(slot));
                *slot = (u8)index;
                (*count)++;
            }
        }
    }

    index = 0;
    if (index < *count) {
        do {
        register u8 *inner_seed asm("r1");
        register u8 *inner_selected asm("r8");
        u8 current;
        struct ZoidBaseRecordView *current_record;
        u16 previous;

        current = gZoidModelSelectionIds[index];
        current_record = &sort_records[current];
        asm volatile("" : "+r"(current_record));
        previous = (u16)(index - 1);
        if ((s16)previous >= 0) {
            inner_seed = gZoidModelSelectionIds;
            asm volatile("" : "+r"(inner_seed));
            inner_selected = inner_seed;
            asm volatile("" : "+r"(inner_selected));
            asm volatile("mov r2, #63\n\tmov r9, r2"
                         :
                         :
                         : "r2", "r9");
        }
        while ((s16)previous >= 0) {
            struct ZoidBaseRecordView *earlier_record;
            register u8 earlier asm("r4");
            s32 signed_previous;
            signed_previous = (s16)previous;
            asm volatile("" : "+r"(signed_previous));
            {
                register u32 earlier_address asm("r0");

                asm volatile("mov r3, r8\n\tadd %0, %1, r3"
                             : "=r"(earlier_address)
                             : "r"(signed_previous)
                             : "r3");
                earlier = *(u8 *)earlier_address;
            }
            earlier_record = &sort_records[earlier];
            asm volatile("" : "+r"(earlier_record));
            {
                register u32 current_flags_or_earlier_sort_key asm("r0") = current_record->movement_flags;
                register u32 earlier_movement_flags asm("r1") = earlier_record->movement_flags;
                register u32 current_key asm("r2");

                asm volatile("mov %0, r9\n\t"
                             "and %0, %1\n\t"
                             "mov %1, r9\n\t"
                             "and %1, %2"
                             : "=r"(current_key), "+r"(current_flags_or_earlier_sort_key)
                             : "r"(earlier_movement_flags));
                if (current_key < current_flags_or_earlier_sort_key ||
                    (current_key == current_flags_or_earlier_sort_key &&
                     current_record->data01 < earlier_record->data01)) {
                    u32 shift_address = signed_previous + 1;
                    shift_address += (u32)inner_selected;
                    *(u8 *)shift_address = earlier;
                    previous = (u16)(signed_previous - 1);
                } else {
                    break;
                }
            }
        }
        {
            u32 final_address = (s16)previous + 1;
            register u8 *final_selected asm("r2") = gZoidModelSelectionIds;
            asm volatile("" : "+r"(final_selected));
            final_address += (u32)final_selected;
            *(u8 *)final_address = current;
        }
        index++;
        } while (index < ({
          register u8 *tail_count asm("r1") = count;
          asm volatile("" : "+r"(tail_count));
          *tail_count;
      }));
    }
}
