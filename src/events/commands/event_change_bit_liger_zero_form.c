#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"

u8 GetZoidFormId(void *, u8) asm("func_080E5320");
void RecalculateZoidStats(void *, s32) asm("func_080E5880");
void SeekEventCommand(s32, s32, s32) asm("func_080A016C");

s32 EventChangeBitLigerZeroForm(s32 script_slot, struct EventBitLigerFormChangeCommand **script_cursor) asm("func_080A6570");

s32 EventChangeBitLigerZeroForm(s32 script_slot, struct EventBitLigerFormChangeCommand **script_cursor)
{
    struct EventBitLigerFormChangeCommand **saved_script_cursor = script_cursor;
    register u32 saved_script_slot asm("r8");
    register u32 storage_slot asm("r3");
    register struct PlayerZoidRecordView *zoid_records asm("r6");
    register u8 *player_state asm("r5");
    register u8 *pilot_records asm("r9");

    asm volatile("" : : "r"(saved_script_cursor), "r"(script_slot));
    saved_script_slot = (u8)script_slot;
    asm volatile("" : : "r"(saved_script_slot));
    storage_slot = 1;
    zoid_records = (struct PlayerZoidRecordView *)0x020218E8;
    player_state = (u8 *)zoid_records - 4;
    pilot_records = player_state + 0x5A94;

scan_zoid_records:
    {
        register struct PlayerZoidRecordView *zoid_record asm("r4");
        register s32 record_offset asm("r0");
        register u32 pilot_slot_or_address asm("r0");
        register u32 pilot_records_offset asm("r1");
        register u32 pilot_id asm("r0");

        record_offset = storage_slot << 3;
        record_offset -= storage_slot;
        record_offset <<= 4;
        zoid_record = (struct PlayerZoidRecordView *)(record_offset + (s32)zoid_records);
        asm volatile("" : : "r"(zoid_record));
        pilot_slot_or_address = zoid_record->pilot_slot;
        asm volatile("" : "+r"(pilot_slot_or_address));
        pilot_slot_or_address <<= 6;
        pilot_slot_or_address += (u32)player_state;
        pilot_records_offset = 0x5A94;
        pilot_slot_or_address += pilot_records_offset;
        pilot_id = *(u8 *)pilot_slot_or_address;
        if ((pilot_id == BIT_PILOT_ID || pilot_id == BIT_ALTERNATE_PILOT_ID) &&
            (u8)(zoid_record->model_id - 0x19) <= 5) {
            register struct EventBitLigerFormChangeCommand *command asm("r2") = *saved_script_cursor;
            register u32 unlocked_forms asm("r1") = zoid_record->form_flags;
            register u32 form_index_or_unlock_mask asm("r0") = command->form_index_or_unlock_mask;
            asm volatile("" : "+r"(unlocked_forms));
            asm volatile("" : "+r"(form_index_or_unlock_mask));
            form_index_or_unlock_mask &= unlocked_forms;
            if (form_index_or_unlock_mask != 0) {
                register u32 pilot_record_address asm("r1");
                zoid_record->model_id = GetZoidFormId(zoid_record, command->form_index_or_unlock_mask);
                asm volatile("" : : "r"(saved_script_cursor));
                pilot_record_address = zoid_record->pilot_slot;
                asm volatile("" : "+r"(pilot_record_address));
                pilot_record_address <<= 6;
                pilot_record_address += (u32)pilot_records;
                RecalculateZoidStats(zoid_record, pilot_record_address);
                goto advance_event;
            }
        }
    }

    {
        register u32 next_index asm("r0");
        next_index = storage_slot + 1;
        storage_slot = (u8)next_index;
    }
    if (storage_slot <= 0xCE) {
        goto scan_zoid_records;
    }

advance_event:
    SeekEventCommand(saved_script_slot, -1, 0);
    return 0;
}
