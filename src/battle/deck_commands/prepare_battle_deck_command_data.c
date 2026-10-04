#include "m2c_prelude.h"
#include "deck_commands.h"
#include "../combinations/battle_combination.h"
#include "../../graphics/screen_effects.h"

#define NULL ((void *)0)

M2C_UNK StopTask(s32) asm("func_08092E0C");
M2C_UNK InitializePerspectiveScanlineBuffers(void *, s32) asm("func_08093AE8");
M2C_UNK StopPerspectiveScanlineCallback(void) asm("func_08093B54");
M2C_UNK DisableDisplayWindows(void) asm("func_0809534C");
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
M2C_UNK RunMenuScript(u32) asm("func_08098BB4");
u8 SelectBattleReplacementPlayerZoid(void) asm("func_080B2EF0");
M2C_UNK RunBattleTeamFormationMenu(void) asm("func_080B484C");
M2C_UNK UpdateBattleUnitGaugeGraphics(void) asm("func_080BAB3C");
M2C_UNK ConfigureBattleUnitSprites(u8, u8, u8, u32, u32, s32, s32) asm("func_080BAF2C");
M2C_UNK InitializeBattleFieldDisplay(void) asm("func_080BB940");
u8 SelectActiveBattleUnit(s32, s32) asm("func_080C05A8");
u8 SelectBattleDeckCommandToAdd(void) asm("func_080C2518");
M2C_UNK RecalculateZoidStats(void *, void *) asm("func_080E5880");
void *AcquireEquipmentStatBuffer(void) asm("func_080E669C");
M2C_UNK ReleaseEquipmentStatBuffer(void) asm("func_080E66B8");
s32 BuildBattleEquipmentStats(u32, u32, u8, s32, void *) asm("func_080E8C90");
s32 IsBattleUnitActive(u32, u32) asm("func_080E9D88");
u32 CallFunctionR0(s32) asm("func_080ECD5C");
s16 DivideSigned32(s32, s32) asm("func_080ECD98");
M2C_UNK CopyBytes(void *, void *, s32) asm("func_080ED038");
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");

extern u8 gBattleState[];
asm(".set D_case27_selected_offset, 0xA084");
extern u8 gDeckCommandSelectedUnitOffset[] asm("D_case27_selected_offset");
asm(".set D_0203EBD0, gBattleState + 41092");
extern u8 gBattleDeckCommandPayloads[] asm("D_0203EBD0");
extern u8 gPlayerZoidStorageBase[] asm("D_020218E4");
extern u8 gPlayerZoidRecords[] asm("D_020218E8");
extern void *gBattleUnitGaugeSprites[2][6] asm("D_02032EBC");

s32 PrepareBattleDeckCommandData(u8 command_side) asm("func_080C25DC");

s32 PrepareBattleDeckCommandData(u8 command_side)
{
    u8 remaining_turn_entries[BATTLE_TURN_ORDER_ENTRY_COUNT];
    u32 command_side_carrier;
    u32 payload_side_offset;
    u8 *replacement_selected_slot;
    u8 *replacement_combination_owner;
    u8 *display_unit_record;
    u8 *replacement_battle_unit;
    u8 *player_zoid_or_deck_list;
    u8 *unused_cleanup_slots;
    u8 *cleanup_slot;
    u8 *action_record_base;
    register void *equipment_stats asm("r4");
    void *pilot_record;
    register u32 display_side asm("r6");
    register u32 display_unit_slot asm("r5");
    register u8 *redistribution_base asm("r7");
    register u32 redistribution_side_offset asm("r4");
    register u32 redistribution_side_next asm("r8");
    u32 redistribution_slot_next;
    register u32 redistribution_unit_address asm("r2");
    register u32 replacement_unit_slot_or_original_party_slot asm("r2");
    register u32 target_unit_slot asm("r7");
    register u32 support_unit_slot asm("r7");
    register u32 payload_store_address asm("r0");
    register u32 payload_store_offset asm("r1");
    register u32 payload_store_value asm("r7");
    u32 target_menu_script;
    u32 support_menu_script;
    u8 unused_picked_slot;
    register u32 added_command_id asm("r7");
    register u32 replaced_strategy_command_address asm("r1");
    u8 replacement_stored_zoid_slot;
    register u32 replacement_original_party_slot asm("r4");
    u8 replacement_original_stored_zoid_slot;
    u32 replacement_side;
    u32 replacement_slot;
    register u32 replacement_zero asm("r9");
    register u32 replacement_side_next asm("r8");
    register u32 replacement_side_offset asm("sl");
    u8 entry_index;
    register u32 remaining_entry_count asm("r4");
    u32 confusion_remaining_next;
    u8 unused_unit_count;
    s16 unused_total_power;
    s16 unused_maximum_power;
    s32 aegis_active_unit_count;
    u32 aegis_count_saved;
    register u32 aegis_count_observed asm("r2");
    s16 aegis_total_weapon_power;
    s16 aegis_unit_maximum_power;
    register u32 aegis_slot asm("r6");
    u8 aegis_equipment_slot;
    u16 *aegis_mean_weapon_power;
    register u32 aegis_slot_next asm("r8");
    u32 aegis_slot_narrowed;
    u32 column_swap_random;
    register u32 confusion_random_entry_index asm("r1");
    register u32 confusion_payload_side_offset asm("r7");
    register u32 confusion_selected_entry_index asm("r5");
    register u8 *confusion_table asm("r9");
    register u32 confusion_output_next asm("r8");
    u8 consumed_command_id;

    command_side_carrier = command_side;
    action_record_base = gBattleState;
    action_record_base += command_side_carrier * (s32)sizeof(struct BattleDeckCommandActionView);
    if (action_record_base[BATTLE_DECK_PREPARATION_OFFSET(actions[0].kind)] != DECK_COMMAND_ACTION_KIND)
        goto finish_preparation;

    switch (action_record_base[BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id)] - 2) {
    case DECK_COMMAND_CONSERVATION - 2:
        target_menu_script = DECK_COMMAND_CONSERVATION_TARGET_SCRIPT;
select_target_unit:
        RunMenuScript(target_menu_script);
        target_unit_slot = SelectActiveBattleUnit(0, 0);
        if (target_unit_slot != BATTLE_UNIT_NOT_SELECTED)
            goto store_target_unit;
        goto cancel_preparation;

    case DECK_COMMAND_CHARGE_ENERGY - 2:
        support_menu_script = DECK_COMMAND_CHARGE_ENERGY_TARGET_SCRIPT;
select_support_unit:
        RunMenuScript(support_menu_script);
        support_unit_slot = SelectActiveBattleUnit(0, 0);
        if (support_unit_slot != BATTLE_UNIT_NOT_SELECTED)
            goto store_support_unit;
        goto cancel_preparation;

    case DECK_COMMAND_FALSE_NEGO - 2:
        target_menu_script = DECK_COMMAND_FALSE_NEGO_TARGET_SCRIPT;
        goto select_target_unit;

    case DECK_COMMAND_LINK_SUPPORT - 2:
    case DECK_COMMAND_AIRRAID - 2:
        support_menu_script = DECK_COMMAND_SUPPORT_TARGET_SCRIPT;
        goto select_support_unit;

    case DECK_COMMAND_STRATEGY_MEET - 2:
        added_command_id = SelectBattleDeckCommandToAdd();
        if (added_command_id == 0)
            goto cancel_preparation;
        entry_index = 0;
        if (gBattleState[BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands)] == DECK_COMMAND_STRATEGY_MEET) {
            gBattleState[BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands)] = added_command_id;
            goto finish_preparation;
        }
strategy_meet_loop:
        entry_index = (u8)(entry_index + 1);
        if (entry_index > PLAYER_SELECTED_DECK_COMMAND_COUNT - 1)
            goto finish_preparation;
        replaced_strategy_command_address = BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands);
        asm volatile(
            "add r0, %1, %0\n\t"
            "add %0, %2, r0"
            : "+r"(replaced_strategy_command_address)
            : "r"(gBattleState), "r"(entry_index)
            : "r0");
        if (*(u8 *)replaced_strategy_command_address != DECK_COMMAND_STRATEGY_MEET)
            goto strategy_meet_loop;
        *(u8 *)replaced_strategy_command_address = added_command_id;
        goto finish_preparation;

    case DECK_COMMAND_DISTURBED_DATA - 2:
        {
            u32 disturbed_data_random;
            u32 disturbed_data_offset;
            u8 *disturbed_data_row;

            disturbed_data_random = CallFunctionR0(*(s32 *)0x03000010);
            disturbed_data_row = gBattleState;
            disturbed_data_offset = (s32)sizeof(union BattleDeckCommandPayload);
            disturbed_data_offset *= command_side_carrier;
            disturbed_data_offset += (u32)disturbed_data_row;
            disturbed_data_row = (u8 *)disturbed_data_offset;
            disturbed_data_random >>= 7;
            disturbed_data_row[BATTLE_DECK_PREPARATION_OFFSET(payloads[0].random_byte)] = (u8)disturbed_data_random;
            goto finish_preparation;
        }

    case DECK_COMMAND_SWITCH - 2:
        payload_side_offset = command_side_carrier * (s32)sizeof(union BattleDeckCommandPayload);
        {
            register u32 replacement_selected_base asm("r3");
            register u32 replacement_selected_address asm("r0");
            register u32 replacement_selected_offset asm("r5");

            replacement_selected_base = (u32)gBattleState;
            asm volatile(
                "add %0, %1, %2"
                : "=r"(replacement_selected_address)
                : "r"(payload_side_offset), "r"(replacement_selected_base));
            replacement_selected_offset = (u32)gDeckCommandSelectedUnitOffset;
            asm volatile(
                "add %0, %1, %0"
                : "+r"(replacement_selected_offset)
                : "r"(replacement_selected_address));
            replacement_selected_slot = (u8 *)replacement_selected_offset;
        }
replacement_select:
        RunMenuScript(DECK_COMMAND_SWITCH_TARGET_SCRIPT);
        target_unit_slot = SelectActiveBattleUnit(0, 0);
        if (target_unit_slot == BATTLE_UNIT_NOT_SELECTED)
            goto cancel_preparation;
        {
            register u8 *replacement_selected_store asm("r0");

            replacement_selected_store = replacement_selected_slot;
            asm volatile("" : "+r"(replacement_selected_store));
            *replacement_selected_store = target_unit_slot;
        }
        StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 8);
        while ((IsScreenTransitionComplete() << 24) == 0) {
            YieldTaskForUpdates(1);
        }
        StopTask(7);
        StopPerspectiveScanlineCallback();
        *(u8 *)0x03000074 = 0;
        DisableDisplayWindows();
        *(u8 *)0x03000075 = 1;
        *(u8 *)0x0300603D = 1;
        replacement_stored_zoid_slot = SelectBattleReplacementPlayerZoid();
        asm volatile("" ::
                     "r"(replacement_stored_zoid_slot), "r"(replacement_stored_zoid_slot),
                     "r"(replacement_stored_zoid_slot), "r"(replacement_stored_zoid_slot),
                     "r"(replacement_stored_zoid_slot), "r"(replacement_stored_zoid_slot));
        asm volatile("" ::
                     "r"(replacement_stored_zoid_slot), "r"(replacement_stored_zoid_slot),
                     "r"(replacement_stored_zoid_slot), "r"(replacement_stored_zoid_slot),
                     "r"(replacement_stored_zoid_slot));
        InitializeBattleFieldDisplay();
        replacement_side = 0;
        asm volatile(
            "mov %0, %1"
            : "=r"(replacement_zero)
            : "r"(replacement_side));
        do {
            replacement_slot = 0;
            asm volatile(
                "add r1, %2, #1\n\t"
                "mov %0, r1\n\t"
                "lsl r0, %2, #2\n\t"
                "add r0, r0, %2\n\t"
                "lsl r0, r0, #3\n\t"
                "sub r0, r0, %2\n\t"
                "lsl r0, r0, #7\n\t"
                "mov %1, r0"
                : "=r"(replacement_side_next),
                  "=r"(replacement_side_offset)
                : "r"(replacement_side)
                : "r0", "r1");
            do {
                if ((IsBattleUnitActive(replacement_side, replacement_slot) << 24) != 0) {
                    u32 replacement_object_offset;
                    register u32 replacement_loop_unit_address asm("r2");
                    register u32 replacement_loop_unit_base asm("r3");

                    replacement_object_offset = replacement_slot << 2;
                    replacement_loop_unit_address =
                        replacement_object_offset + replacement_slot;
                    replacement_loop_unit_address <<= 3;
                    replacement_loop_unit_address -= replacement_slot;
                    replacement_loop_unit_address <<= 4;
                    replacement_loop_unit_address += replacement_side_offset;
                    replacement_loop_unit_base = (u32)gBattleState;
                    replacement_loop_unit_address += replacement_loop_unit_base;
                    display_unit_record = (u8 *)replacement_loop_unit_address;
                    ConfigureBattleUnitSprites(display_unit_record[BATTLE_UNIT_OFFSET(zoid_id)], display_unit_record[BATTLE_UNIT_OFFSET(palette_variant)], display_unit_record[BATTLE_UNIT_OFFSET(size_class)],
                                  replacement_side, replacement_slot,
                                  replacement_zero, replacement_zero);
                    asm volatile("" : "+r"(replacement_side));
                    replacement_object_offset += replacement_side * 0x18;
                    replacement_object_offset += (u32)gBattleUnitGaugeSprites;
                    *(u32 *)((u8 *)*(void **)replacement_object_offset +
                             BATTLE_SPRITE_OFFSET(user_data.gauge.override_flags)) = replacement_zero;
                }
                replacement_slot = (u8)(replacement_slot + 1);
            } while (replacement_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            replacement_side = (u8)replacement_side_next;
        } while (replacement_side <= 1);
        UpdateBattleUnitGaugeGraphics();
        InitializePerspectiveScanlineBuffers((void *)0x0202F094, 1);
        {
            register u8 *replacement_flag asm("r0");
            register u32 replacement_flag_value asm("r1");
            register u32 replacement_flag_bit asm("r2");

            replacement_flag = (u8 *)0x03000074;
            replacement_flag_value = *replacement_flag;
            replacement_flag_bit = 1;
            replacement_flag_value |= replacement_flag_bit;
            *replacement_flag = (u8)replacement_flag_value;
        }
        RunMenuScript(DECK_COMMAND_RESTORE_BATTLE_WINDOWS_SCRIPT);
        StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 8);
        if (replacement_stored_zoid_slot == BATTLE_UNIT_NOT_SELECTED)
            goto replacement_select;

        {
            register u8 *replacement_copy_side_base asm("r4");
            u32 replacement_unit_id_offset;
            u32 replacement_unit_slot_offset;

            {
                register u8 *replacement_pre_call_base asm("r3") = gBattleState;

                asm volatile(
                    "add %0, %1, %2"
                    : "=r"(replacement_copy_side_base)
                    : "r"(payload_side_offset), "r"(replacement_pre_call_base));
            CopyBytes(replacement_copy_side_base + BATTLE_DECK_PREPARATION_OFFSET(payloads[0].replacement.incoming_zoid),
                          ({
                              replacement_slot = (u32)gPlayerZoidStorageBase;
                              (u8 *)replacement_slot +
                                  replacement_stored_zoid_slot * (s32)sizeof(struct PlayerZoidRecordView) + 4;
                          }),
                          ({
                              register u32 replacement_copy_size asm("r2") = (s32)sizeof(struct PlayerZoidRecordView);
                              asm volatile("str r3, [sp, #64]"
                                           : "+r"(replacement_copy_size)
                                           :
                                           : "memory");
                              replacement_copy_size;
                          }));
            }
            replacement_unit_id_offset = command_side_carrier * (s32)sizeof(struct BattleSide);
            {
                register u32 replacement_selected_offset asm("r0") = BATTLE_DECK_PREPARATION_OFFSET(payloads[0].selected_unit_slot);

                asm volatile(
                    "add %0, %0, %1"
                    : "+r"(replacement_copy_side_base)
                    : "r"(replacement_selected_offset));
                replacement_unit_slot_or_original_party_slot = *replacement_copy_side_base;
            }
            replacement_unit_slot_offset = replacement_unit_slot_or_original_party_slot * (s32)sizeof(struct BattleUnit);
            {
                register u8 *replacement_data_base asm("r3");

                asm volatile("ldr %0, [sp, #64]"
                             : "=r"(replacement_data_base)
                             :
                             : "memory");
                asm volatile(
                    "add %0, %0, %1"
                    : "+r"(replacement_unit_slot_offset)
                    : "r"(replacement_data_base));
                replacement_battle_unit = (u8 *)replacement_unit_id_offset +
                                       replacement_unit_slot_offset;
        asm volatile(
            "mov r1, #156\n\t"
            "lsl r1, r1, #6\n\t"
            "add r0, %2, r1\n\t"
            "add %1, %1, r0\n\t"
            "ldrb %0, [%1]"
            : "=r"(replacement_original_party_slot),
              "+r"(replacement_unit_slot_or_original_party_slot)
            : "r"(replacement_data_base)
            : "r0", "r1");
        {
            register u32 replacement_link_offset asm("r2") = BATTLE_COMBINATION_OFFSET(combination_owner_original_party_slots);

            asm volatile(
                "add r0, %1, %2\n\t"
                "add %0, %3, r0"
                : "=r"(replacement_combination_owner)
                : "r"(replacement_data_base),
                  "r"(replacement_link_offset),
                  "r"(replacement_original_party_slot)
                : "r0");
        }
        if (*replacement_combination_owner == BATTLE_COMBINATION_NO_OWNER) {
            u32 replacement_record_address;
            u32 replacement_record_offset;

            replacement_record_address = (u32)replacement_data_base + BATTLE_DECK_PREPARATION_OFFSET(original_party_stored_zoid_slots);
            asm volatile(""
                         : "+&r"(replacement_record_address)
                         : "r"(replacement_data_base));
            asm volatile(
                "add %0, %1, %0"
                : "+r"(replacement_record_address)
                : "r"(replacement_original_party_slot));
            replacement_original_stored_zoid_slot = *(u8 *)replacement_record_address;
            replacement_record_offset = replacement_original_stored_zoid_slot * (s32)sizeof(struct PlayerZoidRecordView);
            asm volatile(
                "add r1, %2, #4\n\t"
                "add %0, %1, r1"
                : "=r"(player_zoid_or_deck_list)
                : "r"(replacement_record_offset),
                  "r"((u8 *)replacement_slot)
                : "r1");
            if (player_zoid_or_deck_list[0] != replacement_battle_unit[0])
                player_zoid_or_deck_list[0] = replacement_battle_unit[0];
            if (*(s16 *)(replacement_battle_unit + BATTLE_UNIT_OFFSET(hp)) <=
                *(s16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(max_hp)))
                *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(current_hp)) =
                    *(u16 *)(replacement_battle_unit + BATTLE_UNIT_OFFSET(hp));
            else
                *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(current_hp)) = *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(max_hp));
            if (*(s16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(level)) < DECK_COMMAND_REPLACEMENT_LEVEL_LIMIT)
                *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(level)) = *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(level)) + 1;
            if (player_zoid_or_deck_list[PLAYER_ZOID_OFFSET(pilot_slot)] != 0) {
                u32 replacement_effect_offset;

                replacement_effect_offset = player_zoid_or_deck_list[PLAYER_ZOID_OFFSET(pilot_slot)] << 6;
                pilot_record = (void *)0x02027378;
                asm volatile(
                    "add %0, %1, %0"
                    : "+r"(pilot_record)
                    : "r"(replacement_effect_offset));
            } else
                pilot_record = NULL;
            RecalculateZoidStats(player_zoid_or_deck_list, pilot_record);
        } else {
            register u8 *replacement_index_table asm("r9");
            register u32 replacement_scan_address asm("r0");
            u32 replacement_table_address;

            replacement_slot = *replacement_combination_owner;
            entry_index = 0;
            replacement_index_table = replacement_data_base + BATTLE_DECK_PREPARATION_OFFSET(original_party_stored_zoid_slots);
            do {
                replacement_scan_address =
                    (u32)replacement_data_base + BATTLE_COMBINATION_OFFSET(combination_owner_original_party_slots);
                replacement_scan_address = entry_index + replacement_scan_address;
                if (*(u8 *)replacement_scan_address == replacement_slot) {
                    u8 *replacement_else_record_base;
                    u32 replacement_else_record_offset;

                    replacement_else_record_offset =
                        *(u8 *)((u32)entry_index +
                                (u32)replacement_index_table) * (s32)sizeof(struct PlayerZoidRecordView);
                    replacement_else_record_base = gPlayerZoidRecords;
                    player_zoid_or_deck_list = replacement_else_record_base +
                             replacement_else_record_offset;
                    {
                        register s32 restored_component_hp asm("sl");
                        register s32 combined_hp_or_maximum_hp asm("ip");

                        restored_component_hp = *(s16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(max_hp));
                        combined_hp_or_maximum_hp = *(s16 *)(replacement_battle_unit + BATTLE_UNIT_OFFSET(hp));
                        restored_component_hp = combined_hp_or_maximum_hp * restored_component_hp;
                        combined_hp_or_maximum_hp = *(s16 *)(replacement_battle_unit + BATTLE_UNIT_OFFSET(max_hp));
                        *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(current_hp)) =
                            DivideSigned32(restored_component_hp, combined_hp_or_maximum_hp);
                    }
                    {
                        register u32 stored_zoid_level asm("sl");

                        stored_zoid_level = *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(level));
                        asm volatile("" : "+r"(stored_zoid_level) : : "memory");
                        if (*(s16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(level)) < DECK_COMMAND_REPLACEMENT_LEVEL_LIMIT)
                            *(u16 *)(player_zoid_or_deck_list + PLAYER_ZOID_OFFSET(level)) = stored_zoid_level + 1;
                    }
                    if (player_zoid_or_deck_list[PLAYER_ZOID_OFFSET(pilot_slot)] != 0) {
                        u32 replacement_effect_offset;
                        register u32 replacement_effect_base asm("r2");

                        replacement_effect_offset = player_zoid_or_deck_list[PLAYER_ZOID_OFFSET(pilot_slot)] << 6;
                        replacement_effect_base =
                            (u32)replacement_else_record_base;
                        replacement_effect_base += 0x5A90;
                        pilot_record = (u8 *)(replacement_effect_offset +
                                        replacement_effect_base);
                    } else
                        pilot_record = NULL;
                    RecalculateZoidStats(player_zoid_or_deck_list, pilot_record);
                    replacement_scan_address =
                        (u32)replacement_data_base + BATTLE_COMBINATION_OFFSET(combination_owner_original_party_slots);
                    replacement_scan_address = entry_index + replacement_scan_address;
                    *(u8 *)replacement_scan_address = BATTLE_COMBINATION_NO_OWNER;
                    asm volatile(
                        "mov r0, %1\n\t"
                        "add %0, %2, r0"
                        : "=r"(replacement_table_address)
                        : "r"(replacement_index_table),
                          "r"(entry_index)
                        : "r0");
                    *(u8 *)replacement_table_address = 0;
                    }
                    entry_index = (u8)(entry_index + 1);
                } while (entry_index <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        }
            }
        }
        {
            register u32 payload_store_base asm("r2");
            register u32 payload_store_term asm("r3");
            register u32 payload_store_selected asm("r5");

            payload_store_base = (u32)gBattleState;
            payload_store_term = BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots);
            payload_store_offset = payload_store_base + payload_store_term;
            payload_store_selected = *replacement_selected_slot;
            payload_store_offset += payload_store_selected;
            payload_store_term = BATTLE_DECK_PREPARATION_OFFSET(original_party_stored_zoid_slots);
            payload_store_address = payload_store_base + payload_store_term;
            payload_store_offset = *(u8 *)payload_store_offset;
            payload_store_value = replacement_stored_zoid_slot;
            goto store_payload_byte;
        }

    case DECK_COMMAND_REDISTRIBUTION - 2:
        StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 8);
        while ((IsScreenTransitionComplete() << 24) == 0) {
            YieldTaskForUpdates(1);
        }
        StopTask(7);
        StopPerspectiveScanlineCallback();
        *(u8 *)0x03000074 = 0;
        DisableDisplayWindows();
        *(u8 *)0x03000075 = 1;
        *(u8 *)0x0300603D = 1;
        RunBattleTeamFormationMenu();
        InitializeBattleFieldDisplay();
        display_side = 0;
        redistribution_base = gBattleState;
        do {
            display_unit_slot = 0;
            redistribution_side_next = display_side + 1;
            asm volatile(
                "lsl r0, %1, #2\n\t"
                "add r0, r0, %1\n\t"
                "lsl r0, r0, #3\n\t"
                "sub r0, r0, %1\n\t"
                "lsl %0, r0, #7"
                : "=r"(redistribution_side_offset)
                : "r"(display_side)
                : "r0");
            do {
                if ((IsBattleUnitActive(display_side, display_unit_slot) << 24) != 0) {
                    redistribution_unit_address = display_unit_slot * (s32)sizeof(struct BattleUnit);
                    redistribution_unit_address += redistribution_side_offset;
                    asm volatile("add %0, %1"
                                 : "+r"(redistribution_unit_address)
                                 : "r"(redistribution_base));
                    ConfigureBattleUnitSprites(((u8 *)redistribution_unit_address)[BATTLE_UNIT_OFFSET(zoid_id)],
                                  ((u8 *)redistribution_unit_address)[BATTLE_UNIT_OFFSET(palette_variant)],
                                  ((u8 *)redistribution_unit_address)[BATTLE_UNIT_OFFSET(size_class)],
                                  display_side, display_unit_slot, 0, 0);
                }
                redistribution_slot_next = display_unit_slot + 1;
                asm volatile(
                    "lsl %1, %1, #24\n\t"
                    "lsr %0, %1, #24"
                    : "=r"(display_unit_slot), "+r"(redistribution_slot_next));
            } while (display_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            asm volatile(
                "mov r1, %1\n\t"
                "lsl r0, r1, #24\n\t"
                "lsr %0, r0, #24"
                : "=r"(display_side)
                : "r"(redistribution_side_next)
                : "r0", "r1");
        } while (display_side <= 1);
        InitializePerspectiveScanlineBuffers((void *)0x0202F094, 1);
        *(u8 *)0x03000074 |= 1;
        RunMenuScript(DECK_COMMAND_RESTORE_BATTLE_WINDOWS_SCRIPT);
        StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 8);
        goto finish_preparation;

    case DECK_COMMAND_PARTS_REMOVAL - 2:
        RunMenuScript(DECK_COMMAND_PARTS_REMOVAL_TARGET_SCRIPT);
        support_unit_slot = SelectActiveBattleUnit(0, 0);
        if (support_unit_slot != BATTLE_UNIT_NOT_SELECTED)
            goto store_support_unit;
        goto cancel_preparation;
store_support_unit:
        {
            register u32 support_address asm("r0");
            register u32 support_id asm("r2");
            register u8 *support_base asm("r1");
            register u32 support_tail asm("r3");

            support_base = gBattleState;
            support_address = (s32)sizeof(union BattleDeckCommandPayload);
            support_id = command_side_carrier;
            asm volatile(
                "mul %0, %1\n\t"
                "add %0, %0, %2"
                : "+r"(support_address)
                : "r"(support_id), "r"(support_base));
            support_tail = BATTLE_DECK_PREPARATION_OFFSET(payloads[0].selected_unit_slot);
            asm volatile("add %0, %1"
                         : "+r"(support_address)
                         : "r"(support_tail));
            *(u8 *)support_address = support_unit_slot;
            goto finish_preparation;
        }

    case DECK_COMMAND_DECOY - 2:
        RunMenuScript(DECK_COMMAND_DECOY_TARGET_SCRIPT);
        target_unit_slot = SelectActiveBattleUnit(0, 0);
        if (target_unit_slot != BATTLE_UNIT_NOT_SELECTED)
            goto store_target_unit;
        goto cancel_preparation;
store_target_unit:
        {
            register u32 target_id asm("r5");

            payload_store_offset = (u32)gBattleState;
            payload_store_address = (s32)sizeof(union BattleDeckCommandPayload);
            target_id = command_side_carrier;
            asm volatile(
                "mul %0, %1\n\t"
                "add %0, %0, %2"
                : "+r"(payload_store_address)
                : "r"(target_id), "r"(payload_store_offset));
            payload_store_offset = BATTLE_DECK_PREPARATION_OFFSET(payloads[0].selected_unit_slot);
            payload_store_value = target_unit_slot;
            goto store_payload_byte;
        }

store_payload_byte:
        payload_store_address += payload_store_offset;
        *(u8 *)payload_store_address = payload_store_value;
        goto finish_preparation;

    case DECK_COMMAND_GRAVITY_STORM - 2:
        {
            register u32 gravity_storm_mapped_side asm("r6");
            u8 gravity_storm_column;
            u32 gravity_storm_payload_side_offset;
            register u32 gravity_storm_second_seed asm("r2");
            register u32 gravity_storm_second_offset asm("r9");
            register u32 gravity_storm_mapped_side_offset asm("r4");
            register u32 gravity_storm_next_mapped_side asm("r8");
            u32 gravity_storm_next_mapped_side_shifted;
            u8 *gravity_storm_table;

            gravity_storm_mapped_side = 0;
            gravity_storm_payload_side_offset = command_side_carrier * (s32)sizeof(union BattleDeckCommandPayload);
            gravity_storm_table = gBattleDeckCommandPayloads;
            gravity_storm_second_seed = gravity_storm_payload_side_offset + 3;
            gravity_storm_second_offset = gravity_storm_second_seed;
            asm volatile("" : "+r"(gravity_storm_second_seed));
            do {
                gravity_storm_column = 0;
                asm volatile(
                    "add r0, %1, #1\n\t"
                    "mov %0, r0"
                    : "=r"(gravity_storm_next_mapped_side)
                    : "r"(gravity_storm_mapped_side)
                    : "r0");
                asm volatile(
                    "lsl r0, %1, #1\n\t"
                    "add r0, r0, %1\n\t"
                    "lsl %0, r0, #1"
                    : "=r"(gravity_storm_mapped_side_offset)
                    : "r"(gravity_storm_mapped_side)
                    : "r0");
                do {
                    column_swap_random = CallFunctionR0(*(s32 *)0x03000010) >> 14;
                    if (column_swap_random == 0) {
                        u32 gravity_storm_zero_entry_offset;
                        u32 gravity_storm_zero_first_address;
                        u32 gravity_storm_zero_second_address;

                        gravity_storm_zero_entry_offset =
                            gravity_storm_mapped_side_offset + gravity_storm_column;
                        gravity_storm_zero_first_address = gravity_storm_zero_entry_offset;
                        gravity_storm_zero_first_address += gravity_storm_payload_side_offset;
                        gravity_storm_zero_first_address += (u32)gravity_storm_table;
                        *(u8 *)gravity_storm_zero_first_address = gravity_storm_column;
                        gravity_storm_zero_second_address = gravity_storm_zero_entry_offset;
                        asm volatile("add %0, r9"
                                     : "+r"(gravity_storm_zero_second_address)
                                     : "r"(gravity_storm_second_offset));
                        gravity_storm_zero_second_address += (u32)gravity_storm_table;
                        *(u8 *)gravity_storm_zero_second_address = gravity_storm_column + 3;
                    } else {
                        u32 gravity_storm_nonzero_entry_offset;
                        u32 gravity_storm_nonzero_first_address;
                        u32 gravity_storm_nonzero_second_address;

                        gravity_storm_nonzero_entry_offset =
                            gravity_storm_mapped_side_offset + gravity_storm_column;
                        gravity_storm_nonzero_first_address =
                            gravity_storm_nonzero_entry_offset;
                        gravity_storm_nonzero_first_address += gravity_storm_payload_side_offset;
                        gravity_storm_nonzero_first_address += (u32)gravity_storm_table;
                        *(u8 *)gravity_storm_nonzero_first_address = gravity_storm_column + 3;
                        gravity_storm_nonzero_second_address =
                            gravity_storm_nonzero_entry_offset;
                        asm volatile("add %0, r9"
                                     : "+r"(gravity_storm_nonzero_second_address)
                                     : "r"(gravity_storm_second_offset));
                        gravity_storm_nonzero_second_address += (u32)gravity_storm_table;
                        *(u8 *)gravity_storm_nonzero_second_address = gravity_storm_column;
                    }
                    gravity_storm_column = (u8)(gravity_storm_column + 1);
                } while (gravity_storm_column <= 2);
                gravity_storm_next_mapped_side_shifted = gravity_storm_next_mapped_side << 24;
                gravity_storm_mapped_side = gravity_storm_next_mapped_side_shifted >> 24;
                asm volatile("" : : "r"(gravity_storm_next_mapped_side_shifted));
            } while (gravity_storm_mapped_side <= 1);
        }
        goto finish_preparation;

    case DECK_COMMAND_CONFUSION - 2:
        entry_index = 0;
        do {
            remaining_turn_entries[entry_index] = entry_index;
            entry_index = (u8)(entry_index + 1);
        } while (entry_index <= BATTLE_TURN_ORDER_ENTRY_COUNT - 1);
        remaining_entry_count = BATTLE_TURN_ORDER_ENTRY_COUNT;
        entry_index = 0;
        confusion_payload_side_offset = command_side_carrier * (s32)sizeof(union BattleDeckCommandPayload);
        confusion_table = gBattleDeckCommandPayloads;
        do {
            u8 *confusion_output;

            confusion_random_entry_index = CallFunctionR0(*(s32 *)0x03000010);
            confusion_random_entry_index = (confusion_random_entry_index * remaining_entry_count) >> 15;
            confusion_random_entry_index = (u8)confusion_random_entry_index;
            confusion_output = (u8 *)(entry_index + confusion_payload_side_offset);
            asm volatile("add %0, %1"
                         : "+r"(confusion_output)
                         : "r"(confusion_table));
            *confusion_output = remaining_turn_entries[confusion_random_entry_index];
            confusion_remaining_next = remaining_entry_count - 1;
            asm volatile(
                "lsl %1, %1, #24\n\t"
                "lsr %0, %1, #24"
                : "=r"(remaining_entry_count), "+r"(confusion_remaining_next));
            confusion_selected_entry_index = (u8)confusion_random_entry_index;
            confusion_output_next = entry_index + 1;
            while (confusion_selected_entry_index < remaining_entry_count) {
                asm volatile(
                    "mov r2, sp\n\t"
                    "add r2, r2, %0\n\t"
                    "add r2, #12\n\t"
                    "add r1, %0, #1\n\t"
                    "mov r0, sp\n\t"
                    "add r0, r0, r1\n\t"
                    "add r0, #12\n\t"
                    "ldrb r0, [r0]\n\t"
                    "strb r0, [r2]\n\t"
                    "lsl r1, r1, #24\n\t"
                    "lsr %0, r1, #24"
                    : "+r"(confusion_selected_entry_index)
                    :
                    : "r0", "r1", "r2", "memory");
            }
            entry_index = (u8)confusion_output_next;
        } while (entry_index <= BATTLE_TURN_ORDER_ENTRY_COUNT - 1);
        goto finish_preparation;

cancel_preparation:
        return 0;

    case DECK_COMMAND_AEGIS_PHALANX - 2:
        equipment_stats = AcquireEquipmentStatBuffer();
        aegis_active_unit_count = 0;
        aegis_total_weapon_power = 0;
        aegis_slot = 0;
        do {
            if ((IsBattleUnitActive(command_side_carrier, aegis_slot) << 24) != 0) {
                aegis_unit_maximum_power = 0;
                aegis_equipment_slot = 0;
                aegis_active_unit_count++;
                aegis_count_saved = aegis_active_unit_count;
                do {
                    if (((BuildBattleEquipmentStats(command_side_carrier, aegis_slot, aegis_equipment_slot, 0,
                                        equipment_stats) << 16) != 0) &&
                        ((*(u16 *)((u8 *)equipment_stats + (s32)&((struct EquipmentRecord *)0)->flags) & EQUIPMENT_COMMAND) == 0) &&
                        (aegis_unit_maximum_power <
                         ({
                             register s32 aegis_weapon_power asm("r1");
                             asm volatile(
                                 "mov r2, #10\n\t"
                                 "ldrsh %0, [%1, r2]"
                                 : "=r"(aegis_weapon_power)
                                 : "r"(equipment_stats)
                                 : "r2");
                             aegis_weapon_power;
                         }))) {
                        aegis_unit_maximum_power = *(u16 *)((u8 *)equipment_stats + (s32)&((struct EquipmentRecord *)0)->power_or_value);
                    }
                    aegis_equipment_slot = (u8)(aegis_equipment_slot + 1);
                } while (aegis_equipment_slot <= BATTLE_EQUIPMENT_SLOT_COUNT - 1);
                asm volatile(
                    "mov r3, %0\n\t"
                    "lsl r0, r3, #16\n\t"
                    "asr r0, r0, #16\n\t"
                    "lsl r1, %1, #16\n\t"
                    "asr r1, r1, #16\n\t"
                    "add r0, r0, r1\n\t"
                    "lsl r0, r0, #16\n\t"
                    "lsr r0, r0, #16\n\t"
                    "mov %0, r0"
                    : "+r"(aegis_total_weapon_power)
                    : "r"(aegis_unit_maximum_power)
                    : "r0", "r1", "r3");
                asm volatile(
                    "mov r5, %1\n\t"
                    "lsl r0, r5, #24\n\t"
                    "lsr %0, r0, #24"
                    : "=r"(aegis_count_observed)
                    : "r"(aegis_count_saved)
                    : "r0", "r5");
                aegis_active_unit_count = aegis_count_observed;
            }
            aegis_slot_next = aegis_slot + 1;
            aegis_slot_narrowed = aegis_slot_next << 24;
            aegis_slot = aegis_slot_narrowed >> 24;
            asm volatile("" : : "r"(aegis_slot_narrowed));
        } while (aegis_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        {
            register u32 aegis_result_address asm("r4");
            register u32 aegis_result_factor asm("r0");
            register u32 aegis_result_id asm("r3");

            aegis_result_factor = (s32)sizeof(union BattleDeckCommandPayload);
            aegis_result_id = command_side_carrier;
            asm volatile(
                "mov %0, %1\n\t"
                "mul %0, %2"
                : "=&r"(aegis_result_address)
                : "r"(aegis_result_id), "r"(aegis_result_factor));
            aegis_result_factor = DECK_COMMAND_PAYLOAD_RAM;
            asm volatile("add %0, %1"
                         : "+r"(aegis_result_address)
                         : "r"(aegis_result_factor));
            aegis_mean_weapon_power = (u16 *)aegis_result_address;
        }
        *aegis_mean_weapon_power = DivideSigned32(
            ({
                register s32 aegis_total_power_argument asm("r0");

                asm volatile(
                    "mov r5, %1\n\t"
                    "lsl %0, r5, #16\n\t"
                    "asr %0, %0, #16"
                    : "=r"(aegis_total_power_argument)
                    : "r"(aegis_total_weapon_power)
                    : "r5");
                aegis_total_power_argument;
            }),
            aegis_active_unit_count);
        ReleaseEquipmentStatBuffer();
        goto finish_preparation;

    default:
        goto finish_preparation;
    }

finish_preparation:
    payload_store_address = command_side_carrier;
    asm volatile("" : "+r"(payload_store_address));
    if (payload_store_address == 0) {
        register u32 cleanup_state_offset asm("r2");
        u8 *cleanup_base;
        register u32 cleanup_state_address asm("r0");
        register s32 cleanup_scratch1 asm("r1");
        register s32 cleanup_scratch2 asm("r2");

        entry_index = 0;
        cleanup_base = gBattleState;
        asm volatile("" : "=&r"(cleanup_state_address),
                       "=&r"(cleanup_scratch1),
                       "=&r"(cleanup_scratch2)
                     : "r"(cleanup_base));
        cleanup_scratch1 = BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands);
        asm volatile(
            "add %0, %1, %2"
            : "=r"(player_zoid_or_deck_list)
            : "r"(cleanup_base), "r"(cleanup_scratch1));
        cleanup_state_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
        asm volatile("add %0, %1, %2"
                     : "=&r"(cleanup_state_address)
                     : "r"(cleanup_base), "r"(cleanup_state_offset));
        consumed_command_id = *(u8 *)cleanup_state_address;
        do {
            u32 cleanup_address;

            cleanup_address = entry_index;
            cleanup_address += (u32)player_zoid_or_deck_list;
            cleanup_slot = (u8 *)cleanup_address;
            if (*cleanup_slot == consumed_command_id) {
                *cleanup_slot = 0;
                break;
            }
            entry_index = (u8)(entry_index + 1);
        } while (entry_index <= PLAYER_SELECTED_DECK_COMMAND_COUNT - 1);
    }
    return 1;
}
