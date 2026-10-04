#include "player_selection.h"

extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern struct PlayerZoidRecordView gPlayerZoidRecords[] asm("D_020218E8");
extern s32 gZoidNameTable[];

extern void PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");
extern void PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C");
extern void ClearWindow(s32) asm("func_080986B4");
extern s32 GetPilotDisplayName(u8) asm("func_080E7B64");
extern s16 DivideSigned32(s16, s32) asm("func_080ECD98");

void DrawPartyHitPointList(void) asm("func_080AC0C8");

void DrawPartyHitPointList(void)
{
    register u32 team_slot asm("r8");
    register u8 *player_state_bytes asm("r9");
    register s32 hp_number_column asm("r10");
    register s32 hp_column_value asm("r2");

    ClearWindow(1);
    team_slot = 0;
    player_state_bytes = gPlayerStateBytes;
    hp_column_value = 16;
    asm volatile("" : "+r"(hp_column_value));
    hp_number_column = hp_column_value;
    do {
        register s32 team_slots_offset asm("r1");
        register u8 *team_slots_base asm("r0");
        register u8 *team_slot_address asm("r1");
        register s32 stored_zoid_slot asm("r0");
        register s32 team_slot_copy asm("r3");

        team_slots_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
        team_slots_base = (u8 *)((s32)player_state_bytes + team_slots_offset);
        team_slot_copy = team_slot;
        asm volatile("" : "+r"(team_slot_copy));
        team_slot_address = (u8 *)(team_slot_copy + (s32)team_slots_base);
        stored_zoid_slot = *team_slot_address;

        if (stored_zoid_slot != 0) {
            register s32 zoid_slot_copy asm("r1");
            register s32 zoid_record_offset asm("r0");
            register struct PlayerZoidRecordView *zoid asm("r6");
            register u8 *zoid_records_base asm("r1");
            register u8 *player_state_view asm("r1");
            s32 name_row;
            s32 name_row_signed;
            s32 current_hp;
            s32 low_hp_color;
            s16 max_hp_row;

            zoid_slot_copy = stored_zoid_slot;
            zoid_record_offset = zoid_slot_copy << 3;
            zoid_record_offset -= zoid_slot_copy;
            zoid_record_offset <<= 4;
            zoid_records_base = player_state_bytes + 4;
            zoid = (struct PlayerZoidRecordView *)(zoid_record_offset + (s32)zoid_records_base);
            player_state_view = player_state_bytes;
            asm volatile("" : "+r"(player_state_view));
            if (M2C_FIELD(player_state_view, u8 *, PLAYER_STATE_OFFSET(party_name_display_mode)) == 0) {
                s32 name_row_value;
                s32 name_row_shifted;

                PrintWindowTextAt(gZoidNameTable[zoid->model_id], 0, 1, 0,
                    (name_row_value = team_slot_copy * 2,
                     name_row_shifted = team_slot_copy << 17,
                     name_row_shifted >> 16));
                asm volatile("" : "+r"(name_row_shifted));
                name_row = name_row_value;
                name_row_signed = name_row_shifted >> 16;
            } else {
                register s32 team_row_index asm("r3");
                register u8 *pilot_record_base asm("r0");
                register s32 pilot_records_offset asm("r2");
                s32 name_row_value;
                s32 name_row_shifted;

                pilot_record_base = player_state_bytes + (zoid->pilot_slot << 6);
                pilot_records_offset = PLAYER_STATE_OFFSET(pilots);
                asm volatile("" : "+r"(pilot_records_offset));
                PrintWindowTextAt(GetPilotDisplayName(pilot_record_base[pilot_records_offset]),
                    0, 1, 0,
                    (team_row_index = team_slot,
                     name_row_value = team_row_index * 2,
                     name_row_shifted = team_row_index << 17,
                     name_row_shifted >> 16));
                asm volatile("" : "+r"(name_row_shifted));
                name_row = name_row_value;
                name_row_signed = name_row_shifted >> 16;
            }
            PrintWindowTextAt(PARTY_HP_LABEL_TEXT_ROM, 0, 1, 14, name_row_signed);
            {
                register s32 occupy_r2 asm("r2");
                register s32 occupy_r3 asm("r3");

                asm volatile("" : "=r"(occupy_r2), "=r"(occupy_r3));
                current_hp = M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(current_hp));
                asm volatile("" : : "r"(occupy_r2), "r"(occupy_r3));
            }
            low_hp_color = 0;
            if (M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(current_hp)) < DivideSigned32(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)), 10)) {
                low_hp_color = 1;
            }
            PrintWindowNumberAt(current_hp, 4, low_hp_color, 10, 1, hp_number_column, name_row_signed);
            max_hp_row = name_row + 1;
            PrintWindowTextAt(PARTY_HP_SEPARATOR_TEXT_ROM, 0, 1, 15, max_hp_row);
            PrintWindowNumberAt(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)), 4, 0, 10, 1, hp_number_column, max_hp_row);
        } else {
            register s32 occupy_r1 asm("r1");
            s32 empty_slot_row;

            asm volatile("" : "=r"(occupy_r1));
            empty_slot_row = team_slot << 1;
            asm volatile("" : : "r"(occupy_r1));
            PrintWindowTextAt(PARTY_EMPTY_SLOT_TEXT_ROM, 0, 1, 0, empty_slot_row);
        }
        team_slot = (u8)(team_slot + 1);
    } while (team_slot < PLAYER_TEAM_SLOT_COUNT);
}
