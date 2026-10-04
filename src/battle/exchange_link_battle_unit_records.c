#include "m2c_prelude.h"
#include "../game/player_state.h"

void RequestWindowRefresh(void) asm("func_080972C8");
void PrintWindowNumberAt(s32, s32, s32, s32, s32, s32, s32) asm("func_0809844C");
void RunMenuScript(const void *) asm("func_08098BB4");
void BeginLinkSend(void *, s32, const void *) asm("func_0809AEC0");
u8 PollLinkSend(void) asm("func_0809AEF4");
void BeginLinkReceive(void *, s32, const void *) asm("func_0809B00C");
u8 PollLinkReceive(void) asm("func_0809B040");
void RecalculateBattleUnitStats(s32, s32) asm("func_080E8B08");
void ApplyBattlePassiveEquipmentEffects(s32, s32) asm("func_080E90AC");
s32 DivideSigned32(s32, s32) asm("func_080ECD98");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void ExchangeLinkBattleUnitRecords(void) asm("func_080E96E8");

void ExchangeLinkBattleUnitRecords(void)
{
    u8 unit_slot;
    u8 send_complete;
    u8 receive_complete;
    volatile s32 next_unit_slot;

    RunMenuScript((void *)BATTLE_LINK_EXCHANGE_PROGRESS_MENU);
    unit_slot = 0;
    do {
        u32 unit_record_offset = unit_slot * 0x270;
        u8 *battle_side_base = (u8 *)0x02034B4C;
        u32 opponent_side_offset_factor;
        u8 *local_record_bytes;
        register u8 *opponent_unit_bytes asm("r8");
        u8 *opponent_auxiliary_bytes;
        u32 unit_slot_times_two;

        local_record_bytes = (u8 *)(unit_record_offset + (u32)battle_side_base);
        opponent_side_offset_factor = 0x9C;
        opponent_side_offset_factor <<= 5;
        battle_side_base += opponent_side_offset_factor;
        unit_record_offset += (u32)battle_side_base;
        opponent_unit_bytes = (u8 *)unit_record_offset;
        BeginLinkSend(local_record_bytes, sizeof(struct PlayerZoidRecordView), (void *)BATTLE_LINK_ZOID_RECORD_MESSAGE);
        BeginLinkReceive(opponent_unit_bytes, sizeof(struct PlayerZoidRecordView), (void *)BATTLE_LINK_ZOID_RECORD_MESSAGE);
        unit_slot_times_two = unit_slot << 1;
        {
            register s32 next_view asm("r1") = unit_slot + 1;
            next_unit_slot = next_view;
        }
        do {
            send_complete = PollLinkSend();
            receive_complete = PollLinkReceive();
            YieldTaskForUpdates(1);
        } while (send_complete == 0 || receive_complete == 0);
        {
            register s32 unit_slot_times_two_carrier asm("r2") = unit_slot_times_two;
            register s32 transfer_percent asm("r0") = unit_slot_times_two_carrier + unit_slot;
            register s32 progress_scale_or_part_count asm("r1");
            asm volatile("" : "+r"(unit_slot_times_two_carrier));
            transfer_percent += 1;
            progress_scale_or_part_count = 100;
            transfer_percent *= progress_scale_or_part_count;
            progress_scale_or_part_count = BATTLE_UNIT_SLOT_COUNT * BATTLE_LINK_UNIT_RECORD_PART_COUNT;
            transfer_percent = DivideSigned32(transfer_percent, progress_scale_or_part_count);
            PrintWindowNumberAt(transfer_percent, 3, 0, 2, 8, 6, 0);
        }
        RequestWindowRefresh();

        BeginLinkSend(local_record_bytes + 0x70, sizeof(struct PlayerPilotRecordView), (void *)BATTLE_LINK_PILOT_RECORD_MESSAGE);
        BeginLinkReceive(opponent_unit_bytes + 0x70, sizeof(struct PlayerPilotRecordView), (void *)BATTLE_LINK_PILOT_RECORD_MESSAGE);
        local_record_bytes += 0xB0;
        {
            register u8 *opponent_auxiliary_address asm("r0") = (u8 *)0xB0;
            asm volatile("" : "+r"(opponent_auxiliary_address));
            opponent_auxiliary_address += (u32)opponent_unit_bytes;
            opponent_auxiliary_bytes = opponent_auxiliary_address;
        }
        do {
            send_complete = PollLinkSend();
            receive_complete = PollLinkReceive();
            YieldTaskForUpdates(1);
        } while (send_complete == 0 || receive_complete == 0);
        {
            register s32 unit_slot_times_two_carrier asm("r1") = unit_slot_times_two;
            register s32 transfer_percent asm("r0") = unit_slot_times_two_carrier + unit_slot;
            register s32 progress_scale_or_part_count asm("r1");
            transfer_percent += 2;
            progress_scale_or_part_count = 100;
            transfer_percent *= progress_scale_or_part_count;
            progress_scale_or_part_count = BATTLE_UNIT_SLOT_COUNT * BATTLE_LINK_UNIT_RECORD_PART_COUNT;
            transfer_percent = DivideSigned32(transfer_percent, progress_scale_or_part_count);
            PrintWindowNumberAt(transfer_percent, 3, 0, 2, 8, 6, 0);
        }
        RequestWindowRefresh();

        BeginLinkSend(local_record_bytes, sizeof(struct PlayerAuxiliaryPilotRecordView), (void *)BATTLE_LINK_AUXILIARY_RECORD_MESSAGE);
        BeginLinkReceive(opponent_auxiliary_bytes, sizeof(struct PlayerAuxiliaryPilotRecordView), (void *)BATTLE_LINK_AUXILIARY_RECORD_MESSAGE);
        do {
            send_complete = PollLinkSend();
            receive_complete = PollLinkReceive();
            YieldTaskForUpdates(1);
        } while (send_complete == 0 || receive_complete == 0);
        {
            register s32 unit_slot_times_two_carrier asm("r2") = unit_slot_times_two;
            register s32 transfer_percent asm("r0") = unit_slot_times_two_carrier + unit_slot;
            register s32 progress_scale_or_part_count asm("r1");
            asm volatile("" : "+r"(unit_slot_times_two_carrier));
            transfer_percent += 3;
            progress_scale_or_part_count = 100;
            transfer_percent *= progress_scale_or_part_count;
            progress_scale_or_part_count = BATTLE_UNIT_SLOT_COUNT * BATTLE_LINK_UNIT_RECORD_PART_COUNT;
            transfer_percent = DivideSigned32(transfer_percent, progress_scale_or_part_count);
            PrintWindowNumberAt(transfer_percent, 3, 0, 2, 8, 6, 0);
        }
        RequestWindowRefresh();

        {
            register u8 *opponent_model_record asm("r1") = opponent_unit_bytes;
            if (opponent_model_record[0] != 0) {
                ApplyBattlePassiveEquipmentEffects(BATTLE_ENEMY_SIDE, unit_slot);
                RecalculateBattleUnitStats(BATTLE_ENEMY_SIDE, unit_slot);
                *(u32 *)(opponent_unit_bytes + BATTLE_UNIT_OFFSET(experience_reward)) = 0;
                *(u32 *)(opponent_unit_bytes + BATTLE_UNIT_OFFSET(money_reward)) = 0;
            }
        }
        {
            register s32 next_view asm("r2") = next_unit_slot;
            register s32 normalized asm("r0");
            normalized = next_view << 24;
            asm volatile("" : "+r"(normalized));
            unit_slot = (u32)normalized >> 24;
        }
    } while (unit_slot <= BATTLE_UNIT_SLOT_COUNT - 1);

    {
        u8 *party_text_bytes = (u8 *)BATTLE_PARTY_IDENTITY_ADDRESS(pilot_names[BATTLE_PLAYER_SIDE]);
        const void *link_message = (void *)BATTLE_LINK_PARTY_NAME_MESSAGE;
        BeginLinkSend(party_text_bytes, BATTLE_PARTY_NAME_CHARACTER_COUNT * 2, link_message);
        party_text_bytes += 0x12;
        BeginLinkReceive(party_text_bytes, BATTLE_PARTY_NAME_CHARACTER_COUNT * 2, link_message);
        do {
            send_complete = PollLinkSend();
            receive_complete = PollLinkReceive();
            YieldTaskForUpdates(1);
        } while (send_complete == 0 || receive_complete == 0);
    }

    {
        u8 *party_text_bytes = (u8 *)BATTLE_PARTY_IDENTITY_ADDRESS(battle_quotes[BATTLE_PLAYER_SIDE]);
        const void *link_message = (void *)BATTLE_LINK_BATTLE_QUOTE_MESSAGE;
        BeginLinkSend(party_text_bytes, BATTLE_PARTY_QUOTE_CHARACTER_COUNT * 2, link_message);
        party_text_bytes += 0x2E;
        BeginLinkReceive(party_text_bytes, BATTLE_PARTY_QUOTE_CHARACTER_COUNT * 2, link_message);
        do {
            send_complete = PollLinkSend();
            receive_complete = PollLinkReceive();
            YieldTaskForUpdates(1);
        } while (send_complete == 0 || receive_complete == 0);
    }
    RunMenuScript((void *)BATTLE_LINK_EXCHANGE_CLOSE_MENU);
}
