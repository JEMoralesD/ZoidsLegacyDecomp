#include "m2c_prelude.h"
#include "deck_commands/deck_commands.h"
#include "combinations/battle_combination.h"
#define NULL ((void *)0)

M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
void *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(void *) asm("func_08094554");                      /* extern */
M2C_UNK DisableDisplayWindows() asm("func_0809534C");                            /* extern */
M2C_UNK ConfigureDisplayWindows(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_0809538C"); /* extern */
M2C_UNK ResetMenuKeyRepeat() asm("func_08096F3C");                            /* extern */
M2C_UNK RequestWindowRefresh() asm("func_080972C8");                            /* extern */
M2C_UNK PrintWindowTextAt(s32, s32, s32, s32, u32) asm("func_080981F0");     /* extern */
M2C_UNK PrintWindowText(s32, s32, s32) asm("func_08098248");               /* extern */
M2C_UNK PrintWindowNumberAt(s16, s32, s32, s32, s32, s32, u32) asm("func_0809844C"); /* extern */
M2C_UNK PrintWindowNumberAtWide(s32, s32, s32, s32, s32, s32, u32)
    asm("func_0809844C");
M2C_UNK PrintWindowNumber(u32, s32, s32, s32, s32) asm("func_080984C4");     /* extern */
M2C_UNK PrintWindowNumberWithStackArguments(u32, s32, s32, s32)
    asm("func_080984C4");
M2C_UNK ClearWindow(s32) asm("func_080986B4");                         /* extern */
M2C_UNK RunMenuScript(s32) asm("func_08098BB4");                         /* extern */
M2C_UNK QueuePilotPortraitGraphics(u8, s32, u8, M2C_UNK, s32, s32) asm("func_0809A9C8"); /* extern */
s32 TestEventFlag(s32) asm("func_0809F818");                             /* extern */
s32 GetZoidBaseFormId() asm("func_080E5354");                                /* extern */
M2C_UNK RecalculateZoidStats(void *, void *) asm("func_080E5880");              /* extern */
s32 AddEquipmentToInventory(u16, s32) asm("func_080E5CE4");                        /* extern */
s32 UnlockZoidData(u8) asm("func_080E5DC4");                              /* extern */
s32 AddZoidCoresToInventory(u8, s32) asm("func_080E5E0C");                         /* extern */
M2C_UNK AddPlayerMoney(u32) asm("func_080E5E64");                         /* extern */
M2C_UNK AssignZoidToPlayerTeam(s32, u8) asm("func_080E5FA8");                     /* extern */
M2C_UNK RemoveZoidFromPlayerTeam(u8) asm("func_080E6020");                          /* extern */
M2C_UNK RemoveZoidFromPlayerTeamWide(s32) asm("func_080E6020");
M2C_UNK RestoreDestroyedZoidHp(s32) asm("func_080E6090");                         /* extern */
M2C_UNK RemovePlayerPilotAndTemporaryZoid(u8) asm("func_080E6F6C");                          /* extern */
M2C_UNK RemovePlayerPilotAndTemporaryZoidWide(s32) asm("func_080E6F6C");
s32 UpdatePilotAbilities(void *, s32) asm("func_080E705C");                     /* extern */
M2C_UNK FormatPilotAbilityText(u8, u8, M2C_UNK) asm("func_080E7664");             /* extern */
M2C_UNK FormatPilotAbilityTextWide(s32, s32, M2C_UNK)
    asm("func_080E7664");
s32 UpdatePulseEffectsFromEmotionGrowth() asm("func_080E79F4");                                /* extern */
s32 GetPilotDisplayName(u8) asm("func_080E7B64");                              /* extern */
M2C_UNK StopAllMusicPlayers() asm("func_080EB888");                            /* extern */
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */
u16 DivideSigned32(s32, s16) asm("func_080ECD98");                        /* extern */
u16 DivideSigned32FullArguments(s32, s32) asm("func_080ECD98");
s32 ModuloUnsigned32(void *, s32) asm("func_080ECF78");                     /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */
M2C_UNK jtbl_080C7B40();                            /* static */
extern s32 gEquipmentNameTable[] asm("D_087EE170");

void ProcessBattleOutcomeAndRewards(u8 outcome_mode) asm("func_080C7190");

void ProcessBattleOutcomeAndRewards(u8 outcome_mode) {
    u8 reward_model_choices[6];
    union BattleRewardScratch {
        u8 core_choices[12];
        struct {
            u8 overlap[4];
            u16 item_ids[24];
        } equipment_choices;
    } scratch;
    u8 manual_stat_growth_points[6];
    u32 reserve_pilot_experience;
    u32 money_reward;
    s32 level_up_ui_started;
    s32 manual_growth_points_remaining;
    u8 *manual_growth_cursor;
    s32 reserve_pilot_count;
    u32 next_unit_or_pilot_slot;
    u8 *pilot_level_address;
    u8 *pilot_growth_class_address;
    register volatile u8 **physical_stack asm("sp");
#define saved_battle_unit_record physical_stack[31]
    s32 temp_r0_10;
    s32 temp_r0_12;
    s32 temp_r0_23;
    s32 temp_r0_3;
    s32 temp_r0_6;
    register s32 temp_r0_7 asm("r0");
    s32 temp_r2_2;
    s32 temp_r3_4;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 temp_r4_4;
    s32 temp_r4_6;
    s32 temp_r5_2;
    register s32 encounter_group_offset_or_growth_points_address asm("r6");
    s32 manual_growth_redraw_or_stat_index;
    u16 reward_equipment_item_id;
    u32 temp_r0_11;
    u32 temp_r0_24;
    u32 temp_r0_2;
    u32 temp_r0_4;
    u32 temp_r0_9;
    u32 temp_r1_10;
    u32 temp_r2_3;
    u32 var_r0;
    u32 participant_experience_or_equipment_table;
    u8 *temp_r0_26;
    u8 *temp_r1_7;
    register u8 *temp_r3_2 asm("r3");
    register u8 *battle_unit_record asm("r3");
    register u32 outcome_window_or_text_address asm("r9");
    u8 temp_r0_13;
    u8 temp_r0_14;
    u8 temp_r0_16;
    u8 temp_r0_17;
    u8 temp_r0_19;
    u8 temp_r0_20;
    u8 temp_r0_22;
    u8 temp_r0_25;
    u8 temp_r0_28;
    u8 temp_r0_29;
    u8 temp_r0_30;
    u8 temp_r0_31;
    u8 temp_r0_5;
    u8 temp_r0_8;
    u8 temp_r1;
    u8 temp_r1_2;
    u8 temp_r1_3;
    u8 temp_r1_4;
    u8 temp_r1_5;
    u32 temp_r1_6;
    u8 temp_r1_8;
    u8 temp_r1_9;
    u8 temp_r2;
    u8 temp_r3_3;
    u8 reward_core_id;
    register u32 battle_state_or_original_party_slot asm("r5");
    u8 temp_r5_3;
    u8 reward_zoid_data_model_id;
    u8 data_reward_model_count;
    u8 core_reward_model_count;
    u8 reward_core_choice_count;
    u8 encounter_equipment_slot;
    u8 var_r5_3;
    u8 var_r7;
    u8 var_r7_2;
    u8 reward_equipment_choice_count;
    register u32 reserve_pilot_slot asm("r8");
    register u32 defeat_protagonist_scan_slot asm("r8");
    register u32 defeat_juno_scan_slot asm("r8");
    register u32 data_reward_enemy_slot asm("r8");
    register u32 core_reward_enemy_slot asm("r8");
    register u32 required_core_index asm("r8");
    register u32 core_reward_removed_model_index asm("r8");
    register u32 equipment_reward_enemy_slot asm("r8");
    register u32 level_up_pilot_slot asm("r8");
    register u32 manual_growth_selected_row;
    u32 menu_row;
    u32 menu_frame_anchor;
    register u32 menu_one_r5 asm("r5");
    u8 *data_reward_encounter_unit_base;
    u8 *core_reward_encounter_unit_base;
    u8 *temp_r0_21;
    u8 *temp_r3;
    u8 *equipment_reward_encounter_unit_base;
    register u8 *temp_r4_7 asm("r4");
    register u8 *player_state_or_pulse_record asm("r4");
    u8 *current_stored_zoid;
    u8 *original_stored_zoid;
    register u8 *reserve_pilot asm("r7");
    register u8 *participant_pilot asm("r7");
    register u8 *participant_pilot_or_zoid_storage_base asm("r7");
    u8 *level_up_pilot;
    u8 *learned_ability_index;
    u8 *learned_pulse_effect_index;
    register u8 *original_zoid_pilot_or_null asm("r7");
    register u32 reward_table_seed asm("r0");
    register u32 reward_successor asm("r8");
    register u32 result_buffer_seed asm("r3");
    register u32 battle_unit_slot asm("r8");
    register u32 owner_map_offset asm("r1");
    register u8 *owner_map_ptr asm("r0");
    register u8 *active_base asm("r2");
    register u32 active_offset asm("r3");
    register u8 *active_sum asm("r0");
    register u8 *active_ptr asm("r1");
    u8 active_value;
    register u8 *roster_base asm("r1");
    register u8 *battle_base asm("r4");
    register u32 battle_offset asm("r2");
    register u8 *battle_sum asm("r1");
    register u8 *battle2_base asm("r2");
    register u32 battle2_offset asm("r4");
    register u8 *battle2_sum asm("r1");
    register u8 *scan_base asm("r2");
    register u8 *scan_map_base asm("r0");
    register u32 scan_map_offset asm("r1");
    register u8 *scan_selected asm("r1");
    register u32 scan_selected_offset asm("r3");
    register u8 *scan_selected_sum asm("r0");
    register u32 scan_first_value asm("r0");
    register u8 *scan2_base asm("r2");
    register u8 *scan2_map_base asm("r3");
    register u32 scan2_map_offset asm("r4");
    register u8 *scan2_selected asm("r1");
    register u8 *scan2_selected_base asm("r4");
    register u8 *scan2_selected_sum asm("r0");
    register u8 *mode_base asm("r4");
    register u32 mode_offset asm("r1");
    register u8 *mode_ptr asm("r5");
    u32 mode_index;
    register u32 mode_table_offset asm("r3");
    register u8 *mode_table asm("r1");
    register u8 *mode2_root asm("r1");
    register u32 mode2_offset asm("r2");
    register u8 *mode2_ptr asm("r4");
    register u8 *mode2_table_root asm("r4");
    register u32 mode2_table_offset asm("r5");
    register u8 *mode2_table asm("r2");
    register u8 *mode2_count_ptr asm("r0");
    register u8 *catalog_base asm("r5");
    register u8 *catalog_coord_base asm("r2");
    register u32 catalog_row_seed asm("r1");
    register u32 catalog_row100 asm("r3");
    register u32 catalog_address asm("r0");
    u32 catalog_page_seed;
    register u32 catalog_page2000 asm("r2");
    register u32 catalog_outer_seed asm("r1");
    register s32 stat_left asm("r0");
    register s32 stat_right asm("r1");
    register u32 stat_left_offset asm("r2");
    register u32 stat_right_offset asm("r3");
    register u8 *state13_root asm("r0");
    register u32 state13_offset asm("r2");
    register u8 *state13_control asm("r2");
    register u8 *state13_catalog_seed asm("r4");
    register u8 *state13_catalog_base asm("ip");
    register u32 state13_page_seed asm("r1");
    register u32 state13_work asm("r0");
    register u32 core_reward_copy_slot asm("r8");
    register u8 *header_mode_base asm("r1");
    register u32 header_mode_offset asm("r2");
    register u8 *header_mode_ptr asm("r0");
    register u8 *header_value_base asm("r3");
    register u32 header_value_offset asm("r4");
    register u8 *header_value_ptr asm("r0");
    register u8 *header_value15_base asm("r1");
    register u32 header_value15_offset asm("r2");
    register u8 *header_value15_ptr asm("r0");
    register u8 *header_mode2_base asm("r4");
    register u32 header_mode2_offset asm("r5");
    register u8 *header_mode2_ptr asm("r0");
    register u32 header_value2_offset asm("r1");
    register u8 *header_value2_ptr asm("r0");
    register u8 *header_value14_base asm("r2");
    register u32 header_value14_offset asm("r3");
    register u8 *header_value14_ptr asm("r0");
    register u32 header_half asm("r5");
    register u8 *initial_roster_base asm("r2");
    register u32 initial_active asm("r0");
    register u32 tail_seed asm("r0");
    register u8 *tail_base asm("r2");
    register u32 tail_offset asm("r1");
    register u32 tail_record_offset asm("r0");
    register u32 tail_status_offset asm("r5");
    register u32 tail_status asm("r1");
    register u32 tail_data_offset asm("r0");
    register u8 *tail_data_base asm("r1");
    register u32 tail2_seed asm("r4");
    register u8 *tail2_base asm("r4");
    register u32 tail2_index asm("r5");
    register u32 tail2_record asm("r0");
    register u32 tail2_offset asm("r1");

    outcome_window_or_text_address = outcome_mode;
    header_mode_base = (u8 *)BATTLE_STATE_RAM;
    header_mode_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
    asm volatile("" : "+r"(header_mode_base), "+r"(header_mode_offset));
    header_mode_ptr = header_mode_base + header_mode_offset;
    if (*header_mode_ptr == DECK_COMMAND_PROVEN_HERO) {
        goto block_2;
    }
    header_value_base = header_mode_base;
    header_value_offset = BATTLE_REWARD_TOTALS_OFFSET(experience_total);
    asm volatile("" : "+r"(header_value_base), "+r"(header_value_offset));
    header_value_ptr = header_value_base + header_value_offset;
    participant_experience_or_equipment_table = *(u32 *)header_value_ptr;
    asm volatile("mov %0, sl" : "=r"(header_half));
    header_half >>= 1;
    reserve_pilot_experience = header_half;
    asm volatile("" : : "m"(reserve_pilot_experience));
    goto block_3;
block_2:
    header_value15_base = (u8 *)BATTLE_STATE_RAM;
    header_value15_offset = BATTLE_REWARD_TOTALS_OFFSET(experience_total);
    asm volatile("" : "+r"(header_value15_base), "+r"(header_value15_offset));
    header_value15_ptr = header_value15_base + header_value15_offset;
    temp_r0_2 = *(u32 *)header_value15_ptr;
    asm volatile(
        "lsl r3, %1, #1\n\t"
        "mov %0, r3"
        : "=r"(participant_experience_or_equipment_table)
        : "r"(temp_r0_2)
        : "r3", "cc");
    reserve_pilot_experience = temp_r0_2;
block_3:
    header_mode2_base = (u8 *)BATTLE_STATE_RAM;
    header_mode2_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
    asm volatile("" : "+r"(header_mode2_base), "+r"(header_mode2_offset));
    header_mode2_ptr = header_mode2_base + header_mode2_offset;
    if (*header_mode2_ptr == DECK_COMMAND_SUPPLIER) {
        goto block_6;
    }
    header_value2_offset = BATTLE_REWARD_TOTALS_OFFSET(money_total);
    asm volatile("" : "+r"(header_value2_offset));
    header_value2_ptr = header_mode2_base + header_value2_offset;
    var_r0 = *(u32 *)header_value2_ptr;
    goto block_7;
block_6:
    header_value14_base = (u8 *)BATTLE_STATE_RAM;
    header_value14_offset = BATTLE_REWARD_TOTALS_OFFSET(money_total);
    asm volatile("" : "+r"(header_value14_base), "+r"(header_value14_offset));
    header_value14_ptr = header_value14_base + header_value14_offset;
    var_r0 = *(u32 *)header_value14_ptr * 2;
block_7:
    money_reward = var_r0;
    {
        register u32 initial_mode_r4 asm("r4") = outcome_window_or_text_address;

        asm volatile("" : "+r"(initial_mode_r4));
        if (initial_mode_r4 != BATTLE_OUTCOME_VICTORY) {
            goto block_15;
        }
    }
    {
        register u32 initial_count_r5 asm("r5") = 0;

        asm volatile("" : "+r"(initial_count_r5));
        reserve_pilot_count = initial_count_r5;
    }
    {
        register u32 initial_index_r0 asm("r0") = 1;

        asm volatile("" : "+r"(initial_index_r0));
        reserve_pilot_slot = initial_index_r0;
    }
    initial_roster_base = (u8 *)0x02027378;
    asm volatile("" : "+r"(initial_roster_base));
loop_9:
    {
        register u32 initial_index_view_r1 asm("r1") = reserve_pilot_slot;
        register u32 initial_offset_r0 asm("r0");

        asm volatile("" : "+r"(initial_index_view_r1));
        initial_offset_r0 = initial_index_view_r1 << 6;
        asm volatile("" : "+r"(initial_offset_r0));
        asm volatile("add %0, %1, %2"
                     : "=r"(reserve_pilot)
                     : "r"(initial_offset_r0), "r"(initial_roster_base)
                     : "cc");
    }
    asm volatile("ldrb %0, [%1]" : "=r"(initial_active) : "r"(reserve_pilot));
    if (initial_active == 0) {
        goto block_14;
    }
    {
        register u32 initial_flags asm("r1");

        asm volatile("ldrh %0, [%1, #2]"
                     : "=r"(initial_flags)
                     : "r"(reserve_pilot)
                     : "memory");
        if (4 & initial_flags) {
            goto block_14;
        }
    }
    {
        register u32 initial_value_r0 asm("r0") =
            M2C_FIELD(reserve_pilot, u32 *, PLAYER_PILOT_OFFSET(experience));
        register u32 initial_gain_r3 asm("r3") = reserve_pilot_experience;

        asm volatile("" : "+r"(initial_value_r0), "+r"(initial_gain_r3));
        temp_r0_4 = initial_value_r0 + initial_gain_r3;
    }
    M2C_FIELD(reserve_pilot, u32 *, PLAYER_PILOT_OFFSET(experience)) = temp_r0_4;
    if (temp_r0_4 <= PILOT_EXPERIENCE_LIMIT) {
        goto block_13;
    }
    M2C_FIELD(reserve_pilot, u32 *, PLAYER_PILOT_OFFSET(experience)) = PILOT_EXPERIENCE_LIMIT;
block_13:
    reserve_pilot_count = (s32) (u8) (reserve_pilot_count + 1);
block_14:
    temp_r0_5 = reserve_pilot_slot + 1;
    reserve_pilot_slot = temp_r0_5;
    if ((u32) temp_r0_5 <= 0x34U) {
        goto loop_9;
    }
block_15:
    {
        register u32 scan_zero_r4 asm("r4") = 0;

        asm volatile("" : "+r"(scan_zero_r4));
        battle_unit_slot = scan_zero_r4;
    }
loop_16:
    battle_state_or_original_party_slot = BATTLE_STATE_RAM;
    owner_map_offset = BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots);
    asm volatile("" : "+r"(battle_state_or_original_party_slot), "+r"(owner_map_offset));
    owner_map_ptr = (u8 *)(battle_state_or_original_party_slot + owner_map_offset);
    owner_map_ptr += battle_unit_slot;
    battle_state_or_original_party_slot = *owner_map_ptr;
    active_base = (u8 *)0x020218E4;
    active_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
    asm volatile("" : "+r"(active_base), "+r"(active_offset));
    active_sum = active_base + active_offset;
    active_ptr = (u8 *)(battle_state_or_original_party_slot + (u32)active_sum);
    active_value = *active_ptr;
    {
        register u32 scan_successor_r4 asm("r4") = battle_unit_slot + 1;

        asm volatile("" : "+r"(scan_successor_r4));
        next_unit_or_pilot_slot = scan_successor_r4;
    }
    asm volatile("" : "+r"(battle_unit_slot));
    if (active_value != 0) {
        goto block_18;
    }
    goto block_73;
block_18:
    temp_r2 = *active_ptr;
    temp_r0_6 = (temp_r2 << 3) - temp_r2;
    temp_r0_6 <<= 4;
    roster_base = (u8 *)0x020218E4;
    asm volatile("" : "+r"(roster_base));
    roster_base += 4;
    asm volatile("add %0, %1, %2"
                 : "=r"(current_stored_zoid)
                 : "r"(temp_r0_6), "r"(roster_base)
                 : "cc");
    {
        register u32 scan_index_r1 asm("r1") = battle_unit_slot;

        asm volatile("" : "+r"(scan_index_r1));
        temp_r0_7 =
            (((s32)(((s32)scan_index_r1 * 4) + battle_unit_slot) * 8)
             - scan_index_r1) * 0x10;
    }
    {
        register u8 *record_base asm("r4") = (u8 *)BATTLE_STATE_RAM;
        asm volatile("" : "+r"(record_base));
        asm volatile("add %0, %1, %2"
                     : "=r"(battle_unit_record)
                     : "r"(temp_r0_7), "r"(record_base)
                     : "cc");
    }
    temp_r0_8 = M2C_FIELD(battle_state_or_original_party_slot, u8 *, BATTLE_PARTY_IDENTITY_ADDRESS(original_party_stored_zoid_slots));
    if (temp_r2 == temp_r0_8) {
        goto block_20;
    }
    goto block_44;
block_20:
    {
        register u32 record_flags_r1 asm("r1") =
            BATTLE_UNIT_FIELD(battle_unit_record, u16, flags);
        register u32 flag_r2 asm("r2") = 8;
        register u32 test_r0 asm("r0") = 8;
        asm volatile("" : "+r"(record_flags_r1), "+r"(flag_r2),
                     "+r"(test_r0));
        if (!(test_r0 & record_flags_r1)) {
            goto block_23;
        }
        M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(flags)) =
            (u16) (M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(flags)) | flag_r2);
    }
    {
        register u32 call_arg_r0 asm("r0") = battle_state_or_original_party_slot;
        asm volatile("" : "+r"(call_arg_r0));
        saved_battle_unit_record = battle_unit_record;
        RemoveZoidFromPlayerTeamWide(call_arg_r0);
    }
    goto block_29;
block_23:
    if (outcome_window_or_text_address != 1) {
        goto block_30;
    }
    temp_r0_9 = M2C_FIELD(current_stored_zoid, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) << 6;
    battle_base = (u8 *)0x020218E4;
    battle_offset = 0x5A94;
    asm volatile("" : "+r"(battle_base), "+r"(battle_offset));
    battle_sum = battle_base + battle_offset;
    participant_pilot = (u8 *)(temp_r0_9 + (u32)battle_sum);
    {
        register u32 active_value_r1 asm("r1") =
            M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(level));
        register s32 active_signed_r0 asm("r0");

        asm volatile("" : "+r"(active_value_r1));
        asm volatile(".syntax unified\n\t"
                     "movs r4, #16\n\t"
                     "ldrsh r0, [%1, r4]\n\t"
                     ".syntax divided"
                     : "=r"(active_signed_r0)
                     : "r"(current_stored_zoid)
                     : "r4", "cc");
        if (active_signed_r0 > 0xC7) {
            goto block_26;
        }
        active_signed_r0 = active_value_r1 + 1;
        asm volatile("" : "+r"(active_signed_r0));
        M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(level)) =
            (u16) active_signed_r0;
    }
block_26:
    temp_r0_9 = M2C_FIELD(participant_pilot, u32 *, PLAYER_PILOT_OFFSET(experience)) + participant_experience_or_equipment_table;
    M2C_FIELD(participant_pilot, u32 *, PLAYER_PILOT_OFFSET(experience)) = temp_r0_9;
    if (temp_r0_9 <= PILOT_EXPERIENCE_LIMIT) {
        goto block_28;
    }
    M2C_FIELD(participant_pilot, u32 *, PLAYER_PILOT_OFFSET(experience)) = PILOT_EXPERIENCE_LIMIT;
block_28:
    {
        register void *call_arg0_r0 asm("r0") = current_stored_zoid;
        register void *call_arg1_r1 asm("r1") = participant_pilot;
        asm volatile("" : "+r"(call_arg0_r0), "+r"(call_arg1_r1));
        saved_battle_unit_record = battle_unit_record;
        RecalculateZoidStats(call_arg0_r0, call_arg1_r1);
    }
block_29:
    battle_unit_record = saved_battle_unit_record;
block_30:
    if (*current_stored_zoid == *battle_unit_record) {
        goto block_34;
    }
    saved_battle_unit_record = battle_unit_record;
    temp_r4 = GetZoidBaseFormId();
    battle_unit_record = saved_battle_unit_record;
    temp_r0_10 = GetZoidBaseFormId(*battle_unit_record);
    temp_r4 <<= 0x18;
    temp_r0_10 <<= 0x18;
    battle_unit_record = saved_battle_unit_record;
    if (temp_r4 != temp_r0_10) {
        goto block_38;
    }
    temp_r1 = *battle_unit_record;
    if (*current_stored_zoid == temp_r1) {
        goto block_34;
    }
    *current_stored_zoid = temp_r1;
    {
        register u32 call_arg1_r1 asm("r1") =
            M2C_FIELD(current_stored_zoid, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) << 6;
        register u8 *call_arg0_r0 asm("r0") = (u8 *)0x02027378;
        asm volatile("" : "+r"(call_arg1_r1), "+r"(call_arg0_r0));
        call_arg1_r1 += (u32)call_arg0_r0;
        call_arg0_r0 = current_stored_zoid;
        asm volatile("" : "+r"(call_arg0_r0), "+r"(call_arg1_r1));
        saved_battle_unit_record = battle_unit_record;
        RecalculateZoidStats(call_arg0_r0, (void *)call_arg1_r1);
    }
    battle_unit_record = saved_battle_unit_record;
block_34:
    {
        register u32 record_value_r4 asm("r4") =
            BATTLE_UNIT_FIELD(battle_unit_record, u16, hp);
        register u32 slot_r2 asm("r2") = 6;
        register s32 record_signed_r1 asm("r1");
        register u32 active_offset_r3 asm("r3");
        register s32 active_signed_r0 asm("r0");

        asm volatile("" : "+r"(record_value_r4));
        asm volatile("" : "+r"(slot_r2));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(record_signed_r1)
                     : "r"(battle_unit_record), "r"(slot_r2)
                     : "memory");
        asm volatile("" : "+r"(record_signed_r1));
        slot_r2 = M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(max_hp));
        asm volatile("" : "+r"(slot_r2));
        asm volatile("" : "=r"(active_offset_r3));
        active_signed_r0 = M2C_FIELD(current_stored_zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp));
        asm volatile("" : "+r"(active_signed_r0));
        if (record_signed_r1 > active_signed_r0) {
            goto block_37;
        }
        M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) = record_value_r4;
        {
            register u32 next_r4 asm("r4") = battle_unit_slot;
            asm volatile("" : "+r"(next_r4));
            next_r4 += 1;
            next_unit_or_pilot_slot = next_r4;
        }
        goto block_73;
block_37:
        M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) = slot_r2;
        {
            register u32 next_r5 asm("r5") = battle_unit_slot;
            asm volatile("" : "+r"(next_r5));
            next_r5 += 1;
            next_unit_or_pilot_slot = next_r5;
        }
        goto block_73;
    }
block_38:
    {
    register u32 scan_counter_r7 asm("r7") = 0;
    asm volatile("" : "+r"(scan_counter_r7));
    scan_map_base = (u8 *)BATTLE_STATE_RAM;
    scan_map_offset = BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots);
    asm volatile("" : "+r"(scan_map_base), "+r"(scan_map_offset));
    scan_base = scan_map_base + scan_map_offset;
    {
        register u32 selected_address_r3 asm("r3") = 0x02037252;
        register u8 *selected_ptr_r1 asm("r1");
        register u32 first_value_r0 asm("r0");
        register u32 selected_value_r1 asm("r1");
        asm volatile("" : "+r"(selected_address_r3));
        selected_ptr_r1 = battle_state_or_original_party_slot + selected_address_r3;
        asm volatile("" : "+r"(selected_ptr_r1));
        first_value_r0 = *scan_base;
        asm volatile("" : "+r"(first_value_r0));
        {
            register u32 next_r4 asm("r4") = battle_unit_slot;
            asm volatile("" : "+r"(next_r4));
            next_r4 += 1;
            next_unit_or_pilot_slot = next_r4;
        }
        selected_value_r1 = *selected_ptr_r1;
        asm volatile("" : "+r"(selected_value_r1));
        if (first_value_r0 == selected_value_r1) {
            goto block_42;
        }
    }
    asm volatile("");
    scan_selected = (u8 *)BATTLE_STATE_RAM;
    scan_selected_offset = BATTLE_COMBINATION_OFFSET(combination_owner_original_party_slots);
    asm volatile("" : "+r"(scan_selected), "+r"(scan_selected_offset));
    scan_selected_sum = scan_selected + scan_selected_offset;
    scan_selected = (u8 *)((u32)battle_state_or_original_party_slot + (u32)scan_selected_sum);
loop_40:
    {
        register u32 next_r0 asm("r0") = scan_counter_r7 + 1;
        asm volatile("" : "+r"(next_r0));
        next_r0 <<= 0x18;
        scan_counter_r7 = next_r0 >> 0x18;
        asm volatile("" : "+r"(scan_counter_r7));
    }
    if (scan_counter_r7 > 5U) {
        goto block_42;
    }
    {
        register u32 scan_work_r0 asm("r0");
        register u32 scan_selected_value_r4 asm("r4");

        asm volatile("add %0, %1, %2"
                     : "=r"(scan_work_r0)
                     : "r"(scan_counter_r7), "r"(scan_base)
                     : "cc");
        scan_work_r0 = *(u8 *)scan_work_r0;
        asm volatile("" : "+r"(scan_work_r0));
        scan_selected_value_r4 = *scan_selected;
        asm volatile("" : "+r"(scan_selected_value_r4));
        if (scan_work_r0 != scan_selected_value_r4) {
            goto loop_40;
        }
    }
block_42:
    {
        register s32 work_r0 asm("r0");
        register u8 *record_base_r5 asm("r5");
        register u8 *record_r3 asm("r3");
        register s32 work_r1 asm("r1");
        register u32 slot_r2 asm("r2");
        register u32 slot_r4 asm("r4");
        asm volatile(
            "lsl %0, %1, #2\n\t"
            "add %0, %0, %1\n\t"
            "lsl %0, %0, #3\n\t"
            "sub %0, %0, %1\n\t"
            "lsl %0, %0, #4"
            : "=&r"(work_r0)
            : "r"(scan_counter_r7));
        record_base_r5 = (u8 *)BATTLE_STATE_RAM;
        asm volatile("" : "+r"(record_base_r5));
        record_r3 = (u8 *)(work_r0 + (u32)record_base_r5);
        asm volatile("" : "+r"(record_r3));
        work_r0 = 0x3A;
        asm volatile("" : "+r"(work_r0));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(work_r1)
                     : "r"(current_stored_zoid), "r"(work_r0));
        slot_r2 = 6;
        asm volatile("" : "+r"(slot_r2));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(work_r0)
                     : "r"(record_r3), "r"(slot_r2));
        work_r0 *= work_r1;
        slot_r4 = 0x3A;
        asm volatile("" : "+r"(slot_r4));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(work_r1)
                     : "r"(record_r3), "r"(slot_r4));
        M2C_FIELD(current_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) =
            DivideSigned32FullArguments(work_r0, work_r1);
    }
    goto block_73;
    }
block_44:
    if (temp_r0_8 != 0) {
        goto block_46;
    }
    goto block_73;
block_46:
    if (8 & BATTLE_UNIT_FIELD(battle_unit_record, u16, flags)) {
        goto block_50;
    }
    if (outcome_window_or_text_address != 1) {
        goto block_50;
    }
    {
        register u32 battle_index_r0 asm("r0") =
            M2C_FIELD(current_stored_zoid, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) << 6;
        asm volatile("" : "+r"(battle_index_r0));
        battle2_base = (u8 *)0x020218E4;
        battle2_offset = 0x5A94;
        asm volatile("" : "+r"(battle2_base), "+r"(battle2_offset));
        battle2_sum = battle2_base + battle2_offset;
        participant_pilot_or_zoid_storage_base = (u8 *)(battle_index_r0 + (u32)battle2_sum);
    }
    temp_r0_11 = M2C_FIELD(participant_pilot_or_zoid_storage_base, u32 *, 4) + participant_experience_or_equipment_table;
    M2C_FIELD(participant_pilot_or_zoid_storage_base, u32 *, 4) = temp_r0_11;
    if (temp_r0_11 <= PILOT_EXPERIENCE_LIMIT) {
        goto block_50;
    }
    M2C_FIELD(participant_pilot_or_zoid_storage_base, u32 *, 4) = PILOT_EXPERIENCE_LIMIT;
block_50:
    {
        register u32 record_offset_r0 asm("r0") =
            M2C_FIELD(battle_state_or_original_party_slot, u8 *, BATTLE_PARTY_IDENTITY_ADDRESS(original_party_stored_zoid_slots)) * 0x70;
        asm volatile("" : "+r"(record_offset_r0));
        participant_pilot_or_zoid_storage_base = (u8 *)0x020218E8;
        asm volatile("" : "+r"(participant_pilot_or_zoid_storage_base));
        original_stored_zoid = (u8 *)(record_offset_r0 + (u32)participant_pilot_or_zoid_storage_base);
    }
    if (*original_stored_zoid == *battle_unit_record) {
        goto block_54;
    }
    saved_battle_unit_record = battle_unit_record;
    temp_r4_2 = GetZoidBaseFormId();
    battle_unit_record = saved_battle_unit_record;
    temp_r0_10 = GetZoidBaseFormId(*battle_unit_record);
    temp_r4_2 <<= 0x18;
    temp_r0_10 <<= 0x18;
    battle_unit_record = saved_battle_unit_record;
    if (temp_r4_2 != temp_r0_10) {
        goto block_58;
    }
    temp_r1_2 = BATTLE_UNIT_FIELD(battle_unit_record, u8, zoid_id);
    if (*original_stored_zoid == temp_r1_2) {
        goto block_54;
    }
    *original_stored_zoid = temp_r1_2;
    {
        register u32 call_arg1_r1 asm("r1") =
            M2C_FIELD(original_stored_zoid, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) << 6;
        register u32 call_offset_r4 asm("r4") = 0x5A90;
        register u8 *call_arg0_r0 asm("r0");
        asm volatile("" : "+r"(call_arg1_r1), "+r"(call_offset_r4));
        call_arg0_r0 = participant_pilot_or_zoid_storage_base + call_offset_r4;
        asm volatile("" : "+r"(call_arg0_r0));
        call_arg1_r1 += (u32)call_arg0_r0;
        call_arg0_r0 = original_stored_zoid;
        asm volatile("" : "+r"(call_arg0_r0), "+r"(call_arg1_r1));
        saved_battle_unit_record = battle_unit_record;
        RecalculateZoidStats(call_arg0_r0, (void *)call_arg1_r1);
    }
    battle_unit_record = saved_battle_unit_record;
block_54:
    {
        register u32 record_value_r4 asm("r4") =
            BATTLE_UNIT_FIELD(battle_unit_record, u16, hp);
        register u32 slot_r5 asm("r5") = 6;
        register s32 record_signed_r1 asm("r1");
        register u32 active_value_r2 asm("r2");
        register s32 active_signed_r0 asm("r0");

        asm volatile("" : "+r"(record_value_r4));
        asm volatile("" : "+r"(slot_r5));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(record_signed_r1)
                     : "r"(battle_unit_record), "r"(slot_r5));
        active_value_r2 = M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(max_hp));
        asm volatile("" : "+r"(active_value_r2));
        asm volatile("" : "=r"(slot_r5));
        active_signed_r0 = M2C_FIELD(original_stored_zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp));
        asm volatile("" : "+r"(active_signed_r0));
        if (record_signed_r1 > active_signed_r0) {
            goto block_57;
        }
        M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) = record_value_r4;
        {
            register u32 next_r0 asm("r0") = battle_unit_slot;
            asm volatile("" : "+r"(next_r0));
            next_r0 += 1;
            next_unit_or_pilot_slot = next_r0;
        }
        goto block_63;
block_57:
        M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) = active_value_r2;
        {
            register u32 next_r1 asm("r1") = battle_unit_slot;
            asm volatile("" : "+r"(next_r1));
            next_r1 += 1;
            next_unit_or_pilot_slot = next_r1;
        }
        goto block_63;
    }
block_58:
    var_r7_2 = 0;
    scan2_map_base = (u8 *)BATTLE_STATE_RAM;
    scan2_map_offset = BATTLE_COMBINATION_OFFSET(current_unit_original_party_slots);
    asm volatile("" : "+r"(scan2_map_base), "+r"(scan2_map_offset));
    scan2_base = scan2_map_base + scan2_map_offset;
    {
        register u8 *selected_ptr_r1 asm("r1");
        register u32 first_value_r0 asm("r0") = 0x02037252;
        register u32 selected_value_r1 asm("r1");
        asm volatile("" : "+r"(first_value_r0));
        selected_ptr_r1 = battle_state_or_original_party_slot + first_value_r0;
        asm volatile("" : "+r"(selected_ptr_r1));
        first_value_r0 = *scan2_base;
        asm volatile("" : "+r"(first_value_r0));
        {
            register u32 next_r3 asm("r3") = battle_unit_slot;
            asm volatile("" : "+r"(next_r3));
            next_r3 += 1;
            next_unit_or_pilot_slot = next_r3;
        }
        selected_value_r1 = *selected_ptr_r1;
        asm volatile("" : "+r"(selected_value_r1));
        if (first_value_r0 == selected_value_r1) {
            goto block_62;
        }
    }
    asm volatile("");
    scan2_selected_base = (u8 *)BATTLE_STATE_RAM;
    scan2_selected = (u8 *)BATTLE_COMBINATION_OFFSET(combination_owner_original_party_slots);
    asm volatile("" : "+r"(scan2_selected_base), "+r"(scan2_selected));
    scan2_selected_sum = scan2_selected_base + (u32)scan2_selected;
    scan2_selected = (u8 *)((u32)battle_state_or_original_party_slot + (u32)scan2_selected_sum);
loop_60:
    var_r7_2 += 1;
    if ((u32) var_r7_2 > 5U) {
        goto block_62;
    }
    if (*(u8 *)(var_r7_2 - (0U - (u32)scan2_base)) !=
            *scan2_selected) {
        goto loop_60;
    }
block_62:
    {
        register s32 work_r0 asm("r0") = var_r7_2 * 0x270;
        register u8 *work_r4 asm("r4") = (u8 *)BATTLE_STATE_RAM;
        register u32 slot_r5 asm("r5");
        register s32 work_r1 asm("r1");
        register u32 slot_r2 asm("r2");
        asm volatile("" : "+r"(work_r0), "+r"(work_r4));
        temp_r3_2 = (u8 *)(work_r0 + (u32)work_r4);
        asm volatile("" : "+r"(temp_r3_2));
        slot_r5 = 0x3A;
        asm volatile("" : "+r"(slot_r5));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(work_r1)
                     : "r"(original_stored_zoid), "r"(slot_r5));
        slot_r2 = 6;
        asm volatile("" : "+r"(slot_r2));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(work_r0)
                     : "r"(temp_r3_2), "r"(slot_r2));
        work_r0 *= work_r1;
        work_r4 = (u8 *)0x3A;
        asm volatile("" : "+r"(work_r4));
        asm volatile("ldrsh %0, [%1, %2]"
                     : "=r"(work_r1)
                     : "r"(temp_r3_2), "r"(work_r4));
        saved_battle_unit_record = temp_r3_2;
        M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) =
            DivideSigned32FullArguments(work_r0, work_r1);
    }
    battle_unit_record = saved_battle_unit_record;
block_63:
    {
        register u32 record_flags_r1 asm("r1") =
            BATTLE_UNIT_FIELD(battle_unit_record, u16, flags);
        register u32 flag_r2 asm("r2") = 8;
        register u32 test_r0 asm("r0") = 8;
        asm volatile("" : "+r"(record_flags_r1), "+r"(flag_r2),
                     "+r"(test_r0));
        if (!(test_r0 & record_flags_r1)) {
            goto block_65;
        }
        M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(flags)) =
            (u16)(M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(flags)) | flag_r2);
    }
    goto block_73;
block_65:
    if (outcome_window_or_text_address != 1) {
        goto block_73;
    }
    if ((s32) (s16) M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(level)) > 0xC7) {
        goto block_68;
    }
    M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(level)) = (u16) (M2C_FIELD(original_stored_zoid, u16 *, PLAYER_ZOID_OFFSET(level)) + 1);
block_68:
    temp_r0_13 = M2C_FIELD(original_stored_zoid, u8 *, PLAYER_ZOID_OFFSET(pilot_slot));
    if (temp_r0_13 == 0) {
        goto block_71;
    }
    {
        register u32 reward_offset_r0 asm("r0") = temp_r0_13 << 6;
        register u8 *reward_base_r1 asm("r1");
        asm volatile("" : "+r"(reward_offset_r0));
        reward_base_r1 = (u8 *)0x02027378;
        asm volatile("" : "+r"(reward_base_r1));
        original_zoid_pilot_or_null = (u8 *)(reward_offset_r0 + (u32)reward_base_r1);
    }
    goto block_72;
block_71:
    original_zoid_pilot_or_null = NULL;
block_72:
    RecalculateZoidStats(original_stored_zoid, original_zoid_pilot_or_null);
block_73:
    temp_r0_14 = (u8) next_unit_or_pilot_slot;
    battle_unit_slot = temp_r0_14;
    if ((u32) temp_r0_14 > 5U) {
        goto block_75;
    }
    goto loop_16;
block_75:
    ClearWindow(0);
    {
        register s32 initial_mode_r4 asm("r4") = outcome_window_or_text_address;

        asm volatile("" : "+r"(initial_mode_r4));
        if (initial_mode_r4 == BATTLE_OUTCOME_VICTORY) {
            goto present_victory_rewards;
        }
        if (initial_mode_r4 > BATTLE_OUTCOME_VICTORY) {
            goto block_79;
        }
        if (initial_mode_r4 == BATTLE_OUTCOME_DEFEAT) {
            goto restore_party_after_defeat;
        }
    }
    return;
block_79:
    {
        register s32 final_mode_r5 asm("r5") = outcome_window_or_text_address;

        asm volatile("" : "+r"(final_mode_r5));
        if (final_mode_r5 == BATTLE_OUTCOME_RETREAT) {
            goto present_retreat_result;
        }
    }
    return;
present_victory_rewards:
    RunMenuScript(0x08003C22);
    AddPlayerMoney(money_reward);
    ClearWindow(0);
    PrintWindowText(0x081075C0, 0, 0);
    {
    register s32 block81_zero_r4 asm("r4") = 0;
    register volatile s32 *block81_outgoing asm("sp");

    block81_outgoing[0] = block81_zero_r4;
    PrintWindowNumberWithStackArguments(participant_experience_or_equipment_table, 7, 2, 0);
    RunMenuScript(0x08003B04);
    if (reserve_pilot_count == 0) {
        goto block_83;
    }
    ClearWindow(0);
    PrintWindowText(0x081075EC, 0, 0);
    block81_outgoing[0] = block81_zero_r4;
    PrintWindowNumberWithStackArguments(reserve_pilot_experience, 7, 2, 0);
    RunMenuScript(0x08003B04);
block_83:
    ClearWindow(0);
    PrintWindowText(0x0810760C, 0, 0);
    block81_outgoing[0] = block81_zero_r4;
    PrintWindowNumberWithStackArguments(money_reward, 7, 2, 0);
    }
    RunMenuScript(0x08003B12);
    mode_base = (u8 *)BATTLE_STATE_RAM;
    mode_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
    asm volatile("" : "+r"(mode_base), "+r"(mode_offset));
    mode_ptr = mode_base + mode_offset;
    if ((u32) (u8) (*mode_ptr - DECK_COMMAND_DATA_GATHER_1) <= 1U) {
        goto block_86;
    }
    if (((u32) (CallFunctionR0(*(s32 *)0x03000010) * 0xA) >> 0xF) == 0) {
        goto block_86;
    }
    goto block_100;
block_86:
    if (*mode_ptr != DECK_COMMAND_DATA_GATHER_2) {
        goto block_88;
    }
    mode_index = (u32) (CallFunctionR0(*(u32 *)0x03000010) *
        mode_base[BATTLE_DECK_REWARD_OFFSET(reward_model_choice_count)]) >> 0xF;
    mode_table_offset = BATTLE_DECK_REWARD_OFFSET(reward_model_choices);
    asm volatile("" : "+r"(mode_table_offset));
    mode_table = mode_base + mode_table_offset;
    mode_index += (u32)mode_table;
    reward_zoid_data_model_id = *(u8 *)mode_index;
    goto block_97;
block_88:
    data_reward_model_count = 0;
    data_reward_enemy_slot = 0;
    catalog_base = (u8 *)BATTLE_ENCOUNTER_TABLE_ROM;
    catalog_coord_base = (u8 *)0x0203055C;
    asm volatile("" : "+r"(catalog_base), "+r"(catalog_coord_base));
    catalog_row_seed = catalog_coord_base[4];
    asm volatile("" : "+r"(catalog_row_seed));
    catalog_row100 = catalog_row_seed;
    catalog_address = 0x64;
    catalog_row100 *= catalog_address;
    catalog_page_seed = catalog_coord_base[3];
    catalog_address = catalog_page_seed << 5;
    catalog_address -= catalog_page_seed;
    catalog_address <<= 2;
    catalog_address += catalog_page_seed;
    catalog_page2000 = catalog_address << 4;
loop_89:
    catalog_outer_seed = data_reward_enemy_slot;
    catalog_address = catalog_outer_seed << 4;
    catalog_address += catalog_row100;
    catalog_address += catalog_page2000;
    catalog_address += (u32)catalog_base;
    data_reward_encounter_unit_base = (u8 *)catalog_address;
    temp_r1_3 = M2C_FIELD(data_reward_encounter_unit_base, u8 *, 4);
    if (temp_r1_3 == 0) {
        goto block_92;
    }
    if (M2C_FIELD(data_reward_encounter_unit_base, u8 *, 7) != 0) {
        goto block_92;
    }
    reward_model_choices[data_reward_model_count] = temp_r1_3;
    data_reward_model_count += 1;
block_92:
    temp_r0_16 = data_reward_enemy_slot + 1;
    data_reward_enemy_slot = temp_r0_16;
    if ((u32) temp_r0_16 <= 5U) {
        goto loop_89;
    }
    if (data_reward_model_count == 0) {
        goto block_96;
    }
    reward_zoid_data_model_id = reward_model_choices[(u32) (CallFunctionR0(*(u32 *)0x03000010) * data_reward_model_count) >> 0xF];
    goto block_97;
block_96:
    reward_zoid_data_model_id = 0;
block_97:
    if (reward_zoid_data_model_id == 0) {
        goto block_100;
    }
    ClearWindow(0);
    RunMenuScript(0x08003B46);
    {
        register s32 *message_table_r1 asm("r1") =
            (s32 *)0x087EDD54;
        register u32 message_address_r0 asm("r0");

        asm volatile("" : "+r"(message_table_r1));
        message_address_r0 = reward_zoid_data_model_id << 2;
        message_address_r0 += (u32)message_table_r1;
        message_address_r0 = *(u32 *)message_address_r0;
        PrintWindowText(message_address_r0, 2, 0);
    }
    RunMenuScript(0x08003B9C);
    if ((UnlockZoidData(reward_zoid_data_model_id) << 0x18) != 0) {
        goto block_100;
    }
    ClearWindow(0);
    RunMenuScript(0x08003B68);
block_100:
    mode2_root = (u8 *)BATTLE_STATE_RAM;
    mode2_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
    asm volatile("" : "+r"(mode2_root), "+r"(mode2_offset));
    mode2_ptr = mode2_root + mode2_offset;
    if ((u32) (u8) (*mode2_ptr - DECK_COMMAND_CORE_SECURITY_1) <= 1U) {
        goto block_103;
    }
    if (((u32) (CallFunctionR0(*(u32 *)0x03000010) * 6) >> 0xF) == 0) {
        goto block_103;
    }
    goto block_125;
block_103:
    if (*mode2_ptr != DECK_COMMAND_CORE_SECURITY_2) {
        goto block_107;
    }
    {
        register u32 mode2_zero_r3 asm("r3") = 0;

        asm volatile("" : "+r"(mode2_zero_r3));
        core_reward_copy_slot = mode2_zero_r3;
    }
    mode2_table_root = (u8 *)BATTLE_STATE_RAM;
    mode2_table_offset = BATTLE_DECK_REWARD_OFFSET(reward_model_choices);
    asm volatile("" : "+r"(mode2_table_root), "+r"(mode2_table_offset));
    mode2_table = mode2_table_root + mode2_table_offset;
loop_105:
    reward_model_choices[core_reward_copy_slot] =
        *(u8 *)((u32)core_reward_copy_slot + (u32)mode2_table);
    temp_r0_17 = (u8) (core_reward_copy_slot + 1);
    core_reward_copy_slot = temp_r0_17;
    if ((u32) temp_r0_17 <= 5U) {
        goto loop_105;
    }
    asm volatile("" : "=r"(mode2_table_root));
    mode2_table_root = (u8 *)BATTLE_STATE_RAM;
    asm volatile("" : "+r"(mode2_table_root));
    mode2_table_offset = BATTLE_DECK_REWARD_OFFSET(reward_model_choice_count);
    mode2_count_ptr = mode2_table_root + mode2_table_offset;
    core_reward_model_count = *mode2_count_ptr;
    goto block_112;
block_107:
    core_reward_model_count = 0;
    core_reward_enemy_slot = 0;
    catalog_base = (u8 *)BATTLE_ENCOUNTER_TABLE_ROM;
    catalog_coord_base = (u8 *)0x0203055C;
    asm volatile("" : "+r"(catalog_base), "+r"(catalog_coord_base));
    catalog_row_seed = catalog_coord_base[4];
    asm volatile("" : "+r"(catalog_row_seed));
    catalog_row100 = catalog_row_seed;
    catalog_address = 0x64;
    catalog_row100 *= catalog_address;
    catalog_page_seed = catalog_coord_base[3];
    catalog_address = catalog_page_seed << 5;
    catalog_address -= catalog_page_seed;
    catalog_address <<= 2;
    catalog_address += catalog_page_seed;
    catalog_page2000 = catalog_address << 4;
loop_108:
    catalog_outer_seed = core_reward_enemy_slot;
    catalog_address = catalog_outer_seed << 4;
    catalog_address += catalog_row100;
    catalog_address += catalog_page2000;
    catalog_address += (u32)catalog_base;
    core_reward_encounter_unit_base = (u8 *)catalog_address;
    temp_r1_4 = M2C_FIELD(core_reward_encounter_unit_base, u8 *, 4);
    if (temp_r1_4 == 0) {
        goto block_111;
    }
    if (M2C_FIELD(core_reward_encounter_unit_base, u8 *, 7) != 0) {
        goto block_111;
    }
    reward_model_choices[core_reward_model_count] = temp_r1_4;
    core_reward_model_count += 1;
block_111:
    temp_r0_19 = core_reward_enemy_slot + 1;
    core_reward_enemy_slot = temp_r0_19;
    if ((u32) temp_r0_19 <= 5U) {
        goto loop_108;
    }
block_112:
    reward_core_choice_count = 0;
    if (core_reward_model_count == 0) {
        goto block_122;
    }
    {
    register u8 *reward_table_r7 asm("r7") = (u8 *)0x087B1E05;
    register u8 *scratch_base_r6 asm("r6");
    register u32 chosen_r3 asm("r3");
    asm volatile("" : "+r"(reward_table_r7));
    asm volatile("add %0, sp, #28" : "=r"(scratch_base_r6));
loop_114:
    {
        register u32 selected_r0 asm("r0") =
            CallFunctionR0(*(u32 *)0x03000010);
        selected_r0 *= core_reward_model_count;
        selected_r0 >>= 0xF;
        selected_r0 <<= 0x18;
        chosen_r3 = selected_r0 >> 0x18;
        asm volatile("" : "+r"(chosen_r3));
    }
    temp_r1_5 = reward_model_choices[chosen_r3];
    required_core_index = 0;
    temp_r2_2 = temp_r1_5 * 0xC;
loop_115:
    {
        register u32 reward_address_r0 asm("r0") =
            required_core_index + temp_r2_2;

        asm volatile("add %0, %0, %1"
                     : "+r"(reward_address_r0)
                     : "r"(reward_table_r7)
                     : "cc");
        temp_r1_5 = *(u8 *)reward_address_r0;
    }
    if (temp_r1_5 == 0) {
        goto block_117;
    }
    scratch_base_r6[reward_core_choice_count] = temp_r1_5;
    reward_core_choice_count += 1;
block_117:
    temp_r0_20 = required_core_index + 1;
    required_core_index = temp_r0_20;
    if ((u32) temp_r0_20 <= 1U) {
        goto loop_115;
    }
    if (reward_core_choice_count != 0) {
        goto block_123;
    }
    core_reward_removed_model_index = chosen_r3;
    asm volatile("" : "+r"(core_reward_removed_model_index));
    chosen_r3 = core_reward_model_count - 1;
    asm volatile("" : "+r"(chosen_r3));
    {
    register s32 bound_r4 asm("r4") = chosen_r3;
    asm volatile("" : "+r"(bound_r4));
    if ((s32) core_reward_removed_model_index >= bound_r4) {
        goto block_121;
    }
loop_120:
    {
        register u8 *dest_r2 asm("r2");
        register u32 next_r1 asm("r1");
        register u8 *source_r0 asm("r0");
        register u32 value_r0 asm("r0");
        asm volatile("mov %0, sp" : "=r"(dest_r2));
        dest_r2 += core_reward_removed_model_index;
        dest_r2 += 0x14;
        asm volatile("" : "+r"(dest_r2));
        next_r1 = core_reward_removed_model_index + 1;
        asm volatile("" : "+r"(next_r1));
        asm volatile("mov %0, sp" : "=r"(source_r0));
        source_r0 += next_r1;
        source_r0 += 0x14;
        asm volatile("" : "+r"(source_r0));
        value_r0 = *source_r0;
        asm volatile("" : "+r"(value_r0));
        *dest_r2 = value_r0;
        next_r1 <<= 0x18;
        next_r1 >>= 0x18;
        core_reward_removed_model_index = next_r1;
        asm volatile("" : "+r"(core_reward_removed_model_index));
    }
    if ((s32) core_reward_removed_model_index < (s32)chosen_r3) {
        goto loop_120;
    }
block_121:
    {
        register u32 normalized_bound_r0 asm("r0") = bound_r4 << 24;

        asm volatile("" : "+r"(normalized_bound_r0));
        core_reward_model_count = normalized_bound_r0 >> 24;
    }
    }
    if (core_reward_model_count != 0) {
        goto loop_114;
    }
    }
block_122:
    if (reward_core_choice_count == 0) {
        goto block_125;
    }
block_123:
    reward_core_id = scratch.core_choices[(u32) (CallFunctionR0(*(u32 *)0x03000010) * reward_core_choice_count) >> 0xF];
    ClearWindow(0);
    PrintWindowText(0x0810761C, 0, 0);
    {
        register s32 *message_table_r1 asm("r1") =
            (s32 *)0x087EEE60;
        register u32 message_address_r0 asm("r0");

        asm volatile("" : "+r"(message_table_r1));
        message_address_r0 = reward_core_id << 2;
        message_address_r0 += (u32)message_table_r1;
        message_address_r0 = *(u32 *)message_address_r0;
        PrintWindowText(message_address_r0, 2, 0);
    }
    RunMenuScript(0x08003B9C);
    if ((AddZoidCoresToInventory(reward_core_id, 1) << 0x18) != 0) {
        goto block_125;
    }
    ClearWindow(0);
    RunMenuScript(0x08003BE4);
block_125:
    state13_root = (u8 *)BATTLE_STATE_RAM;
    state13_offset = BATTLE_DECK_REWARD_OFFSET(persistent_command_ids);
    asm volatile("" : "+r"(state13_root), "+r"(state13_offset));
    state13_root += state13_offset;
    if (*state13_root != DECK_COMMAND_JUNK_PARTS) {
        goto block_137;
    }
    reward_equipment_choice_count = 0;
    {
        register u32 state13_outer_zero asm("r3") = 0;

        asm volatile("" : "+r"(state13_outer_zero));
        equipment_reward_enemy_slot = state13_outer_zero;
    }
    state13_catalog_seed = (u8 *)BATTLE_ENCOUNTER_TABLE_ROM;
    asm volatile("" : "+r"(state13_catalog_seed));
    state13_catalog_base = state13_catalog_seed;
    state13_control = (u8 *)0x0203055C;
    {
        register u32 catalog_row_r1 asm("r1");
        register u32 catalog_hundred_r0 asm("r0");
        register u32 catalog_product_r5 asm("r5");

        catalog_row_r1 = state13_control[4];
        asm volatile("" : "+r"(catalog_row_r1));
        catalog_hundred_r0 = 100;
        catalog_product_r5 = catalog_row_r1;
        catalog_product_r5 *= catalog_hundred_r0;
        temp_r5_2 = catalog_product_r5;
        asm volatile("" : "+r"(catalog_product_r5));
    }
    state13_page_seed = state13_control[3];
    {
        register u32 catalog_plus8_r0 asm("r0") = 8;

        asm volatile("" : "+r"(catalog_plus8_r0));
        catalog_plus8_r0 += (u32)state13_catalog_base;
        participant_experience_or_equipment_table = catalog_plus8_r0;
    }
    state13_work = state13_page_seed << 5;
    state13_work -= state13_page_seed;
    state13_work <<= 2;
    state13_work += state13_page_seed;
    encounter_group_offset_or_growth_points_address = state13_work << 4;
loop_127:
    encounter_equipment_slot = 0;
    {
        register u32 state13_outer_r1 asm("r1") = equipment_reward_enemy_slot;

        asm volatile("" : "+r"(state13_outer_r1));
        temp_r4_4 = state13_outer_r1 << 4;
    }
    asm volatile("" : : "r"(temp_r4_4));
    equipment_reward_encounter_unit_base = temp_r4_4 + temp_r5_2 + encounter_group_offset_or_growth_points_address +
        (u32)state13_catalog_base;
    asm volatile("" : : "r"(equipment_reward_encounter_unit_base));
loop_128:
    if (M2C_FIELD(equipment_reward_encounter_unit_base, u8 *, 4) == 0) {
        goto block_132;
    }
    if (M2C_FIELD(equipment_reward_encounter_unit_base, u8 *, 7) != 0) {
        goto block_132;
    }
    temp_r0_21 = encounter_equipment_slot + temp_r4_4 + temp_r5_2 + encounter_group_offset_or_growth_points_address;
    if (M2C_FIELD(temp_r0_21, u8 *, participant_experience_or_equipment_table) == 0) {
        goto block_132;
    }
    {
        register u16 *scratch_store_r1 asm("r1");
        register u32 scratch_offset_r0 asm("r0");

        asm volatile("add %0, sp, #32" : "=r"(scratch_store_r1));
        scratch_offset_r0 = reward_equipment_choice_count << 1;
        scratch_store_r1 =
            (u16 *)((u8 *)scratch_store_r1 + scratch_offset_r0);
        scratch_offset_r0 = M2C_FIELD(temp_r0_21, u8 *, participant_experience_or_equipment_table);
        *scratch_store_r1 = scratch_offset_r0;
    }
    reward_equipment_choice_count += 1;
block_132:
    encounter_equipment_slot += 1;
    if ((u32) encounter_equipment_slot <= 3U) {
        goto loop_128;
    }
    asm volatile("" : "+r"(equipment_reward_enemy_slot));
    temp_r0_22 = equipment_reward_enemy_slot + 1;
    equipment_reward_enemy_slot = temp_r0_22;
    if ((u32) temp_r0_22 <= 5U) {
        goto loop_127;
    }
    if (reward_equipment_choice_count == 0) {
        goto block_137;
    }
    {
        register u16 *scratch_load_r4 asm("r4");
        register u32 scratch_index_r0 asm("r0");

        asm volatile("add %0, sp, #32" : "=r"(scratch_load_r4));
        scratch_index_r0 = CallFunctionR0(*(u32 *)0x03000010);
        scratch_index_r0 *= reward_equipment_choice_count;
        scratch_index_r0 >>= 0xF;
        scratch_index_r0 <<= 1;
        scratch_load_r4 =
            (u16 *)((u8 *)scratch_load_r4 + scratch_index_r0);
        reward_equipment_item_id = *scratch_load_r4;
    }
    ClearWindow(0);
    PrintWindowText(0x08107638, 0, 0);
    PrintWindowText(gEquipmentNameTable[reward_equipment_item_id], 2, 0);
    RunMenuScript(0x08003B9C);
    if ((AddEquipmentToInventory(reward_equipment_item_id, 1) << 0x18) != 0) {
        goto block_137;
    }
    ClearWindow(0);
    RunMenuScript(0x08003BA6);
block_137:
    {
        register u32 reward_zero_r2 asm("r2") = 0;
        register u32 reward_one_r3 asm("r3");

        level_up_ui_started = reward_zero_r2;
        asm volatile("mov %0, #1" : "=r"(reward_one_r3));
        level_up_pilot_slot = reward_one_r3;
    }
scan_pilot_level_ups:
    {
    register u32 outer_index asm("r4") = level_up_pilot_slot;
    register u32 record_offset asm("r0");
    register u8 *record_base asm("r1");
    register u8 *record asm("r7");
    register u32 active asm("r0");

    record_offset = outer_index << 6;
    asm volatile(
        ".macro ldr dst, addr:vararg\n\t"
        ".short 0x494B\n\t"
        ".endm\n\t"
        ".macro add dst, lhs, rhs\n\t"
        ".short 0x1847\n\t"
        ".endm");
    record_base = (u8 *)0x02027378;
    record = (u8 *)(record_offset + (u32)record_base);
    asm volatile(".purgem ldr\n\t.purgem add");
    asm volatile("ldrb %0, [%1]" : "=r"(active) : "r"(record));
    outer_index += 1;
    next_unit_or_pilot_slot = outer_index;
    level_up_pilot = record;
    if (active == 0) {
        goto block_220;
    }
    {
    register u32 table_value asm("r2") = PILOT_EXPERIENCE_THRESHOLDS_ROM;
    register u8 *status_ptr asm("r1") = record;
    register u32 table_index asm("r0");

    asm volatile("" : "+r"(table_value));
    status_ptr += 0x30;
    table_index = *status_ptr;
    table_index <<= 2;
    table_index += table_value;
    table_value = *(u32 *)table_index;
    pilot_level_address = status_ptr;
    if (table_value == 0) {
        goto block_220;
    }
    if ((u32) M2C_FIELD(record, u32 *, 4) < table_value) {
        goto block_220;
    }
    }
    }
apply_next_pilot_level_up:
    if (level_up_ui_started != 0) {
        goto block_143;
    }
    RunMenuScript(0x08003DE6);
    CreateSprite(0x08359850, 0x0835985C, 0, 0x18, 0x28, 0x3C2, 0xE, 8,
        level_up_ui_started);
    level_up_ui_started = 1;
    StopAllMusicPlayers();
block_143:
    *(s8 *)0x02032EF8 = 0x34;
    PlaySong(0x34);
    RunMenuScript(0x08003DF9);
    ClearWindow(0);
    PrintWindowText(GetPilotDisplayName(*level_up_pilot), 2, 0);
    RunMenuScript(0x08003B1C);
    PrintWindowNumber(*pilot_level_address + 1, 2, 1, 0, 0);
    RunMenuScript(0x08003B3E);
    QueuePilotPortraitGraphics(*level_up_pilot, 2, 0U, 0x3C2, 0xE, 0x02002880);
    {
    register u8 *status_ptr_r0 asm("r0") = level_up_pilot;
    register u32 status_value_r1 asm("r1");

    status_ptr_r0 += 0x32;
    status_value_r1 = *status_ptr_r0;
    pilot_growth_class_address = status_ptr_r0;
    if (status_value_r1 <= 4U) {
        goto block_154;
    }
    }
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY])) = 0U;
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP])) = 0U;
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY])) = 0U;
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY])) = 0U;
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP])) = 0U;
    if (*pilot_growth_class_address != PILOT_GROWTH_CLASS_RANDOM) {
        goto block_154;
    }
    {
    register u32 reward_counter_r5 asm("r5") = 0;

    asm volatile("" : "+r"(reward_counter_r5));
loop_146:
    temp_r0_24 = (u32) (CallFunctionR0(*(u32 *)0x03000010) * 5) >> 0xF;
    if (temp_r0_24 > 4U) {
        goto block_153;
    }
    switch (temp_r0_24) {                           /* jump table: jtbl_080C7B40 */
case PILOT_RANDOM_GROWTH_MAX_HP:
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP])) = (u16) (M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP])) + 1);
    goto block_153;
case PILOT_RANDOM_GROWTH_MOBILITY:
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY])) = (u16) (M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY])) + 1);
    goto block_153;
case PILOT_RANDOM_GROWTH_SENSOR_ACCURACY:
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY])) = (u16) (M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY])) + 1);
    goto block_153;
case PILOT_RANDOM_GROWTH_DCP:
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP])) = (u16) (M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP])) + 1);
    goto block_153;
case PILOT_RANDOM_GROWTH_WEAPON_ACCURACY:
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY])) = (u16) (M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY])) + 1);
    }
block_153:
    {
        register u32 reward_next_r0 asm("r0") = reward_counter_r5 + 1;

        asm volatile("" : "+r"(reward_next_r0));
        reward_next_r0 <<= 24;
        reward_counter_r5 = reward_next_r0 >> 24;
    }
    if (reward_counter_r5 <= 9U) {
        goto loop_146;
    }
    }
block_154:
    PrintWindowNumberAt(M2C_FIELD(level_up_pilot, s16 *, PLAYER_PILOT_OFFSET(max_hp_bonus_percent)), 3, 0, 0xA, 1, 4, 0U);
    PrintWindowNumberAt(M2C_FIELD(level_up_pilot, s16 *, PLAYER_PILOT_OFFSET(mobility_bonus_percent)), 3, 0, 0xA, 1, 4, 1U);
    PrintWindowNumberAt(M2C_FIELD(level_up_pilot, s16 *, PLAYER_PILOT_OFFSET(dcp_bonus_percent)), 3, 0, 0xA, 1, 4, 2U);
    PrintWindowNumberAt(M2C_FIELD(level_up_pilot, s16 *, PLAYER_PILOT_OFFSET(sensor_accuracy_bonus_percent)), 3, 0, 0xA, 1, 4, 3U);
    PrintWindowNumberAt(M2C_FIELD(level_up_pilot, s16 *, PLAYER_PILOT_OFFSET(weapon_accuracy_bonus_percent)), 3, 0, 0xA, 1, 4, 4U);
    PrintWindowNumberAt((s16) M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP])), 3, 0, 0xA, 1, 8, 0U);
    PrintWindowNumberAt((s16) M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY])), 3, 0, 0xA, 1, 8, 1U);
    PrintWindowNumberAt((s16) M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP])), 3, 0, 0xA, 1, 8, 2U);
    PrintWindowNumberAt((s16) M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY])), 3, 0, 0xA, 1, 8, 3U);
    PrintWindowNumberAt((s16) M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY])), 3, 0, 0xA, 1, 8, 4U);
    stat_left_offset = 0x34;
    stat_right_offset = 0x26;
    stat_left = *(s16 *)(level_up_pilot + stat_left_offset);
    stat_right = *(s16 *)(level_up_pilot + stat_right_offset);
    stat_left += stat_right;
    PrintWindowNumberAtWide(stat_left, 3, 0, 0xA, 1, 0xC, 0U);
    stat_left_offset = 0x36;
    stat_right_offset = 0x28;
    stat_left = *(s16 *)(level_up_pilot + stat_left_offset);
    stat_right = *(s16 *)(level_up_pilot + stat_right_offset);
    stat_left += stat_right;
    PrintWindowNumberAtWide(stat_left, 3, 0, 0xA, 1, 0xC, 1U);
    stat_left_offset = 0x3C;
    stat_right_offset = 0x2E;
    stat_left = *(s16 *)(level_up_pilot + stat_left_offset);
    stat_right = *(s16 *)(level_up_pilot + stat_right_offset);
    stat_left += stat_right;
    PrintWindowNumberAtWide(stat_left, 3, 0, 0xA, 1, 0xC, 2U);
    stat_left_offset = 0x38;
    stat_right_offset = 0x2A;
    stat_left = *(s16 *)(level_up_pilot + stat_left_offset);
    stat_right = *(s16 *)(level_up_pilot + stat_right_offset);
    stat_left += stat_right;
    PrintWindowNumberAtWide(stat_left, 3, 0, 0xA, 1, 0xC, 3U);
    stat_left_offset = 0x3A;
    stat_right_offset = 0x2C;
    stat_left = *(s16 *)(level_up_pilot + stat_left_offset);
    stat_right = *(s16 *)(level_up_pilot + stat_right_offset);
    stat_left += stat_right;
    PrintWindowNumberAtWide(stat_left, 3, 0, 0xA, 1, 0xC, 4U);
    RequestWindowRefresh();
    {
        register volatile u16 *keys asm("r4") =
            (volatile u16 *)0x0300000E;
        register u32 mask asm("r5") = 3;
        register u32 key_value_r1 asm("r1");
        register u32 key_test_r0 asm("r0");

        do {
            YieldTaskForUpdates(1);
            asm volatile("ldrh %0, [%1]"
                         : "=r"(key_value_r1)
                         : "r"(keys)
                         : "memory");
            key_test_r0 = mask;
            asm volatile("" : "+r"(key_test_r0));
        } while (!(key_test_r0 & key_value_r1));
    }
    PlaySong(0x41);
    if (*pilot_growth_class_address == PILOT_GROWTH_CLASS_MANUAL) {
        goto block_158;
    }
    goto block_182;
block_158:
    RunMenuScript(0x08003E61);
    {
        register u32 menu_selection_zero_r4 asm("r4") = 0;

        asm volatile("" : "+r"(menu_selection_zero_r4));
        manual_growth_selected_row = menu_selection_zero_r4;
    }
    {
    register u32 clear_index asm("r5") = 0;
    register u8 *clear_base asm("r2");
    register u32 clear_zero asm("r1");
    register u8 *clear_address asm("r0");
    encounter_group_offset_or_growth_points_address = (s32) manual_stat_growth_points;
    clear_base = (u8 *) encounter_group_offset_or_growth_points_address;
    asm volatile("" : "+r"(clear_base));
    clear_zero = 0;
loop_159:
    clear_address = clear_base + clear_index;
    *clear_address = (u8)clear_zero;
    {
        register u32 clear_successor asm("r0") = clear_index + 1;

        clear_successor <<= 24;
        clear_index = clear_successor >> 24;
    }
    if (clear_index <= 5U) {
        goto loop_159;
    }
    }
    {
        register u32 menu_remaining_r5 asm("r5") = PILOT_MANUAL_GROWTH_POINTS;

        asm volatile("" : "+r"(menu_remaining_r5));
        manual_growth_points_remaining = menu_remaining_r5;
    }
    manual_growth_redraw_or_stat_index = 1;
    manual_growth_cursor = CreateSprite(0x080ED830, 0x080ED864, 0, 0x98,
        (s32) ((manual_growth_selected_row << 19) + ({
            register u32 menu_y_base_r4 asm("r4") = 0xA0;

            menu_y_base_r4 <<= 14;
            asm volatile("" : "+r"(menu_y_base_r4));
            menu_y_base_r4;
        })) >> 16,
        0x3EE, 0xF, 0x20,
        ({ register s32 zero asm("r2") = 0; zero; }));
    asm volatile("" : "=g"(menu_frame_anchor));
    ResetMenuKeyRepeat();
    asm volatile("" : "+r"(manual_growth_redraw_or_stat_index));
    outcome_window_or_text_address = manual_growth_redraw_or_stat_index;
    level_up_pilot_slot = (u32) encounter_group_offset_or_growth_points_address;
loop_161:
    menu_row = manual_growth_selected_row << 3;
    asm volatile("" : "+r"(menu_row));
    asm volatile(
        ".macro bne target\n\t"
        ".short 0xD06B\n\t"
        ".endm\n\t"
        ".macro b target\n\t"
        ".endm");
    if (manual_growth_redraw_or_stat_index == 0) {
        goto block_165;
    }
    asm volatile(".purgem bne\n\t.purgem b");
    manual_growth_redraw_or_stat_index = 0;
loop_163:
    PrintWindowNumberAt((s16) ((u8 *) encounter_group_offset_or_growth_points_address)[manual_growth_redraw_or_stat_index], 3, 0, 0xA, outcome_window_or_text_address, 8, manual_growth_redraw_or_stat_index);
    manual_growth_redraw_or_stat_index = (u32) (u8) (manual_growth_redraw_or_stat_index + 1);
    if ((u32) manual_growth_redraw_or_stat_index <= 4U) {
        goto loop_163;
    }
    {
        register u32 stat_offset asm("r2") = 0x34;
        register s32 stat_value asm("r0");
        register u32 menu_value asm("r3");
        register u32 one_view asm("r5");
        register u32 zero_view asm("r2");

        stat_value = *(s16 *)(level_up_pilot + stat_offset);
        menu_value = level_up_pilot_slot;
        asm volatile("" : "+r"(menu_value));
        menu_value = *(u8 *)menu_value;
        asm volatile("" : "+r"(menu_value));
        stat_value += menu_value;
        one_view = outcome_window_or_text_address;
        asm volatile("" : "+r"(one_view));
        PrintWindowNumberAtWide(stat_value, 3, zero_view, 0xA, one_view,
            ({
                register u32 twelve asm("r1") = 0xC;
                asm volatile("" : "+r"(twelve));
                twelve;
            }),
            ({
                zero_view = 0;
                asm volatile("" : "+r"(zero_view));
                zero_view;
            }));
    }
    {
        register s32 stat_value asm("r0");
        register u32 stat_offset asm("r3") = 0x36;
        register u8 *menu_view asm("r5");
        register u32 one_r1 asm("r1");

        stat_value = *(s16 *)(level_up_pilot + stat_offset);
        menu_view = (u8 *) level_up_pilot_slot;
        asm volatile("" : "+r"(menu_view));
        stat_value += menu_view[1];
        one_r1 = outcome_window_or_text_address;
        asm volatile("" : "+r"(one_r1));
        PrintWindowNumberAtWide(stat_value, 3, 0, 0xA, one_r1,
            ({
                register u32 twelve asm("r2") = 0xC;
                asm volatile("" : "+r"(twelve));
                twelve;
            }), one_r1);
        {
            register s32 stat_value asm("r0");
        register u32 stat_offset asm("r3") = 0x3C;
        register u32 menu_value asm("r1");

        asm volatile(
            ".macro mov dst, src\n\t"
            ".short 0x233C\n\t"
            ".endm\n\t"
            ".macro ldrsh dst, addr:vararg\n\t"
            ".short 0x5EF8\n\t"
            ".endm");
        stat_value = *(s16 *)(level_up_pilot + stat_offset);
        asm volatile(".purgem mov\n\t.purgem ldrsh");
            menu_value = menu_view[2];
            asm volatile("" : "+r"(menu_value));
            stat_value += menu_value;
            menu_one_r5 = outcome_window_or_text_address;
            asm volatile("" : "+r"(menu_one_r5));
            PrintWindowNumberAtWide(stat_value, 3, 0, 0xA, menu_one_r5,
                ({
                    register u32 twelve asm("r1") = 0xC;
                    asm volatile("" : "+r"(twelve));
                    twelve;
                }), 2U);
        }
    }
    {
        register s32 stat_value asm("r0");
        register u32 stat_offset asm("r2") = 0x38;
        register u8 *menu_view asm("r3");
        register u32 menu_value asm("r1");
        register u32 three_r1 asm("r1");

        stat_value = *(s16 *)(level_up_pilot + stat_offset);
        menu_view = (u8 *)level_up_pilot_slot;
        asm volatile("" : "+r"(menu_view));
        menu_value = menu_view[3];
        asm volatile("" : "+r"(menu_value));
        stat_value += menu_value;
        PrintWindowNumberAtWide(stat_value, three_r1, 0, 0xA, menu_one_r5,
            ({
                register u32 twelve asm("r5") = 0xC;
                asm volatile("" : "+r"(twelve));
                twelve;
            }),
            ({
                three_r1 = 3;
                asm volatile("" : "+r"(three_r1));
                three_r1;
            }));
    }
    {
        register s32 stat_value asm("r0");
        register u32 stat_offset asm("r2") = 0x3A;
        register u32 menu_value asm("r3");

        asm volatile(
            ".macro mov dst, src\n\t"
            ".short 0x223A\n\t"
            ".endm\n\t"
            ".macro ldrsh dst, addr:vararg\n\t"
            ".short 0x5EB8\n\t"
            ".endm");
        stat_value = *(s16 *)(level_up_pilot + stat_offset);
        asm volatile(".purgem mov\n\t.purgem ldrsh");
        menu_value = level_up_pilot_slot;
        asm volatile("" : "+r"(menu_value));
        menu_value = *(u8 *)(menu_value + 4);
        asm volatile("" : "+r"(menu_value));
        stat_value += menu_value;
        PrintWindowNumberAtWide(stat_value, 3, 0, 0xA, outcome_window_or_text_address,
            ({
                register u32 twelve asm("r1") = 0xC;
                asm volatile("" : "+r"(twelve));
                twelve;
            }), 4U);
    }
    asm volatile(".syntax unified\n\t"
                 "movs r2, #3\n\t"
                 "str r2, [sp]\n\t"
                 "movs r0, #9\n\t"
                 "str r0, [sp, #4]\n\t"
                 "movs r3, #0\n\t"
                 "str r3, [sp, #8]\n\t"
                 "ldr r0, [sp, #100]\n\t"
                 "movs r1, #2\n\t"
                 "movs r2, #0\n\t"
                 "movs r3, #10\n\t"
                 "bl func_0809844C\n\t"
                 ".syntax divided"
                 :
                 :
                 : "r0", "r1", "r2", "r3", "lr", "cc", "memory");
    RequestWindowRefresh();
    manual_growth_redraw_or_stat_index = 0;
block_165:
    asm volatile("" : : "g"(menu_frame_anchor));
    asm volatile(
        ".macro ldr dst, addr:vararg\n\t"
        ".short 0x9C1A\n\t"
        ".endm\n\t"
        ".macro strh src, addr:vararg\n\t"
        ".short 0x80E0\n\t"
        ".endm");
    M2C_FIELD(manual_growth_cursor, s16 *, 6) = (s16) (menu_row + 0x28);
    asm volatile(".purgem ldr\n\t.purgem strh");
    YieldTaskForUpdates(1);
    if (!(0x40 & *(u16 *)0x03006034)) {
        goto block_168;
    }
    {
        u32 selection = manual_growth_selected_row;

        asm volatile("" : "+r"(selection));
        if (selection == 0) {
            goto block_168;
        }
        selection = (u8)(selection - 1);
        manual_growth_selected_row = selection;
    }
    PlaySong(0x40);
block_168:
    if (!(0x80 & *(u16 *)0x03006034)) {
        goto block_171;
    }
    asm volatile(
        ".macro mov dst, src\n\t"
        ".short 0x4651\n\t"
        ".endm\n\t"
        ".macro cmp lhs, rhs\n\t"
        ".short 0x2903\n\t"
        ".endm");
    if ((u32) manual_growth_selected_row > 3U) {
        goto block_171;
    }
    asm volatile(".purgem mov\n\t.purgem cmp");
    manual_growth_selected_row = (u8)(manual_growth_selected_row + 1);
    PlaySong(0x40);
block_171:
    if (!(0x20 & *(u16 *)0x03006034)) {
        goto block_174;
    }
    asm volatile(
        ".macro mov dst, src\n\t"
        ".short 0x4652\n\t"
        ".endm\n\t"
        ".macro add dst, lhs, rhs\n\t"
        ".short 0x18B1\n\t"
        ".endm");
    temp_r1_7 = &((u8 *) encounter_group_offset_or_growth_points_address)[manual_growth_selected_row];
    asm volatile(".purgem mov\n\t.purgem add");
    temp_r0_25 = *temp_r1_7;
    if (temp_r0_25 == 0) {
        goto block_174;
    }
    *temp_r1_7 = temp_r0_25 - 1;
    manual_growth_points_remaining = (s32) (u8) (manual_growth_points_remaining + 1);
    manual_growth_redraw_or_stat_index = 1;
    PlaySong(0x40);
block_174:
    if (!(0x10 & *(u16 *)0x03006034)) {
        goto block_177;
    }
    {
        register u32 remaining_r3 asm("r3") = manual_growth_points_remaining;
        register u32 selection_r4 asm("r4");
        register u32 next_r0 asm("r0");
        register u8 *address_r0 asm("r0");
        register u32 value_r1 asm("r1");

        asm volatile("" : "+r"(remaining_r3));
        if (remaining_r3 == 0) {
            goto block_177;
        }
        selection_r4 = manual_growth_selected_row;
        asm volatile("" : "+r"(selection_r4));
        address_r0 = &((u8 *) encounter_group_offset_or_growth_points_address)[selection_r4];
        asm volatile("" : "+r"(address_r0));
        value_r1 = *address_r0;
        value_r1 += 1;
        *address_r0 = value_r1;
        next_r0 = remaining_r3;
        asm volatile("" : "+r"(next_r0));
        next_r0 = (u8)(next_r0 - 1);
        manual_growth_points_remaining = next_r0;
    }
    manual_growth_redraw_or_stat_index = 1;
    PlaySong(0x40);
block_177:
    asm volatile(
        ".macro ldrh dst, addr:vararg\n\t"
        ".short 0x8801, 0x4648\n\t"
        ".macro mov dst2, src2\n\t"
        ".purgem ldrh\n\t"
        ".purgem mov\n\t"
        ".endm\n\t"
        ".endm");
    if (outcome_window_or_text_address & *(u16 *)0x0300000E) {
        goto block_179;
    }
    goto loop_161;
block_179:
    asm volatile(
        ".macro ldr dst, addr:vararg\n\t"
        ".short 0x9819, 0x2800\n\t"
        ".macro cmp lhs, rhs\n\t"
        ".purgem ldr\n\t"
        ".purgem cmp\n\t"
        ".endm\n\t"
        ".endm");
    if (manual_growth_points_remaining == 0) {
        goto block_181;
    }
    PlaySong(0x58);
    ConfigureDisplayWindows(1, 0, 0, 1, 0x48A8, 0x2858, 0x2B3B, 0x3E);
    RunMenuScript(0x08001C22);
    DisableDisplayWindows();
    {
        register s32 transition_zero_r1 asm("r1") = 0;

        asm volatile("" : "+r"(transition_zero_r1));
        ConfigureDisplayWindows(1, transition_zero_r1, 0, 0,
            transition_zero_r1, transition_zero_r1, 0x3B, 0x3E);
    }
    RequestWindowRefresh();
    goto loop_161;
block_181:
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP])) = (u16) ((u8 *) encounter_group_offset_or_growth_points_address)[0];
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY])) = (u16) ((u8 *) encounter_group_offset_or_growth_points_address)[1];
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP])) = (u16) ((u8 *) encounter_group_offset_or_growth_points_address)[2];
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY])) = (u16) ((u8 *) encounter_group_offset_or_growth_points_address)[3];
    M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY])) = (u16) ((u8 *) encounter_group_offset_or_growth_points_address)[4];
    RunMenuScript(0x08003E90);
    DestroySprite(manual_growth_cursor);
    PlaySong(0x3E);
block_182:
    {
        register u8 *status_r2 asm("r2") = pilot_level_address;
        register u32 updated_r0 asm("r0") = *status_r2;

        updated_r0 += 1;
        *status_r2 = updated_r0;
    }
    {
        register u32 updated_r0 asm("r0") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP]));
        register u32 current_r3 asm("r3") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(max_hp_bonus_percent));

        updated_r0 += current_r3;
        M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(max_hp_bonus_percent)) = updated_r0;
    }
    {
        register u32 updated_r0 asm("r0") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY]));
        register u32 current_r4 asm("r4") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(mobility_bonus_percent));

        updated_r0 += current_r4;
        M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(mobility_bonus_percent)) = updated_r0;
    }
    {
        register u32 updated_r0 asm("r0") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP]));
        register u32 current_r5 asm("r5") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(dcp_bonus_percent));

        updated_r0 += current_r5;
        M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(dcp_bonus_percent)) = updated_r0;
    }
    {
        register u32 updated_r0 asm("r0") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY]));
        register u32 current_r1 asm("r1") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(sensor_accuracy_bonus_percent));

        updated_r0 += current_r1;
        M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(sensor_accuracy_bonus_percent)) = updated_r0;
    }
    {
        register u32 updated_r0 asm("r0") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY]));
        register u32 current_r2 asm("r2") =
            M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(weapon_accuracy_bonus_percent));

        updated_r0 += current_r2;
        M2C_FIELD(level_up_pilot, u16 *, PLAYER_PILOT_OFFSET(weapon_accuracy_bonus_percent)) = updated_r0;
    }
    if ((UpdatePilotAbilities(level_up_pilot, 0) << 0x18) == 0) {
        goto block_194;
    }
    RunMenuScript(0x08003E93);
    learned_ability_index = NULL;
    if ((u32) learned_ability_index >= (u32) *(u8 *)0x02032E20) {
        goto block_191;
    }
    result_buffer_seed = 0x02030564;
    outcome_window_or_text_address = result_buffer_seed;
loop_185:
    {
        register u32 result_successor_r4 asm("r4") =
            (u32)learned_ability_index + 1;

        asm volatile("" : "+r"(result_successor_r4));
        reward_successor = result_successor_r4;
    }
    if (learned_ability_index == NULL) {
        goto block_190;
    }
    if ((ModuloUnsigned32(learned_ability_index, 3) << 0x18) != 0) {
        goto block_190;
    }
    RequestWindowRefresh();
    {
    register volatile u16 *result_keys asm("r4") =
        (volatile u16 *)0x0300000E;
    register u32 result_key_mask asm("r6") = 3;
loop_188:
    YieldTaskForUpdates(1);
    {
    register u32 result_key_value_r1 asm("r1");
    register u32 result_key_test_r0 asm("r0");

    asm volatile("ldrh %0, [%1]"
                 : "=r"(result_key_value_r1)
                 : "r"(result_keys)
                 : "memory");
    asm volatile("add %0, %1, #0"
                 : "=r"(result_key_test_r0)
                 : "r"(result_key_mask));
    if (!(result_key_test_r0 & result_key_value_r1)) {
        goto loop_188;
    }
    }
    }
    PlaySong(0x41);
    ClearWindow(1);
block_190:
    {
        register u32 result_first_r0 asm("r0") = 0x02032E21;
        register u32 result_second_r1 asm("r1");

        asm volatile("add %0, %1, %0"
                     : "+r"(result_first_r0)
                     : "r"(learned_ability_index));
        result_first_r0 = *(u8 *)result_first_r0;
        result_second_r1 = 0x02032E2B;
        asm volatile(".short 0x1869"
                     : "+r"(result_second_r1)
                     : "r"(learned_ability_index));
        result_second_r1 = *(u8 *)result_second_r1;
        FormatPilotAbilityTextWide(
            result_first_r0, result_second_r1, outcome_window_or_text_address);
    }
    PrintWindowTextAt(outcome_window_or_text_address, 0, 1, 0, (u32) (ModuloUnsigned32(learned_ability_index, 3) << 0x18) >> 0x17);
    {
        register u32 result_successor_r5 asm("r5") = reward_successor;
        register u32 result_successor_r0 asm("r0");

        asm volatile("" : "+r"(result_successor_r5));
        result_successor_r0 = result_successor_r5 << 24;
        learned_ability_index = (void *)(result_successor_r0 >> 24);
    }
    if ((u32) learned_ability_index < (u32) *(u8 *)0x02032E20) {
        goto loop_185;
    }
block_191:
    RequestWindowRefresh();
    {
    register volatile u16 *summary_keys asm("r4") =
        (volatile u16 *)0x0300000E;
    register u32 summary_key_mask asm("r5") = 3;
loop_192:
    YieldTaskForUpdates(1);
    {
    register u32 summary_key_value_r1 asm("r1");
    register u32 summary_key_test_r0 asm("r0");

    asm volatile("ldrh %0, [%1]"
                 : "=r"(summary_key_value_r1)
                 : "r"(summary_keys)
                 : "memory");
    asm volatile("add %0, %1, #0"
                 : "=r"(summary_key_test_r0)
                 : "r"(summary_key_mask));
    if (!(summary_key_test_r0 & summary_key_value_r1)) {
        goto loop_192;
    }
    }
    }
    PlaySong(0x41);
block_194:
    {
    register u32 reward_kind_r0 asm("r0") =
        M2C_FIELD(level_up_pilot, u8 *, PLAYER_PILOT_OFFSET(zoid_slot));
    register u32 reward_kind_r1 asm("r1");

    if (reward_kind_r0 == 0) {
        goto block_196;
    }
    asm volatile(".include \"src/unnamed/sub_080C7190_fix8090.inc\""
                 : "=r"(reward_kind_r1)
                 : "r"(reward_kind_r0));
    RecalculateZoidStats((reward_kind_r1 * 0x70) + 0x020218E8, level_up_pilot);
    }
block_196:
    temp_r0_28 = *level_up_pilot;
    if (temp_r0_28 == 1) {
        goto block_198;
    }
    goto block_218;
block_198:
    if (M2C_FIELD(level_up_pilot, u8 *, PLAYER_PILOT_OFFSET(auxiliary_pilot_id)) != 0) {
        goto check_pulse_level_up;
    }
    goto block_218;
check_pulse_level_up:
    player_state_or_pulse_record = (u8 *)0x020218E4;
    asm volatile("" : "+r"(player_state_or_pulse_record));
    if ((u32) *(u8 *)0x02028114 <= 0x62U) {
        goto block_202;
    }
    goto block_218;
block_202:
    *(u8 *)0x02032EF8 = 0x34;
    PlaySong(0x34);
    player_state_or_pulse_record += PLAYER_STATE_OFFSET(auxiliary_pilots[1]);
    RunMenuScript(0x08003EA0);
    ClearWindow(0);
    PrintWindowText(GetPilotDisplayName(0x4BU), 2, 0);
    RunMenuScript(0x08003B1C);
    PrintWindowNumber(*(u8 *)0x02028114 + 1, 2, 1, 0, 0);
    RunMenuScript(0x08003B3E);
    QueuePilotPortraitGraphics(0x4BU, 0, M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)), 0x3C2, 0xE, 0x02002880);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)), 3, 0, 0xA, (s32) temp_r0_28, 4, 0U);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 4, (u32) temp_r0_28);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 4, 2U);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 4, 3U);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 4, 4U);
    {
    register u8 *reward_stat_base asm("r6") = (u8 *)0x087B7988;
    register u32 reward_stat_last_offset asm("r2");
    register u8 *reward_stat_last asm("r8");
    register u32 reward_update_index asm("r1");
    register u32 reward_update_offset asm("r0");
    register u32 reward_update_current asm("r5");
    register u32 reward_second_base asm("r1");
    register u32 reward_second_current asm("r2");
    register u32 reward_third_index asm("r1");
    register u32 reward_third_base asm("r3");
    register u32 reward_third_current asm("r5");
    register u32 reward_fourth_base asm("r1");
    register u32 reward_fourth_current asm("r2");
    register u32 reward_fifth_index asm("r1");
    register u32 reward_fifth_current asm("r3");
    register u32 reward_display_fifth_index asm("r1");
    register u32 reward_display_fifth_offset asm("r0");
    PrintWindowNumberAt(M2C_FIELD((M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)) * 0xA), s16 *, (u32)reward_stat_base), 3, 0, 0xA, (s32) temp_r0_28, 8, 0U);
    PrintWindowNumberAt(M2C_FIELD((M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)) * 0xA), s16 *, 0x087B798A), 3, 0, 0xA, (s32) temp_r0_28, 8, (u32) temp_r0_28);
    PrintWindowNumberAt(M2C_FIELD((M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)) * 0xA), s16 *, 0x087B798C), 3, 0, 0xA, (s32) temp_r0_28, 8, 2U);
    PrintWindowNumberAt(M2C_FIELD((M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant)) * 0xA), s16 *, 0x087B798E), 3, 0, 0xA, (s32) temp_r0_28, 8, 3U);
    reward_display_fifth_index = M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant));
    reward_display_fifth_offset = reward_display_fifth_index << 2;
    reward_display_fifth_offset += reward_display_fifth_index;
    reward_display_fifth_offset <<= 1;
    reward_stat_last_offset = 8;
    reward_stat_last_offset += (u32)reward_stat_base;
    reward_stat_last = (u8 *)reward_stat_last_offset;
    reward_display_fifth_offset += (u32)reward_stat_last;
    PrintWindowNumberAt(*(s16 *)reward_display_fifth_offset, 3, 0, 0xA,
        (s32) temp_r0_28, 8, 4U);
    *(u8 *)0x02028114 = (u8) (*(u8 *)0x02028114 + 1);
    reward_update_index = M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant));
    reward_update_offset = reward_update_index << 2;
    reward_update_offset += reward_update_index;
    reward_update_offset <<= 1;
    reward_update_offset += (u32)reward_stat_base;
    reward_update_offset = *(u16 *)reward_update_offset;
    reward_update_current = M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent));
    reward_update_offset += reward_update_current;
    M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)) = (u16)reward_update_offset;
    temp_r1_8 = reward_update_index;
    reward_update_offset = temp_r1_8 << 2;
    reward_update_offset += temp_r1_8;
    reward_update_offset <<= 1;
    reward_second_base = 0x087B798A;
    asm volatile("add %0, %0, %1"
                 : "+r"(reward_update_offset)
                 : "r"(reward_second_base));
    reward_update_offset = *(u16 *)reward_update_offset;
    reward_second_current = M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent));
    reward_update_offset += reward_second_current;
    M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)) = (u16)reward_update_offset;
    reward_third_index = M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant));
    reward_update_offset = reward_third_index << 2;
    reward_update_offset += reward_third_index;
    reward_update_offset <<= 1;
    reward_third_base = 0x087B798C;
    asm volatile("add %0, %0, %1"
                 : "+r"(reward_update_offset)
                 : "r"(reward_third_base));
    reward_update_offset = *(u16 *)reward_update_offset;
    reward_third_current = M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent));
    reward_update_offset += reward_third_current;
    M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)) = (u16)reward_update_offset;
    reward_update_offset = reward_third_index << 2;
    reward_update_offset += reward_third_index;
    reward_update_offset <<= 1;
    reward_fourth_base = 0x087B798E;
    asm volatile("add %0, %0, %1"
                 : "+r"(reward_update_offset)
                 : "r"(reward_fourth_base));
    reward_update_offset = *(u16 *)reward_update_offset;
    reward_fourth_current = M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent));
    reward_update_offset += reward_fourth_current;
    M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)) = (u16)reward_update_offset;
    reward_fifth_index = M2C_FIELD(player_state_or_pulse_record, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(palette_or_portrait_variant));
    reward_update_offset = reward_fifth_index << 2;
    reward_update_offset += reward_fifth_index;
    reward_update_offset <<= 1;
    reward_update_offset += (u32)reward_stat_last;
    reward_update_offset = *(u16 *)reward_update_offset;
    reward_fifth_current = M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent));
    reward_update_offset += reward_fifth_current;
    M2C_FIELD(player_state_or_pulse_record, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent)) = (u16)reward_update_offset;
    }
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)), 3, 0, 0xA, (s32) temp_r0_28, 0xC, 0U);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 0xC, (u32) temp_r0_28);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 0xC, 2U);
    PrintWindowNumberAt(M2C_FIELD(player_state_or_pulse_record, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)), 3, 0, 0xA, (s32) temp_r0_28, 0xC, 3U);
    {
        register s32 final_reward_value_r0 asm("r0");

        asm volatile(".include \"src/unnamed/sub_080C7190_fix8310.inc\"\n\t"
                     : "=r"(final_reward_value_r0)
                     : "r"(player_state_or_pulse_record));
        PrintWindowNumberAtWide(final_reward_value_r0, 3, 0, 0xA,
            (s32) temp_r0_28, 0xC, 4U);
    }
    RequestWindowRefresh();
    {
    register volatile u16 *reward_summary_keys asm("r4") =
        (volatile u16 *)0x0300000E;
    register u32 reward_summary_mask asm("r5") = 3;
loop_203:
    YieldTaskForUpdates(1);
    {
    register u32 reward_summary_value_r1 asm("r1");
    register u32 reward_summary_test_r0 asm("r0");

    asm volatile("ldrh %0, [%1]"
                 : "=r"(reward_summary_value_r1)
                 : "r"(reward_summary_keys)
                 : "memory");
    asm volatile("add %0, %1, #0"
                 : "=r"(reward_summary_test_r0)
                 : "r"(reward_summary_mask));
    if (!(reward_summary_test_r0 & reward_summary_value_r1)) {
        goto loop_203;
    }
    }
    }
    PlaySong(0x41);
    if ((UpdatePulseEffectsFromEmotionGrowth() << 0x18) == 0) {
        goto block_218;
    }
    RunMenuScript(0x08003E93);
    learned_pulse_effect_index = NULL;
    if ((u32) learned_pulse_effect_index >= (u32) *(u8 *)0x02032E35) {
        goto block_215;
    }
    reward_table_seed = 0x087EF410;
    asm volatile("" : "+r"(reward_table_seed));
    outcome_window_or_text_address = reward_table_seed;
loop_209:
    {
        register u32 reward_successor_r1 asm("r1") =
            (u32)learned_pulse_effect_index + 1;

        asm volatile("" : "+r"(reward_successor_r1));
        reward_successor = reward_successor_r1;
    }
    if (learned_pulse_effect_index == NULL) {
        goto block_214;
    }
    if ((ModuloUnsigned32(learned_pulse_effect_index, 3) << 0x18) != 0) {
        goto block_214;
    }
    RequestWindowRefresh();
    {
    register volatile u16 *reward_keys asm("r4") =
        (volatile u16 *)0x0300000E;
    register u32 reward_key_mask asm("r6") = 3;
    register u32 reward_key_value_r1 asm("r1");
    register u32 reward_key_test_r0 asm("r0");
loop_212:
    YieldTaskForUpdates(1);
    asm volatile("ldrh %0, [%1]"
                 : "=r"(reward_key_value_r1)
                 : "r"(reward_keys)
                 : "memory");
    reward_key_test_r0 = reward_key_mask;
    asm volatile("" : "+r"(reward_key_test_r0));
    if (!(reward_key_test_r0 & reward_key_value_r1)) {
        goto loop_212;
    }
    }
    PlaySong(0x41);
    ClearWindow(1);
block_214:
    {
        register u32 reward_index_r0 asm("r0") = 0x02032E36;

        asm volatile("" : "+r"(reward_index_r0));
        reward_index_r0 = (u32)learned_pulse_effect_index + reward_index_r0;
        reward_index_r0 = *(u8 *)reward_index_r0;
        temp_r4_6 = *(s32 *)((reward_index_r0 * 4) + outcome_window_or_text_address);
    }
    PrintWindowTextAt(temp_r4_6, 0, 1, 0, (u32) (ModuloUnsigned32(learned_pulse_effect_index, 3) << 0x18) >> 0x17);
    {
        register u32 reward_successor_r2 asm("r2") = reward_successor;
        register u32 reward_successor_r0 asm("r0");

        asm volatile("" : "+r"(reward_successor_r2));
        reward_successor_r0 = reward_successor_r2 << 24;
        learned_pulse_effect_index = (void *)(reward_successor_r0 >> 24);
    }
    if ((u32) learned_pulse_effect_index < (u32) *(u8 *)0x02032E35) {
        goto loop_209;
    }
block_215:
    RequestWindowRefresh();
    {
    register volatile u16 *final_keys asm("r4") =
        (volatile u16 *)0x0300000E;
    register u32 final_key_mask asm("r5") = 3;
    register u32 final_key_value_r1 asm("r1");
    register u32 final_key_test_r0 asm("r0");
loop_216:
    YieldTaskForUpdates(1);
    asm volatile("ldrh %0, [%1]"
                 : "=r"(final_key_value_r1)
                 : "r"(final_keys)
                 : "memory");
    final_key_test_r0 = final_key_mask;
    asm volatile("" : "+r"(final_key_test_r0));
    if (!(final_key_test_r0 & final_key_value_r1)) {
        goto loop_216;
    }
    }
    PlaySong(0x41);
block_218:
    {
    register u32 final_table_value asm("r1") = PILOT_EXPERIENCE_THRESHOLDS_ROM;
    register u8 *final_status_ptr asm("r3") = pilot_level_address;
    register u32 final_table_index asm("r0");

    asm volatile("" : "+r"(final_table_value));
    final_table_index = *final_status_ptr;
    final_table_index <<= 2;
    final_table_index += final_table_value;
    final_table_value = *(u32 *)final_table_index;
    temp_r1_10 = final_table_value;
    }
    if (temp_r1_10 == 0) {
        goto block_220;
    }
    if ((u32) M2C_FIELD(level_up_pilot, u32 *, PLAYER_PILOT_OFFSET(experience)) >= temp_r1_10) {
        goto apply_next_pilot_level_up;
    }
block_220:
    {
        register u32 final_owner_r4 asm("r4") = next_unit_or_pilot_slot;
        register u32 final_owner_r0 asm("r0");

        asm volatile("" : "+r"(final_owner_r4));
        final_owner_r0 = final_owner_r4 << 24;
        final_owner_r0 >>= 24;
        level_up_pilot_slot = final_owner_r0;
        if (final_owner_r0 <= 0x34U) {
            goto scan_pilot_level_ups;
        }
    }
    return;
block_223:
    tail_status_offset = 0x5A95;
    asm volatile("" : "+r"(tail_status_offset));
    temp_r4_7 += tail_status_offset;
    tail_status = *temp_r4_7;
    tail_data_offset = (tail_status << 3) - tail_status;
    tail_data_offset <<= 4;
    tail_data_base = tail_base + 4;
    tail_data_offset += (u32)tail_data_base;
    RestoreDestroyedZoidHp(tail_data_offset);
    AssignZoidToPlayerTeam(1, *temp_r4_7);
    goto block_228;
restore_party_after_defeat:
    RunMenuScript(0x08003C58);
    tail_seed = 1;
    asm volatile("" : "+r"(tail_seed));
    defeat_protagonist_scan_slot = tail_seed;
    tail_base = (u8 *)0x020218E4;
    tail_offset = 0x5A94;
    asm volatile(".include \"src/unnamed/sub_080C7190_fix84B6.inc\""
                 : "+r"(tail_base), "+r"(tail_offset));
loop_226:
    tail_record_offset = defeat_protagonist_scan_slot << 6;
    asm volatile("add %0, %1, %2"
                 : "=r"(temp_r4_7)
                 : "r"(tail_record_offset), "r"(tail_base));
    if (M2C_FIELD(temp_r4_7, u8 *, tail_offset) == 1) {
        goto block_223;
    }
    temp_r0_30 = defeat_protagonist_scan_slot + 1;
    defeat_protagonist_scan_slot = temp_r0_30;
    if ((u32) temp_r0_30 <= 0x34U) {
        goto loop_226;
    }
block_228:
    if ((TestEventFlag(2) << 0x18) != 0) {
        goto outcome_done;
    }
    tail2_seed = 1;
    asm volatile("" : "+r"(tail2_seed));
    defeat_juno_scan_slot = tail2_seed;
    tail2_base = (u8 *)0x020218E4;
    asm volatile("" : "+r"(tail2_base));
loop_230:
    tail2_index = defeat_juno_scan_slot;
    asm volatile("" : "+r"(tail2_index));
    tail2_record = tail2_index << 6;
    tail2_record += (u32)tail2_base;
    tail2_offset = 0x5A94;
    asm volatile("" : "+r"(tail2_offset));
    tail2_record += tail2_offset;
    if (*(u8 *)tail2_record != 0x60) {
        goto block_232;
    }
    RemovePlayerPilotAndTemporaryZoidWide(defeat_juno_scan_slot);
block_232:
    temp_r0_31 = defeat_juno_scan_slot + 1;
    defeat_juno_scan_slot = temp_r0_31;
    if ((u32) temp_r0_31 <= 0x34U) {
        goto loop_230;
    }
    return;
present_retreat_result:
    PlaySong(0x53);
    RunMenuScript(0x08003C88);
outcome_done:
    return;
}
