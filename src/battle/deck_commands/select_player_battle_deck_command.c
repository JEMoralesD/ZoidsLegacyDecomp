#include "m2c_prelude.h"
#include "deck_commands.h"
#include "../combinations/battle_combination.h"

s32 PlaySong(s32) asm("func_08092E84");                             /* extern */
s32 RequestWindowRefresh() asm("func_080972C8");                                /* extern */
s32 PrintWindowTextAt(s32, s32, s32, s32, s32) asm("func_080981F0");         /* extern */
s32 PrintWindowText(s32, s32, s32) asm("func_08098248");                   /* extern */
s32 OpenWindow(s32, s32, s32, s32, u32, s32) asm("func_08098514");    /* extern */
s32 CloseWindow(s32) asm("func_08098754");                             /* extern */
s32 RunMenuScript(s32) asm("func_08098BB4");                             /* extern */
s32 AppendSelectedDeckCommandRows(s32) asm("func_080C0930");                             /* extern */
s32 ShowDeckCommandDescription(u8) asm("func_080C0A9C");                              /* extern */
u8 IsBattleCombinationFormationValid(s32, s32, s32) asm("func_080C0C54");                    /* extern */
s32 TestBattleRuleFlag(s32) asm("func_080E6664");                             /* extern */
void *AcquireEquipmentStatBuffer() asm("func_080E669C");                              /* extern */
s32 ReleaseEquipmentStatBuffer() asm("func_080E66B8");                                /* extern */
s32 FindAbilityValue(s32, s32, s32) asm("func_080E74F0");                   /* extern */
s32 GetPilotDisplayName(s32) asm("func_080E7B64");                             /* extern */
s32 BuildBattleEquipmentStats(s32, u32, u8, s32, void *) asm("func_080E8C90");       /* extern */
s32 IsBattleUnitActive(s32, u32) asm("func_080E9D88");                        /* extern */
s32 YieldTaskForUpdates(s32) asm("func_080ED17C");                             /* extern */
M2C_UNK jtbl_080C148C();                            /* static */
extern u8 gCannonGravityGunEquipmentOffset0[] asm("D_000002C2");
extern u8 gCannonGravityGunEquipmentOffset1[] asm("D_000002C6");
extern u8 gCannonGravityGunEquipmentOffset2[] asm("D_000002CA");


s32 SelectPlayerBattleDeckCommand(void) asm("func_080C1414");

s32 SelectPlayerBattleDeckCommand(void) {
    s32 unused_helper_result;
    s32 selection_succeeded;
    u32 command_dispatch_index;
    u32 fiona_unit_slot;
    u32 juno_prayer_protagonist_slot;
    u32 juno_unit_slot;
    u32 airraid_strategy_unit_slot;
    u32 phalanx_unit_slot;
    u32 protagonist_slot;
    u32 brave_protagonist_slot;
    u32 rear_unit_slot;
    u32 strategy_1_unit_slot;
    u32 strategy_3_unit_slot;
    u32 strategy_2_unit_slot;
    u32 van_unit_slot;
    u8 menu_result;
    u8 command_id;
    u8 van_pilot_id;
    u8 fiona_pilot_id;
    u8 juno_pilot_id;
    u8 cannon_model_id;
    u8 fuzor_formation_valid;
    u8 chimera_formation_valid;
    u8 gojulox_formation_valid;
    u8 griffin_formation_valid;
    u8 killer_spiner_formation_valid;
    u8 giga_cannon_formation_valid;
    u8 lord_gale_formation_valid;
    register u32 destroyed_unit_slot asm("r4");
    u8 phalanx_equipment_slot;
    register u8 *selected_deck_base asm("r0");
    register u8 *selected_deck_menu_index asm("r1");
    register u32 selected_deck_offset asm("r2");
    register u32 selected_deck_entry_address asm("r1");
    register u8 *selection_battle_state asm("r1");
    register u8 *selection_menu_index asm("r2");
    register u32 selection_deck_offset asm("r3");
    register u32 selection_deck_entry_address asm("r0");
    register u32 selected_action_command_offset asm("r2");
    register s32 unit_offset_or_pilot_address asm("r0");
    register s32 pilot_records_base asm("r1");
    register u8 *cannon_base asm("r4");
    register u32 cannon_offset asm("r3");
    register u8 *cannon_addr asm("r0");
    register u32 cannon_status_offset asm("r1");
    register u8 *airraid_battle_setup asm("r1");
    register u8 *strategy_battle_setup asm("r0");
    register struct EquipmentRecord *equipment_stats_carrier asm("r2");
    register u32 equipment_attributes_or_unit_flags asm("r1");
    register u32 equipment_or_unit_mask asm("r0");
    register u8 *revival_battle_units asm("r2");
    register u32 destroyed_flag asm("r3");
    register u8 *revival_unit_record asm("r1");
    u8 *protagonist_battle_units;
    u8 *van_battle_units;
    u8 *fiona_battle_units;
    u32 phalanx_row_offset;
    u8 *phalanx_records;
    register struct EquipmentRecord *equipment_stats asm("r8");

    equipment_stats = AcquireEquipmentStatBuffer();
    RunMenuScript(DECK_COMMAND_BATTLE_SELECT_OPEN_SCRIPT);
    RunMenuScript(DECK_COMMAND_BATTLE_SELECT_LIST_SCRIPT);
    AppendSelectedDeckCommandRows(DECK_COMMAND_BATTLE_LIST);
select_command:
    RunMenuScript(DECK_COMMAND_BATTLE_SELECT_SCRIPT);
    menu_result = *(u8 *)0x0200A882;
    if (menu_result == DECK_COMMAND_MENU_CONFIRMED) {
        selected_deck_base = (u8 *)0x02034B4C;
        asm volatile("" : "+r"(selected_deck_base));
        selected_deck_menu_index = (u8 *)0x0200A880;
        asm volatile("" : "+r"(selected_deck_menu_index));
        selected_deck_offset = BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands);
        asm volatile("" : "+r"(selected_deck_offset));
        selected_deck_base += selected_deck_offset;
        asm volatile("" : "+r"(selected_deck_base));
        selected_deck_entry_address = *selected_deck_menu_index;
        selected_deck_entry_address = (u32)selected_deck_base + selected_deck_entry_address;
        command_id = *(u8 *)selected_deck_entry_address;
        if (command_id != 0) {
            command_dispatch_index = command_id - 4;
            switch (command_dispatch_index) {
            case DECK_COMMAND_KINGS_WAY - 4:
            case DECK_COMMAND_GODS_TERRITORY - 4:
                protagonist_slot = 0;
                protagonist_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(protagonist_battle_units));
                while (protagonist_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if (((IsBattleUnitActive(0, protagonist_slot) << 0x18) != 0) &&
                        (M2C_FIELD(((protagonist_slot * (s32)sizeof(struct BattleUnit)) + BATTLE_UNIT_OFFSET(pilot_id)), u8 *,
                                   (u32)protagonist_battle_units) == PLAYER_PROTAGONIST_PILOT_ID)) {
                        break;
                    }
                    protagonist_slot = (u32) (u8) (protagonist_slot + 1);
                }
                if (protagonist_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 4, 5, 0x16, protagonist_slot, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_PILOT_TEXT, 0, 1);
                    PrintWindowTextAt(GetPilotDisplayName(PLAYER_PROTAGONIST_PILOT_ID), 0, 1, 0xC, 0);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            default:
                goto accept_command;
            case DECK_COMMAND_THE_BRAVE - 4:
                brave_protagonist_slot = 0;
                protagonist_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(protagonist_battle_units));
                while (brave_protagonist_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if (((IsBattleUnitActive(0, brave_protagonist_slot) << 0x18) != 0) &&
                        (M2C_FIELD(((brave_protagonist_slot * (s32)sizeof(struct BattleUnit)) + BATTLE_UNIT_OFFSET(pilot_id)), u8 *,
                                   (u32)protagonist_battle_units) == PLAYER_PROTAGONIST_PILOT_ID)) {
                        break;
                    }
                    brave_protagonist_slot = (u32) (u8) (brave_protagonist_slot + 1);
                }
                if (brave_protagonist_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 4, 5, 0x16, brave_protagonist_slot, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_PILOT_TEXT, 0, 1);
                    PrintWindowTextAt(GetPilotDisplayName(PLAYER_PROTAGONIST_PILOT_ID), 0, 1, 0xC, 0);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                destroyed_unit_slot = 0;
                revival_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(revival_battle_units));
                destroyed_flag = BATTLE_UNIT_DESTROYED;
                asm volatile("" : "+r"(destroyed_flag));
find_destroyed_unit:
                unit_offset_or_pilot_address = destroyed_unit_slot * (s32)sizeof(struct BattleUnit);
                asm volatile("" : "+r"(unit_offset_or_pilot_address));
                revival_unit_record = (u8 *)((u32)unit_offset_or_pilot_address + (u32)revival_battle_units);
                asm volatile("" : "+r"(revival_unit_record));
                if ((*revival_unit_record == 0) || !({ equipment_attributes_or_unit_flags = *(u16 *)(revival_unit_record + BATTLE_UNIT_OFFSET(flags)); asm volatile("" : "+r"(equipment_attributes_or_unit_flags)); equipment_or_unit_mask = destroyed_flag; asm volatile("" : "+r"(equipment_or_unit_mask)); equipment_or_unit_mask & equipment_attributes_or_unit_flags; })) {
                    unit_offset_or_pilot_address = destroyed_unit_slot + 1;
                    asm volatile("" : "+r"(unit_offset_or_pilot_address));
                    destroyed_unit_slot = (u32)(u8)unit_offset_or_pilot_address;
                    if ((u32) destroyed_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                        goto find_destroyed_unit;
                    }
                }
                if (destroyed_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 6, 4, 0x12, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_DESTROYED_ZOID_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_DESTROYED_ZOID_NOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_LOGISTICS_SUPPORT - 4:
                rear_unit_slot = 3;
                while (rear_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if ((IsBattleUnitActive(0, rear_unit_slot) << 0x18) != 0) {
                        break;
                    }
                    rear_unit_slot = (u32) (u8) (rear_unit_slot + 1);
                }
                if (rear_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 6, 4, 0x12, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_REAR_ZOID_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_NO_REAR_ZOID_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_COVERING_FIRE - 4:
            case DECK_COMMAND_DEFEND_OR_DIE - 4:
            case DECK_COMMAND_LINK_SUPPORT - 4:
                strategy_1_unit_slot = 0;
                while (strategy_1_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if (((IsBattleUnitActive(0, strategy_1_unit_slot) << 0x18) != 0) &&
                        ((FindAbilityValue((strategy_1_unit_slot * (s32)sizeof(struct BattleUnit)) + (0x02034B4C + BATTLE_UNIT_OFFSET(pilot_id)),
                                       PILOT_ABILITY_STRATEGY_COMMAND_1, 0) << 0x10) != 0)) {
                        break;
                    }
                    strategy_1_unit_slot = (u32) (u8) (strategy_1_unit_slot + 1);
                }
                if (strategy_1_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 6, 4, 0x12, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_STRATEGY_1_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_PILOT_NOT_IN_TROOP_PERIOD_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                strategy_battle_setup = (u8 *)0x0203055C;
                asm volatile("" : "+r"(strategy_battle_setup));
                if ((u32) (u8) (strategy_battle_setup[5] - 7) <= 2U) {
                    OpenWindow(1, 7, 4, 0x10, 8U, 0);
                    PrintWindowText(DECK_COMMAND_ARENA_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_ARENA_IN_PROGRESS_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_STRATEGY_MEET - 4:
            case DECK_COMMAND_DISTURBED_DATA - 4:
                strategy_3_unit_slot = 0;
                while (strategy_3_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if ((IsBattleUnitActive(0, strategy_3_unit_slot) << 0x18) != 0) {
                        unit_offset_or_pilot_address = strategy_3_unit_slot * (s32)sizeof(struct BattleUnit);
                        asm volatile("" : "+r"(unit_offset_or_pilot_address));
                        pilot_records_base = 0x02034B4C + BATTLE_UNIT_OFFSET(pilot_id);
                        asm volatile("" : "+r"(pilot_records_base));
                        unit_offset_or_pilot_address += pilot_records_base;
                        asm volatile("" : "+r"(unit_offset_or_pilot_address));
                        if ((FindAbilityValue(unit_offset_or_pilot_address, PILOT_ABILITY_STRATEGY_COMMAND_3, 0) << 0x10) != 0) {
                            break;
                        }
                    }
                    strategy_3_unit_slot = (u32) (u8) (strategy_3_unit_slot + 1);
                }
                if (strategy_3_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 6, 4, 0x12, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_STRATEGY_3_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_PILOT_NOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_SWITCH - 4:
            case DECK_COMMAND_REDISTRIBUTION - 4:
            case DECK_COMMAND_PARTS_REMOVAL - 4:
                strategy_2_unit_slot = 0;
                while (strategy_2_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if ((IsBattleUnitActive(0, strategy_2_unit_slot) << 0x18) != 0) {
                        unit_offset_or_pilot_address = strategy_2_unit_slot * (s32)sizeof(struct BattleUnit);
                        asm volatile("" : "+r"(unit_offset_or_pilot_address));
                        pilot_records_base = 0x02034B4C + BATTLE_UNIT_OFFSET(pilot_id);
                        asm volatile("" : "+r"(pilot_records_base));
                        unit_offset_or_pilot_address += pilot_records_base;
                        asm volatile("" : "+r"(unit_offset_or_pilot_address));
                        if ((FindAbilityValue(unit_offset_or_pilot_address, PILOT_ABILITY_STRATEGY_COMMAND_2, 0) << 0x10) != 0) {
                            break;
                        }
                    }
                    strategy_2_unit_slot = (u32) (u8) (strategy_2_unit_slot + 1);
                }
                if (strategy_2_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 6, 4, 0x12, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_STRATEGY_2_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_PILOT_NOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_FIONAS_PRAYER - 4:
                van_unit_slot = 0;
                van_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(van_battle_units));
find_van:
                if (((IsBattleUnitActive(0, van_unit_slot) << 0x18) == 0) || ((van_pilot_id = M2C_FIELD(((van_unit_slot * (s32)sizeof(struct BattleUnit)) + BATTLE_UNIT_OFFSET(pilot_id)), u8 *, (u32)van_battle_units), (van_pilot_id != DECK_COMMAND_VAN_PILOT_1)) && (van_pilot_id != DECK_COMMAND_VAN_PILOT_2))) {
                    van_unit_slot = (u32) (u8) (van_unit_slot + 1);
                    if (van_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                        goto find_van;
                    }
                }
                if (van_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 4, 5, 0x16, van_unit_slot, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_VAN_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_VAN_NOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                fiona_unit_slot = 0;
                fiona_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(fiona_battle_units));
find_fiona:
                if (((IsBattleUnitActive(0, fiona_unit_slot) << 0x18) == 0) || ((fiona_pilot_id = M2C_FIELD(((fiona_unit_slot * (s32)sizeof(struct BattleUnit)) + BATTLE_UNIT_OFFSET(pilot_id)), u8 *, (u32)fiona_battle_units), (fiona_pilot_id != DECK_COMMAND_FIONA_PILOT_1)) && (fiona_pilot_id != DECK_COMMAND_FIONA_PILOT_2))) {
                    fiona_unit_slot = (u32) (u8) (fiona_unit_slot + 1);
                    if (fiona_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                        goto find_fiona;
                    }
                }
                if (fiona_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    OpenWindow(1, 5, 5, 0x14, 6U, 0);
                    PrintWindowText(DECK_COMMAND_FIONA_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_PILOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                if ((TestBattleRuleFlag(BATTLE_RULE_NO_ORGANOID_OR_ZOS) << 0x18) != 0) {
                    OpenWindow(1, 7, 5, 0x10, 6U, 0);
                    PrintWindowText(DECK_COMMAND_RULE_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_RULE_PROHIBITS_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_JUNOS_PRAYER - 4:
                juno_prayer_protagonist_slot = 0;
                protagonist_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(protagonist_battle_units));
                while (juno_prayer_protagonist_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if (((IsBattleUnitActive(0, juno_prayer_protagonist_slot) << 0x18) != 0) &&
                        (M2C_FIELD(((juno_prayer_protagonist_slot * (s32)sizeof(struct BattleUnit)) + BATTLE_UNIT_OFFSET(pilot_id)), u8 *,
                                   (u32)protagonist_battle_units) == PLAYER_PROTAGONIST_PILOT_ID)) {
                        break;
                    }
                    juno_prayer_protagonist_slot = (u32) (u8) (juno_prayer_protagonist_slot + 1);
                }
                if (juno_prayer_protagonist_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 4, 5, 0x16, juno_prayer_protagonist_slot, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_PILOT_TEXT, 0, 1);
                    PrintWindowTextAt(GetPilotDisplayName(PLAYER_PROTAGONIST_PILOT_ID), 0, 1, 0xC, 0);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                juno_unit_slot = 0;
                protagonist_battle_units = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(protagonist_battle_units));
find_juno:
                if (((IsBattleUnitActive(0, juno_unit_slot) << 0x18) == 0) || ((juno_pilot_id = M2C_FIELD(((juno_unit_slot * (s32)sizeof(struct BattleUnit)) + BATTLE_UNIT_OFFSET(pilot_id)), u8 *, (u32)protagonist_battle_units), (juno_pilot_id != DECK_COMMAND_JUNO_PILOT_1)) && (juno_pilot_id != DECK_COMMAND_JUNO_PILOT_2) && (juno_pilot_id != DECK_COMMAND_JUNO_PILOT_3))) {
                    juno_unit_slot = (u32) (u8) (juno_unit_slot + 1);
                    if (juno_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                        goto find_juno;
                    }
                }
                if (juno_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    OpenWindow(1, 6, 5, 0x12, 6U, 0);
                    PrintWindowText(DECK_COMMAND_JUNO_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_PILOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                if ((TestBattleRuleFlag(BATTLE_RULE_NO_ORGANOID_OR_ZOS) << 0x18) != 0) {
                    OpenWindow(1, 7, 5, 0x10, 6U, 0);
                    PrintWindowText(DECK_COMMAND_RULE_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_RULE_PROHIBITS_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            case DECK_COMMAND_TWO_ARM_LIZARD_GATTAI - 4: {
                register u32 two_arm_formation_valid asm("r1");

                if (((IsBattleCombinationFormationValid(BATTLE_COMBINATION_TWO_ARM_LIZARD, 0, 0) << 0x18) == 0) && ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_TWO_ARM_LIZARD, 0, 1) << 0x18) == 0) && ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_TWO_ARM_LIZARD, 0, 3) << 0x18) == 0)) {
                    two_arm_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_TWO_ARM_LIZARD, 0, 4);
                    if (two_arm_formation_valid != 0) {
                        asm volatile(".short (0xE000 | (((.Lsub_080C1414_block252 - . - 4) >> 1) & 0x7FF))");
                    }
                    OpenWindow(1, 3, 5, 0x18, 6U, (s32) two_arm_formation_valid);
                    PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                goto accept_command;
            }
            case DECK_COMMAND_FUZOR_DRAGON_GATTAI - 4:
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_FUZOR_DRAGON, 0, 0) << 0x18) != 0) {
                    goto accept_command;
                }
                fuzor_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_FUZOR_DRAGON, 0, 1);
                if (fuzor_formation_valid != 0) {
                    goto accept_command;
                }
                OpenWindow(1, 3, 5, 0x18, 6U, (s32) fuzor_formation_valid);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_CHIMERA_DRAGON_GATTAI - 4:
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_CHIMERA_DRAGON, 0, 0) << 0x18) != 0) {
                    goto accept_command;
                }
                chimera_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_CHIMERA_DRAGON, 0, 1);
                if (chimera_formation_valid != 0) {
                    goto accept_command;
                }
                OpenWindow(1, 3, 5, 0x18, 6U, (s32) chimera_formation_valid);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_GOJULOX_GATTAI - 4:
                gojulox_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_GOJULOX, 0, 0);
                if (gojulox_formation_valid != 0) {
                    goto accept_command;
                }
                OpenWindow(1, 3, 5, 0x18, 6U, (s32) gojulox_formation_valid);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_GRIFFIN_GATTAI - 4:
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_GRIFFIN, 0, 3) << 0x18) != 0) {
                    goto accept_command;
                }
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_GRIFFIN, 0, 4) << 0x18) != 0) {
                    goto accept_command;
                }
                griffin_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_GRIFFIN, 0, 5);
                if (griffin_formation_valid != 0) {
                    goto accept_command;
                }
                OpenWindow(1, 3, 5, 0x18, 6U, (s32) griffin_formation_valid);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_AIRRAID - 4:
                airraid_strategy_unit_slot = 0;
                while (airraid_strategy_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    if (((IsBattleUnitActive(0, airraid_strategy_unit_slot) << 0x18) != 0) &&
                        ((FindAbilityValue((airraid_strategy_unit_slot * (s32)sizeof(struct BattleUnit)) + (0x02034B4C + BATTLE_UNIT_OFFSET(pilot_id)),
                                       PILOT_ABILITY_STRATEGY_COMMAND_1, 0) << 0x10) != 0)) {
                        break;
                    }
                    airraid_strategy_unit_slot = (u32) (u8) (airraid_strategy_unit_slot + 1);
                }
                if (airraid_strategy_unit_slot == BATTLE_ACTIVE_UNIT_COUNT) {
                    OpenWindow(1, 6, 4, 0x12, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_STRATEGY_1_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_PILOT_NOT_IN_TROOP_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                airraid_battle_setup = (u8 *)0x0203055C;
                asm volatile("" : "+r"(airraid_battle_setup));
                if ((u32) (u8) (airraid_battle_setup[5] - 7) <= 2U) {
                    OpenWindow(1, 7, 4, 0x10, 8U, 0);
                    PrintWindowText(DECK_COMMAND_ARENA_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_ARENA_IN_PROGRESS_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                    goto reject_command;
                }
                if (((u32) airraid_battle_setup[1] <= 0xDU) && (*(u8 *)0x02030664 == 0)) {
                    goto accept_command;
                }
                OpenWindow(1, 5, 5, 0x14, 6U, 0);
                PrintWindowText(DECK_COMMAND_WHALE_KING_UNAVAILABLE_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_WHALE_KING_NOT_RESPONDING_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_KILLER_DOME_GATTAI - 4:
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_KILLER_SPINER, 0, 0) << 0x18) != 0) {
                    goto accept_command;
                }
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_KILLER_SPINER, 0, 1) << 0x18) != 0) {
                    goto accept_command;
                }
                killer_spiner_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_KILLER_SPINER, 0, 2);
                if (killer_spiner_formation_valid != 0) {
                    goto accept_command;
                }
                OpenWindow(1, 3, 5, 0x18, 6U, (s32) killer_spiner_formation_valid);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_ARROW_PHALANX - 4:
                if (((IsBattleUnitActive(0, 1U) << 0x18) != 0) && ((IsBattleUnitActive(0, 3U) << 0x18) != 0) && ((IsBattleUnitActive(0, 5U) << 0x18) != 0)) {
                    goto accept_command;
                }
                OpenWindow(1, 3, 5, 0x18, 6U, 0);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_PERIOD_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_T_H_PHALANX - 4:
            {
                u32 phalanx_address;
                u32 phalanx_scale;
                phalanx_equipment_slot = 0;
                phalanx_unit_slot = 0;
check_phalanx_units:
                if ((IsBattleUnitActive(0, phalanx_unit_slot) << 0x18) != 0) {
                    phalanx_equipment_slot = 0;
                    phalanx_scale = phalanx_unit_slot << 2;
                    phalanx_records = (u8 *)0x02034B4C;
                    asm volatile("" : "+r"(phalanx_records));
                    phalanx_scale += phalanx_unit_slot;
                    phalanx_scale <<= 3;
                    phalanx_scale -= phalanx_unit_slot;
                    phalanx_row_offset = phalanx_scale << 4;
find_phalanx_weapon:
                    phalanx_address = (phalanx_equipment_slot * 4) + phalanx_row_offset;
                    phalanx_address += (u32)phalanx_records;
                    if ((M2C_FIELD(BATTLE_UNIT_OFFSET(equipment[0].item_id), u16 *, phalanx_address) == 0) || (BuildBattleEquipmentStats(0, phalanx_unit_slot, phalanx_equipment_slot, 0, ({ register struct EquipmentRecord *phalanx_call_arg asm("r3") = equipment_stats; asm volatile("" : "+r"(phalanx_call_arg)); phalanx_call_arg; })), ((EQUIPMENT_COMMAND & equipment_stats->flags) != 0)) || ({ equipment_stats_carrier = equipment_stats; asm volatile("" : "+r"(equipment_stats_carrier)); equipment_attributes_or_unit_flags = equipment_stats_carrier->attributes; asm volatile("" : "+r"(equipment_attributes_or_unit_flags)); equipment_or_unit_mask = WEAPON_MELEE; asm volatile("" : "+r"(equipment_or_unit_mask)); equipment_attributes_or_unit_flags & equipment_or_unit_mask; })) {
                        phalanx_equipment_slot += 1;
                        if ((u32) phalanx_equipment_slot <= BATTLE_EQUIPMENT_SLOT_COUNT - 1) {
                            goto find_phalanx_weapon;
                        }
                    }
                    if (phalanx_equipment_slot != BATTLE_EQUIPMENT_SLOT_COUNT) {
                        phalanx_unit_slot = (u32) (u8) (phalanx_unit_slot + 2);
                        if (phalanx_unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
                            goto check_phalanx_units;
                        }
                    }
                }
                if (phalanx_unit_slot > BATTLE_ACTIVE_UNIT_COUNT - 1) {
                    goto accept_command;
                }
                if ((u32) phalanx_equipment_slot <= BATTLE_EQUIPMENT_SLOT_COUNT - 1) {
                    OpenWindow(1, 3, 5, 0x18, 6U, 0);
                    PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                    PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                } else {
                    OpenWindow(1, 5, 4, 0x14, 8U, 0);
                    PrintWindowText(DECK_COMMAND_REQUIRES_RANGED_WEAPON_TEXT, 0, 0);
                    PrintWindowText(DECK_COMMAND_RANGED_WEAPON_FORMATION_TEXT, 1, 0);
                    PrintWindowText(DECK_COMMAND_NO_RANGED_WEAPON_TEXT, 0, 0);
                    RequestWindowRefresh();
                    do {
                        YieldTaskForUpdates(1);
                    } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                }
                goto reject_command;
            }
            case DECK_COMMAND_CANNON_PHALANX - 4:
                if ((IsBattleUnitActive(0, 1U) << 0x18) == 0) {
                    goto cannon_failure;
                }
                cannon_base = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(cannon_base));
                cannon_offset = (s32)sizeof(struct BattleUnit) / 4;
                asm volatile("" : "+r"(cannon_offset));
                cannon_offset *= 4;
                asm volatile("" : "+r"(cannon_offset));
                cannon_addr = cannon_base + cannon_offset;
                asm volatile("" : "+r"(cannon_addr));
                cannon_model_id = *cannon_addr;
                if ((cannon_model_id != DECK_COMMAND_CANNON_ULTRA_SAURUS) && (cannon_model_id != DECK_COMMAND_CANNON_ULTE_PHALANX)) {
                    goto cannon_failure;
                }
                if ((IsBattleUnitActive(0, 4U) << 0x18) == 0) {
                    goto cannon_failure;
                }
                cannon_status_offset = 4 * (s32)sizeof(struct BattleUnit) + BATTLE_UNIT_OFFSET(size_class);
                asm volatile("" : "+r"(cannon_status_offset));
                cannon_addr = cannon_base + cannon_status_offset;
                asm volatile("" : "+r"(cannon_addr));
                if ((u32)*cannon_addr > ZOID_SIZE_CLASS_L) {
                    goto cannon_failure;
                }
                goto cannon_success;
cannon_failure:
                OpenWindow(1, 3, 5, 0x18, 6U, 0);
                PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
cannon_success:
                {
                register u32 cannon_word_offset asm("r2") = (u32)gCannonGravityGunEquipmentOffset0;
                register u32 cannon_word_address asm("r0");
                cannon_word_address = (u32)cannon_base + cannon_word_offset;
                if (*(u16 *)cannon_word_address == DECK_COMMAND_CANNON_GRAVITY_GUN) {
                    goto accept_command;
                }
                }
                {
                register u32 cannon_word_offset asm("r3") = (u32)gCannonGravityGunEquipmentOffset1;
                register u32 cannon_word_address asm("r0");
                cannon_word_address = (u32)cannon_base + cannon_word_offset;
                if (*(u16 *)cannon_word_address == DECK_COMMAND_CANNON_GRAVITY_GUN) {
                    goto accept_command;
                }
                }
                {
                register u32 cannon_word_offset asm("r1") = (u32)gCannonGravityGunEquipmentOffset2;
                register u32 cannon_word_address asm("r0");
                cannon_word_address = (u32)cannon_base + cannon_word_offset;
                if (*(u16 *)cannon_word_address == DECK_COMMAND_CANNON_GRAVITY_GUN) {
                    goto accept_command;
                }
                }
                asm volatile("" :: "r"(cannon_base));
                OpenWindow(1, 6, 4, 0x12, 8U, 0);
                PrintWindowText(DECK_COMMAND_REQUIRES_GRAVITY_GUN_TEXT, 0, 1);
                PrintWindowText(DECK_COMMAND_GRAVITY_GUN_NAME_TEXT, 0, 1);
                RequestWindowRefresh();
                do {
                    YieldTaskForUpdates(1);
                } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                goto reject_command;
            case DECK_COMMAND_GOJULAS_GIGA_CANNON_GATTAI - 4:
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_GOJULAS_GIGA_CANNON, 0, 0) << 0x18) != 0) {
                    goto accept_command;
                }
                if ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_GOJULAS_GIGA_CANNON, 0, 1) << 0x18) == 0) {
                    giga_cannon_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_GOJULAS_GIGA_CANNON, 0, 2);
                    if (giga_cannon_formation_valid == 0) {
                        OpenWindow(1, 3, 5, 0x18, 6U, (s32) giga_cannon_formation_valid);
                        PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                        PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                        RequestWindowRefresh();
                        do {
                            YieldTaskForUpdates(1);
                        } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                        goto reject_command;
                    }
                }
                goto accept_command;
            case DECK_COMMAND_LORD_GALE_GATTAI - 4:
                if (((IsBattleCombinationFormationValid(BATTLE_COMBINATION_LORD_GALE, 0, 0) << 0x18) == 0) && ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_LORD_GALE, 0, 1) << 0x18) == 0) && ((IsBattleCombinationFormationValid(BATTLE_COMBINATION_LORD_GALE, 0, 3) << 0x18) == 0)) {
                    lord_gale_formation_valid = IsBattleCombinationFormationValid(BATTLE_COMBINATION_LORD_GALE, 0, 4);
                    if (lord_gale_formation_valid == 0) {
                        OpenWindow(1, 3, 5, 0x18, 6U, (s32) lord_gale_formation_valid);
                        PrintWindowText(DECK_COMMAND_FORMATION_RESTRICTION_TEXT, 0, 1);
                        PrintWindowText(DECK_COMMAND_FORMATION_INVALID_TEXT, 0, 1);
                        RequestWindowRefresh();
                        do {
                            YieldTaskForUpdates(1);
                        } while (!(DECK_COMMAND_DESCRIPTION_CLOSE_KEYS & *(u16 *)0x0300000E));
                        goto reject_command;
                    }
                }
                goto accept_command;
            }
reject_command:
            CloseWindow(1);
            goto select_command;
accept_command:
            asm volatile(".Lsub_080C1414_block252:");
            selection_battle_state = (u8 *)0x02034B4C;
            asm volatile("" : "+r"(selection_battle_state));
            selection_menu_index = (u8 *)0x0200A880;
            asm volatile("" : "+r"(selection_menu_index));
            selection_deck_offset = BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands);
            asm volatile("" : "+r"(selection_deck_offset));
            selection_deck_entry_address = (u32)selection_battle_state + selection_deck_offset;
            selection_deck_entry_address += *selection_menu_index;
            command_id = *(u8 *)selection_deck_entry_address;
            selected_action_command_offset = BATTLE_DECK_PREPARATION_OFFSET(actions[0].command_id);
            asm volatile("" : "+r"(selected_action_command_offset));
            selection_battle_state += selected_action_command_offset;
            *selection_battle_state = command_id;
            selection_succeeded = 1;
            goto close_selection;
        } else {
            goto select_command;
        }
    } else {
        if (menu_result != DECK_COMMAND_MENU_CANCELLED) {
            if (DECK_COMMAND_DESCRIPTION_KEY & *(u16 *)0x0200A884) {
                PlaySong(DECK_COMMAND_DESCRIPTION_SOUND);
                selected_deck_base = (u8 *)0x02034B4C;
                asm volatile("" : "+r"(selected_deck_base));
                selected_deck_menu_index = (u8 *)0x0200A880;
                asm volatile("" : "+r"(selected_deck_menu_index));
                selection_deck_offset = BATTLE_DECK_COMMAND_OFFSET(selected_deck_commands);
                asm volatile("" : "+r"(selection_deck_offset));
                selected_deck_base += selection_deck_offset;
                selected_deck_entry_address = *selected_deck_menu_index;
                selected_deck_base += selected_deck_entry_address;
                ShowDeckCommandDescription(*selected_deck_base);
            }
            goto select_command;
        }
        selection_succeeded = 0;
    }
close_selection:
    RunMenuScript(DECK_COMMAND_BATTLE_SELECT_CLOSE_SCRIPT);
    ReleaseEquipmentStatBuffer();
    return selection_succeeded;
}
