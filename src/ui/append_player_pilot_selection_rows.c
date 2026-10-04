#include "player_selection.h"

void CopyBytes(void *, const void *, u32) asm("func_80ED038");
void *GetPilotDisplayName(u8) asm("func_080E7B64");
void AppendString(void *, void *) asm("func_08099F5C");
void AppendWindowTextItem(s32, void *) asm("func_080988C8");

void AppendPlayerPilotSelectionRows(s32 window_id, s32 reject_filters, s32 excluded_pilot_id) asm("func_080AC3B8");

void AppendPlayerPilotSelectionRows(s32 window_id, s32 reject_filters, s32 excluded_pilot_id)
{
    register u32 saved_window_id asm("r10") = (u8)window_id;
    register u32 saved_reject_filters asm("r9") = (u16)reject_filters;
    register u32 saved_excluded_pilot_id asm("r8") = (u8)excluded_pilot_id;
    u8 selection_index = 0;

    if (selection_index < *(u8 *)PLAYER_PILOT_SELECTION_COUNT_RAM) {
        register u8 *text_prefix_bytes = (u8 *)PLAYER_SELECTION_TEXT_BUFFER_RAM;
        register u8 *row_text = text_prefix_bytes + 2;

        do {
            register struct PlayerPilotRecordView *pilot asm("r4");
            register u8 *selection_slot_address asm("r0") = (u8 *)PLAYER_PILOT_SELECTION_SLOTS_RAM;
            register u32 pilot_record_offset asm("r0");

            asm volatile("" : "+r"(selection_slot_address));
            asm volatile("add %0, %1, %0"
                         : "+r"(selection_slot_address)
                         : "r"((u32)selection_index));
            pilot_record_offset = *selection_slot_address;
            pilot_record_offset <<= 6;
            {
                register struct PlayerPilotRecordView *pilot_records asm("r1") =
                    (struct PlayerPilotRecordView *)PLAYER_PILOT_RECORDS_RAM;
                asm volatile("" : "+r"(pilot_records));
                pilot = (struct PlayerPilotRecordView *)(pilot_record_offset + (u32)pilot_records);
            }
            if (pilot->active_pilot_id == saved_excluded_pilot_id) {
                goto rejected_pilot;
            }
            {
                register u32 normal_text_prefix asm("r2") = PLAYER_SELECTION_NORMAL_TEXT_PREFIX;
                if (saved_reject_filters & normal_text_prefix) {
                    if (pilot->flags_and_pilot_id & PLAYER_RECORD_TEMPORARY_PAIR) {
                        goto rejected_pilot;
                    }
                }
                *(u16 *)text_prefix_bytes = normal_text_prefix;
                goto append_pilot_row;
            }
rejected_pilot:
            {
                register u32 rejected_text_prefix asm("r0") = PLAYER_SELECTION_REJECTED_TEXT_PREFIX;
                asm volatile("" : "+r"(rejected_text_prefix));
                *(u16 *)text_prefix_bytes = rejected_text_prefix;
            }
append_pilot_row:
            CopyBytes(row_text, (void *)PLAYER_SELECTION_SPACE_TEXT_ROM, 3);
            AppendString(row_text, GetPilotDisplayName(pilot->active_pilot_id));
            AppendWindowTextItem(saved_window_id, row_text - 2);
            selection_index++;
        } while (selection_index < *(u8 *)PLAYER_PILOT_SELECTION_COUNT_RAM);
    }
}
