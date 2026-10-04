#include "m2c_prelude.h"
#include "../game/player_state.h"

extern s32 TestBattleRuleFlag(s32) asm("func_080E6664");
extern void UpdatePilotAbilities(void *, s32) asm("func_080E705C");
extern void LoadAuxiliaryPilotEffects(void *) asm("func_080E77FC");
extern void RecalculateBattleUnitStats(s32, s32) asm("func_080E8B08");
extern void ApplyBattlePassiveEquipmentEffects(s32, s32) asm("func_080E90AC");
extern void BiosCpuSet(void *, void *, u32) asm("func_080ECD2C");

void InitializeEnemyBattleUnitsFromEncounter(s32 encounter_group_word, s32 encounter_index_word) asm("func_080E94A0");

void InitializeEnemyBattleUnitsFromEncounter(s32 encounter_group_word, s32 encounter_index_word)
{
    volatile u32 dma_zero;
    u8 * volatile encounter_units;
    volatile u32 unit_slot;
    u8 * volatile pilot_level_address;
    u8 * volatile equipment_destination;
    volatile u32 next_unit_slot;
    volatile u32 unit_slot_times_four;
    volatile u32 encounter_unit_offset;
    u8 * volatile saved_encounter_equipment;

    encounter_group_word <<= 24;
    encounter_group_word = (u32)encounter_group_word >> 24;
    encounter_index_word <<= 24;
    encounter_index_word = (u32)encounter_index_word >> 24;
    {
        register u32 group_record_address asm("r2") = encounter_group_word * (s32)sizeof(struct BattleEncounterGroup);
        register u32 encounter_size_or_offset asm("r0") = sizeof(struct BattleEncounterRecord);
        register u32 encounter_table_address asm("r1");

        encounter_size_or_offset *= encounter_index_word;
        encounter_table_address = BATTLE_ENCOUNTER_TABLE_ROM;
        encounter_size_or_offset += encounter_table_address;
        group_record_address += encounter_size_or_offset;
        group_record_address += BATTLE_ENCOUNTER_OFFSET(units);
        encounter_units = (u8 *)group_record_address;
    }
    unit_slot = 0;
initialize_encounter_unit:
    {
        register u32 unit_slot_carrier asm("r1") = unit_slot;
        register u32 unit_slot_times_four_carrier asm("r3") = unit_slot_carrier << 2;
        register u32 battle_unit_offset asm("r0");
        u8 *battle_unit_bytes;
        u8 *pilot_record_bytes;
        u8 *auxiliary_pilot_bytes;
        register u32 encounter_unit_byte_offset asm("r2");
        register u8 *encounter_unit_bytes asm("r1");
        register u32 form_or_equipment_index asm("r5");
        register u8 *form_weapon_power_bonuses asm("r4");
        register u8 *form_ep_regen_bonuses asm("ip");
        register u8 *form_defense_bonuses asm("r9");
        register u8 *auxiliary_pilot_id_address asm("sl");

        battle_unit_offset = unit_slot_times_four_carrier + unit_slot_carrier;
        battle_unit_offset <<= 3;
        battle_unit_offset -= unit_slot_carrier;
        battle_unit_offset <<= 4;
        {
            register u8 *unit_record_base asm("r2") = (u8 *)0x02035ECC;
            register u8 *pilot_record_base asm("r1") = unit_record_base;

            battle_unit_bytes = (u8 *)(battle_unit_offset + (u32)unit_record_base);
            pilot_record_base += 0x70;
            pilot_record_bytes = (u8 *)(battle_unit_offset + (u32)pilot_record_base);
            unit_record_base += 0xB0;
            auxiliary_pilot_bytes = (u8 *)(battle_unit_offset + (u32)unit_record_base);
        }

        {
            register u32 input_unit_slot asm("r4") = unit_slot;
            encounter_unit_byte_offset = input_unit_slot << 4;
            {
                register u8 *config_view asm("r0") = encounter_units;
                encounter_unit_bytes = (u8 *)(encounter_unit_byte_offset + (u32)config_view);
            }
        }
        battle_unit_bytes[BATTLE_UNIT_OFFSET(zoid_id)] = encounter_unit_bytes[BATTLE_ENCOUNTER_UNIT_OFFSET(zoid_id)];
        battle_unit_bytes[BATTLE_UNIT_OFFSET(palette_variant)] = encounter_unit_bytes[BATTLE_ENCOUNTER_UNIT_OFFSET(palette_variant)];
        battle_unit_bytes[BATTLE_UNIT_OFFSET(pilot_slot_or_definition_id)] = encounter_unit_bytes[BATTLE_ENCOUNTER_UNIT_OFFSET(pilot_id)];
        {
            register u32 zero8 asm("r1") = 0;
            battle_unit_bytes[BATTLE_UNIT_OFFSET(form_flags)] = zero8;
        }
        {
            register u32 zero16 asm("r4") = 0;
            *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(flags)) = zero16;
            *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(level)) = zero16;
        }

        form_or_equipment_index = 0;
        unit_slot_times_four = unit_slot_times_four_carrier;
        encounter_unit_offset = encounter_unit_byte_offset;
        {
            register u32 next_view asm("r0") = unit_slot + 1;
            next_unit_slot = next_view;
        }
        form_weapon_power_bonuses = battle_unit_bytes + 0x1E;
        {
            register u8 *copy_view asm("r1") = battle_unit_bytes + BATTLE_UNIT_OFFSET(equipment);
            equipment_destination = copy_view;
        }
        {
            register u8 *metadata_view asm("r2") = pilot_record_bytes + PLAYER_PILOT_OFFSET(level);
            pilot_level_address = metadata_view;
        }
        {
            register u8 *detail_view asm("r3");
            detail_view = (u8 *)PLAYER_PILOT_OFFSET(auxiliary_pilot_id);
            asm volatile("" : "+r"(detail_view));
            detail_view += (u32)pilot_record_bytes;
            auxiliary_pilot_id_address = detail_view;
        }
        {
            register u32 first_view asm("r0") = BATTLE_UNIT_OFFSET(form_ep_regen_bonuses);
            asm volatile("" : "+r"(first_view));
            first_view += (u32)battle_unit_bytes;
            form_ep_regen_bonuses = (u8 *)first_view;
        }
        {
            register u32 second_view asm("r1") = BATTLE_UNIT_OFFSET(form_defense_bonuses);
            asm volatile("" : "+r"(second_view));
            second_view += (u32)battle_unit_bytes;
            form_defense_bonuses = (u8 *)second_view;
        }

clear_form_bonuses:
        {
            register u32 zero asm("r3");
            {
                register u8 *first_view asm("r2") = form_ep_regen_bonuses;
                register u8 *first_at asm("r0");
                asm volatile("" : "+r"(first_view));
                first_at = first_view + form_or_equipment_index;
                zero = 0;
                *first_at = zero;
            }
            {
                register u8 *second_view asm("r1") = form_defense_bonuses;
                register u8 *second_at asm("r0");
                asm volatile("" : "+r"(second_view));
                second_at = second_view + form_or_equipment_index;
                *second_at = zero;
            }
            {
                register u32 inner asm("r1") = 0;
clear_form_weapon_power:
                {
                    register u32 row asm("r2") = form_or_equipment_index << 2;
                    register u32 at asm("r0") = inner + row;
                    register u32 inner_zero asm("r3");
                    register u32 next asm("r0");

                    at = (u32)form_weapon_power_bonuses + at;
                    inner_zero = 0;
                    *(u8 *)at = inner_zero;
                    next = inner + 1;
                    next <<= 24;
                    inner = next >> 24;
                }
                if (inner <= 3) {
                    goto clear_form_weapon_power;
                }
            }
        }
        {
            register u32 next asm("r0") = form_or_equipment_index + 1;
            next <<= 24;
            form_or_equipment_index = next >> 24;
        }
        if (form_or_equipment_index <= 5) {
            goto clear_form_bonuses;
        }

        {
            register u8 *definition_record_bytes asm("r9");
            register u8 *encounter_equipment_ids asm("r2");

            {
                register u32 id asm("r0") = battle_unit_bytes[BATTLE_UNIT_OFFSET(zoid_id)];
                register u32 definition_record_offset asm("r1") = id << 3;

                definition_record_offset -= id;
                definition_record_offset <<= 3;
                {
                    register u8 *definition_table_base asm("r4") = (u8 *)0x087AFCC4;
                    asm volatile("" : "+r"(definition_table_base));
                    definition_record_offset += (u32)definition_table_base;
                }
                definition_record_bytes = (u8 *)definition_record_offset;
            }
            form_or_equipment_index = 0;
            {
                register u32 input_offset_view asm("r1") = encounter_unit_offset;
                register u8 *config_view2 asm("r2") = encounter_units;
                register u8 *sum asm("r0") = (u8 *)(input_offset_view + (u32)config_view2);
                encounter_equipment_ids = sum + 4;
            }
copy_equipment_slots:
            {
                register u32 word_offset asm("r4") = form_or_equipment_index << 2;
                register u8 *dst_base asm("r3") = equipment_destination;
                register u8 *dst asm("r0") = dst_base + word_offset;
                register u8 *src asm("r1") = definition_record_bytes;

                src += ZOID_BASE_RECORD_OFFSET(equipment);
                src += word_offset;
                *(u32 *)dst = *(u32 *)src;
                if (form_or_equipment_index <= 3) {
                    s32 equipment_slots_restricted;
                    register s32 lookup_id asm("r0") = BATTLE_RULE_EQUIPMENT_SLOTS_4_TO_7_ONLY;

                    asm volatile("" : "+r"(lookup_id));
                    saved_encounter_equipment = encounter_equipment_ids;
                    equipment_slots_restricted = TestBattleRuleFlag(lookup_id);
                    equipment_slots_restricted <<= 24;
                    encounter_equipment_ids = saved_encounter_equipment;
                    if (equipment_slots_restricted == 0) {
                        register u8 *slot asm("r0") = battle_unit_bytes + word_offset;
                        register u8 *equipment_view asm("r1") = encounter_equipment_ids + form_or_equipment_index;
                        register u32 value asm("r1") = *equipment_view;
                        slot += 0x52;
                        *(u16 *)slot = value;
                    } else {
                        register u8 *slot asm("r0") = battle_unit_bytes + word_offset;
                        register u32 zero asm("r4");
                        slot += 0x52;
                        zero = 0;
                        *(u16 *)slot = zero;
                    }
                }
            }
            {
                register u32 next asm("r0") = form_or_equipment_index + 1;
                next <<= 24;
                form_or_equipment_index = next >> 24;
            }
            if (form_or_equipment_index <= 7) {
                goto copy_equipment_slots;
            }
        }

        {
            register u32 definition_id asm("r2") = battle_unit_bytes[BATTLE_UNIT_OFFSET(pilot_slot_or_definition_id)];
            register u32 definition_record_offset asm("r1") = definition_id << 4;
            register u8 *definition_table_base asm("r0") = (u8 *)PILOT_BASE_RECORDS_ROM;
            register u8 *definition_record_bytes asm("r1");

            asm volatile("" : "+r"(definition_table_base));
            definition_record_offset += (u32)definition_table_base;
            definition_record_bytes = (u8 *)definition_record_offset;
            pilot_record_bytes[PLAYER_PILOT_OFFSET(active_pilot_id)] = definition_id;
            {
                register u32 value asm("r0") = definition_record_bytes[PILOT_BASE_RECORD_OFFSET(level)];
                register u8 *dst asm("r2") = pilot_level_address;
                *dst = value;
            }
            {
                register u32 value asm("r0") = definition_record_bytes[PILOT_BASE_RECORD_OFFSET(auxiliary_pilot_id)];
                register u8 *dst asm("r3") = auxiliary_pilot_id_address;
                *dst = value;
            }
            *(u16 *)(pilot_record_bytes + PLAYER_PILOT_OFFSET(max_hp_bonus_percent)) = *(u16 *)(definition_record_bytes + PILOT_BASE_RECORD_OFFSET(max_hp_bonus_percent));
            *(u16 *)(pilot_record_bytes + PLAYER_PILOT_OFFSET(mobility_bonus_percent)) = *(u16 *)(definition_record_bytes + PILOT_BASE_RECORD_OFFSET(mobility_bonus_percent));
            *(u16 *)(pilot_record_bytes + PLAYER_PILOT_OFFSET(sensor_accuracy_bonus_percent)) = *(u16 *)(definition_record_bytes + PILOT_BASE_RECORD_OFFSET(sensor_accuracy_bonus_percent));
            *(u16 *)(pilot_record_bytes + PLAYER_PILOT_OFFSET(weapon_accuracy_bonus_percent)) = *(u16 *)(definition_record_bytes + PILOT_BASE_RECORD_OFFSET(weapon_accuracy_bonus_percent));
            *(u16 *)(pilot_record_bytes + PLAYER_PILOT_OFFSET(dcp_bonus_percent)) = *(u16 *)(definition_record_bytes + PILOT_BASE_RECORD_OFFSET(dcp_bonus_percent));
        }
        UpdatePilotAbilities(pilot_record_bytes, 1);
        {
            register u8 *kind_view asm("r4") = auxiliary_pilot_id_address;
            register u32 r2_birth_guard asm("r2");
            u32 definition_id;

            asm volatile("" : "=r"(r2_birth_guard));
            definition_id = *kind_view;
            asm volatile("" : : "r"(r2_birth_guard));

            if (definition_id != 0) {
                register u32 auxiliary_pilot_id_carrier asm("r2");
                register u32 definition_record_offset asm("r1");
                register u8 *definition_record_bytes asm("r1");

                auxiliary_pilot_id_carrier = definition_id;
                definition_record_offset = auxiliary_pilot_id_carrier << 1;
                definition_record_offset += auxiliary_pilot_id_carrier;
                definition_record_offset <<= 2;
                {
                    register u8 *definition_table_base asm("r0") = (u8 *)AUXILIARY_PILOT_BASE_RECORDS_ROM;
                    asm volatile("" : "+r"(definition_table_base));
                    definition_record_offset += (u32)definition_table_base;
                    definition_record_bytes = (u8 *)definition_record_offset;
                }
                {
                    register u8 *auxiliary_record_cursor asm("r0") = auxiliary_pilot_bytes;
                    register u32 table_value asm("r2");
                    auxiliary_record_cursor[PLAYER_AUXILIARY_PILOT_OFFSET(auxiliary_pilot_id)] = auxiliary_pilot_id_carrier;
                    table_value = definition_record_bytes[AUXILIARY_PILOT_BASE_RECORD_OFFSET(level_bonus)];
                    auxiliary_record_cursor += PLAYER_AUXILIARY_PILOT_OFFSET(level_bonus);
                    *auxiliary_record_cursor = table_value;
                }
                {
                    register u8 *auxiliary_record_store asm("r2");
                    register u32 first_value asm("r0") = *(u16 *)(definition_record_bytes + AUXILIARY_PILOT_BASE_RECORD_OFFSET(hp_recovery_percent));
                    auxiliary_record_store = auxiliary_pilot_bytes;
                    *(u16 *)(auxiliary_record_store + PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)) = first_value;
                    *(u16 *)(auxiliary_record_store + PLAYER_AUXILIARY_PILOT_OFFSET(weapon_power_bonus_percent)) = *(u16 *)(definition_record_bytes + AUXILIARY_PILOT_BASE_RECORD_OFFSET(weapon_power_bonus_percent));
                    *(u16 *)(auxiliary_record_store + PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent)) = *(u16 *)(definition_record_bytes + AUXILIARY_PILOT_BASE_RECORD_OFFSET(sensor_accuracy_bonus_percent));
                    *(u16 *)(auxiliary_record_store + PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent)) = *(u16 *)(definition_record_bytes + AUXILIARY_PILOT_BASE_RECORD_OFFSET(speed_bonus_percent));
                    *(u16 *)(auxiliary_record_store + PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent)) = *(u16 *)(definition_record_bytes + AUXILIARY_PILOT_BASE_RECORD_OFFSET(defense_bonus_percent));
                }
                LoadAuxiliaryPilotEffects(auxiliary_pilot_bytes);
            } else {
                dma_zero = 0;
                {
                    register void *zero_ptr asm("r0");
                    register void *auxiliary_record_cursor asm("r1");
                    register u32 control asm("r2");

                    zero_ptr = (void *)&dma_zero;
                    asm volatile("" : "+r"(zero_ptr));
                    auxiliary_record_cursor = auxiliary_pilot_bytes;
                    asm volatile("" : "+r"(auxiliary_record_cursor));
                    control = 0x0500000D;
                    asm volatile("" : "+r"(control));
                    BiosCpuSet(zero_ptr, auxiliary_record_cursor, control);
                }
            }
        }

        {
            register s32 enemy_side asm("r0") = BATTLE_ENEMY_SIDE;
            asm volatile("" : "+r"(enemy_side));
            ApplyBattlePassiveEquipmentEffects(enemy_side, unit_slot);
        }
        {
            register s32 enemy_side asm("r0") = BATTLE_ENEMY_SIDE;
            asm volatile("" : "+r"(enemy_side));
            RecalculateBattleUnitStats(enemy_side, unit_slot);
        }
        *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(hp)) = *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(max_hp));
        {
            register s32 value asm("r0");
            register u32 sign asm("r1");
            register u32 r1_birth_guard asm("r1");
            register u32 r2_birth_guard asm("r2");

            asm volatile("" : "=r"(r1_birth_guard), "=r"(r2_birth_guard));
            value = *(s16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(max_ep));
            asm volatile("" : : "r"(r1_birth_guard), "r"(r2_birth_guard));
            sign = (u32)value >> 31;
            value += sign;
            value >>= 1;
            *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(ep)) = value;
        }

        {
            register u32 unit_slot_times_four_carrier_again asm("r4") = unit_slot_times_four;
            register u32 unit_slot_carrier_again asm("r0") = unit_slot;
            register u32 battle_unit_offset_again asm("r2") = unit_slot_times_four_carrier_again + unit_slot_carrier_again;
            register u8 *encounter_unit_bytes_again asm("r3");

            battle_unit_offset_again <<= 3;
            battle_unit_offset_again -= unit_slot_carrier_again;
            battle_unit_offset_again <<= 4;
            {
                register u32 dst asm("r1");
                {
                    register u32 global_base asm("r3") = 0x02034B4C;
                    register u32 global_offset asm("r4") = (sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(experience_reward));
                    asm volatile("" : "+r"(global_base));
                    asm volatile("" : "+r"(global_offset));
                    dst = global_base + global_offset;
                    dst = battle_unit_offset_again + dst;
                }
                {
                    register u32 input_offset_view2 asm("r0") = encounter_unit_offset;
                    register u8 *config_view3 asm("r4") = encounter_units;
                    encounter_unit_bytes_again = (u8 *)(input_offset_view2 + (u32)config_view3);
                }
                *(u32 *)dst = *(u32 *)(encounter_unit_bytes_again + BATTLE_ENCOUNTER_UNIT_OFFSET(experience_reward));
            }
            {
                register u32 dst asm("r0");
                register u32 global_base asm("r1") = 0x02034B4C;
                register u32 global_offset asm("r4") = (sizeof(struct BattleSide) + BATTLE_UNIT_OFFSET(money_reward));
                asm volatile("" : "+r"(global_base));
                asm volatile("" : "+r"(global_offset));
                dst = global_base + global_offset;
                dst = battle_unit_offset_again + dst;
                *(u32 *)dst = *(u32 *)(encounter_unit_bytes_again + BATTLE_ENCOUNTER_UNIT_OFFSET(money_reward));
            }
            {
                register u32 global_base asm("r0") = 0x02034B4C;
                asm volatile("" : "+r"(global_base));
                battle_unit_offset_again += global_base;
                {
                    register u32 global_offset asm("r1") = 0x15E4;
                    asm volatile("" : "+r"(global_offset));
                    battle_unit_offset_again += global_offset;
                    {
                        register u32 zero asm("r4") = 0;
                        *(u16 *)battle_unit_offset_again = zero;
                    }
                }
            }

            {
                register u32 flags asm("r1") = *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(flags));
                u32 enabled = encounter_unit_bytes_again[BATTLE_ENCOUNTER_UNIT_OFFSET(flag40_enabled)];
                if (enabled != 0) {
                    flags |= 0x40;
                }
                *(u16 *)(battle_unit_bytes + BATTLE_UNIT_OFFSET(flags)) = flags;
            }
        }
    }

    {
        register u32 next_view asm("r1") = next_unit_slot;
        register u32 normalized asm("r0") = next_view << 24;
        normalized >>= 24;
        unit_slot = normalized;
        if (normalized <= BATTLE_ACTIVE_UNIT_COUNT - 1) {
            goto initialize_encounter_unit;
        }
    }
}
