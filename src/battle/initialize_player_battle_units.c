#include "../field/field_actor.h"
#include "../game/player_state.h"
#include "combinations/battle_combination.h"

s32 TestBattleRuleFlag(s32) asm("func_080E6664");                             /* extern */
M2C_UNK UpdatePilotAbilities(s8 *, s32) asm("func_080E705C");                   /* extern */
M2C_UNK RecalculateBattleUnitStats(s32, u32) asm("func_080E8B08");                    /* extern */
M2C_UNK ApplyBattlePassiveEquipmentEffects(s32, u32) asm("func_080E90AC");                    /* extern */
M2C_UNK BiosCpuSet(void *, void *, u32) asm("func_080ECD2C");          /* extern */
M2C_UNK CopyBytes(void *, void *, s32) asm("func_080ED038");         /* extern */
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u8 gFieldTransportStateBytes[] asm("D_0202ECF4");
extern u8 gZoidBaseStatTable[];

void InitializePlayerBattleUnits(void) asm("func_080E9128");

void InitializePlayerBattleUnits(void) {
    volatile s32 missing_auxiliary_zero;
    volatile s32 empty_pilot_zero;
    volatile s32 empty_auxiliary_zero;
    /* Transport HP and EP reuse the last stored Zoid pointer; an empty party leaves it untouched. */
    void *volatile last_stored_zoid_record;
    /* Unrecognized field models leave the native transport-model stack slot untouched. */
    volatile s32 transport_model_id;
    s8 *volatile transport_pilot_record;
    s32 *volatile transport_auxiliary_zero_source;
    u8 *volatile transport_pilot_level_address;
    s8 *volatile transport_auxiliary_id_address;
    u16 *volatile transport_pilot_hp_bonus_address;
    u16 *volatile transport_pilot_mobility_bonus_address;
    u16 *volatile transport_pilot_sensor_bonus_address;
    u16 *volatile transport_pilot_weapon_accuracy_bonus_address;
    u16 *volatile transport_pilot_dcp_bonus_address;
    void *volatile transport_auxiliary_record;
    s16 transport_ep_source;
    s16 party_max_ep;
    register s32 unit_record_offset asm("r0");
    s32 name_character_offset;
    s32 quote_character_offset;
    s32 transport_pilot_definition_offset;
    register s32 selected_transport_model asm("r1");
    register u32 unit_or_text_index asm("r8");
    u8 next_unit_slot;
    u8 next_name_character;
    u8 next_quote_character;
    u8 field_transport_actor_model;
    register u32 auxiliary_pilot_id asm("r0");
    u8 stored_zoid_slot;
    u8 equipment_slot;
    u8 *stored_zoid_record;
    u8 *equipment_slot_base;
    u8 *transport_pilot_definition;
    register u8 *stored_pilot_record asm("r4");
    register u8 *battle_unit_bytes asm("r5");

    unit_or_text_index = 0;
initialize_player_unit:
    unit_record_offset = unit_or_text_index * (s32)sizeof(struct BattleUnit);
    battle_unit_bytes = unit_record_offset + BATTLE_STATE_RAM;
    if (unit_or_text_index <= BATTLE_ACTIVE_UNIT_COUNT - 1U) {
        register u8 *save_base asm("r6") = gPlayerStateBytes;
        {
            register u32 offset asm("r1") = PLAYER_STATE_OFFSET(team_zoid_slots);
            register u8 *lookup asm("r0") = save_base + offset;
            lookup += unit_or_text_index;
            stored_zoid_slot = *lookup;
        }
        if (stored_zoid_slot != 0) {
            {
                register u32 id asm("r1") = stored_zoid_slot;
                register u32 offset asm("r0") = id << 3;
                register u8 *base_plus4 asm("r1");
                asm volatile("" : "+r"(id));
                offset -= id;
                offset <<= 4;
                base_plus4 = save_base + 4;
                stored_zoid_record = (u8 *)(offset + (u32)base_plus4);
            }
            last_stored_zoid_record = stored_zoid_record;
            stored_pilot_record = (u8 *)(M2C_FIELD(stored_zoid_record, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) << 6);
            {
                register u32 offset asm("r2") = PLAYER_STATE_OFFSET(pilots);
                register u8 *base asm("r0") = save_base + offset;
                stored_pilot_record += (u32)base;
            }
            {
                register u8 *dst asm("r0") = battle_unit_bytes;
                asm volatile("" : "+r"(dst));
                CopyBytes(dst, last_stored_zoid_record, sizeof(struct PlayerZoidRecordView));
            }
            CopyBytes(battle_unit_bytes + BATTLE_UNIT_PILOT_OFFSET(pilot), stored_pilot_record, sizeof(struct PlayerPilotRecordView));
            auxiliary_pilot_id = M2C_FIELD(stored_pilot_record, u8 *, PLAYER_PILOT_OFFSET(auxiliary_pilot_id));
            if (auxiliary_pilot_id != 0) {
                register u32 detail asm("r1") = auxiliary_pilot_id;
                register u32 stride asm("r0") = 0x34;
                register u32 offset asm("r2") = PLAYER_STATE_OFFSET(auxiliary_pilots);
                register u8 *base asm("r0");
                register u8 *src asm("r1");
                register u8 *dst asm("r0");
                detail *= stride;
                base = save_base + offset;
                src = (u8 *)(detail + (u32)base);
                dst = battle_unit_bytes + 0xB0;
                CopyBytes(dst, src, sizeof(struct PlayerAuxiliaryPilotRecordView));
            } else {
                register u8 *dst asm("r1");
                register void *src asm("r0");
                missing_auxiliary_zero = auxiliary_pilot_id;
                dst = battle_unit_bytes + 0xB0;
                asm volatile("" : "+r"(dst));
                src = (void *)&missing_auxiliary_zero;
                asm volatile("" : "+r"(src));
                BiosCpuSet(src, dst, 0x0500000D);
            }
            if ((TestBattleRuleFlag(BATTLE_RULE_EQUIPMENT_SLOTS_4_TO_7_ONLY) << 0x18) != 0) {
                equipment_slot = 0;
                do {
                    u8 *item;
                    equipment_slot_base = battle_unit_bytes + (equipment_slot * 4);
                    item = (u8 *)((M2C_FIELD(equipment_slot_base, u16 *, BATTLE_UNIT_OFFSET(equipment[0].item_id)) * 0x18) + EQUIPMENT_RECORDS_ROM);
                    asm volatile("" : "+r"(item));
                    if (EQUIPMENT_PASSIVE & M2C_FIELD(item, u16 *, (s32)&((struct EquipmentRecord *)0)->flags)) {
                        u32 zero;
                        asm volatile("" : "=r"(zero));
                        zero = 0;
                        M2C_FIELD(equipment_slot_base, u16 *, BATTLE_UNIT_OFFSET(equipment[0].item_id)) = zero;
                    }
                    equipment_slot += 1;
                } while ((u32) equipment_slot <= 3U);
            }
            ApplyBattlePassiveEquipmentEffects(BATTLE_PLAYER_SIDE, unit_or_text_index);
            RecalculateBattleUnitStats(BATTLE_PLAYER_SIDE, unit_or_text_index);
            party_max_ep = M2C_FIELD(battle_unit_bytes, s16 *, BATTLE_UNIT_OFFSET(max_ep));
            M2C_FIELD(battle_unit_bytes, s16 *, BATTLE_UNIT_OFFSET(ep)) = (s16) ((s32) (party_max_ep + ((u32) party_max_ep >> 0x1F)) >> 1);
            if (*(u8 *)0x0203055C == 1) {
                M2C_FIELD(battle_unit_bytes, u16 *, BATTLE_UNIT_OFFSET(hp)) = (u16) M2C_FIELD(battle_unit_bytes, u16 *, BATTLE_UNIT_OFFSET(max_hp));
            }
        } else {
            battle_unit_bytes[0] = stored_zoid_slot;
            empty_pilot_zero = (s32) stored_zoid_slot;
            {
                register u8 *dst asm("r1") = battle_unit_bytes + 0x70;
                asm volatile("" : "+r"(dst));
                BiosCpuSet(&empty_pilot_zero, dst, 0x05000010);
            }
            empty_auxiliary_zero = (s32) stored_zoid_slot;
            {
                register u8 *dst asm("r1") = battle_unit_bytes + 0xB0;
                asm volatile("" : "+r"(dst));
                BiosCpuSet(&empty_auxiliary_zero, dst, 0x0500000D);
            }
        }
        {
            register u8 *base asm("r2") = (u8 *)0x02034B4C;
            register u32 offset asm("r1") = 0x9C;
            register u8 *dst asm("r0");
            asm volatile("" : "+r"(base), "+r"(offset));
            offset <<= 6;
            dst = base + offset;
            dst += unit_or_text_index;
            *dst = (u8)unit_or_text_index;
        }
        {
            register u8 *base asm("r0") = (u8 *)0x02034B4C;
            register u32 offset asm("r2") = BATTLE_PARTY_IDENTITY_OFFSET(original_party_stored_zoid_slots);
            register u8 *dst asm("r1");
            register u8 *save_base asm("r0");
            register u32 save_offset asm("r2");
            asm volatile("" : "+r"(base), "+r"(offset));
            dst = base + offset;
            dst += unit_or_text_index;
            save_base = gPlayerStateBytes;
            save_offset = PLAYER_STATE_OFFSET(team_zoid_slots);
            asm volatile("" : "+r"(save_base), "+r"(save_offset));
            save_base += save_offset;
            save_base += unit_or_text_index;
            *dst = *save_base;
        }
        {
            register u8 *dst asm("r1") = (u8 *)0x02037252;
            register u32 value asm("r0");
            dst += unit_or_text_index;
            asm volatile("" : "+r"(dst));
            value = BATTLE_COMBINATION_NO_OWNER;
            *dst = value;
        }
    } else {
        register u32 transport_model_times_eight asm("r9");
        register u8 *equipment_destination asm("sl");
        register u8 *form_ep_regen_bonuses asm("ip");
        register u8 *form_weapon_power_bonuses asm("r6");
        u8 *form_defense_bonuses;
        if (unit_or_text_index == 6) {
            field_transport_actor_model = gFieldTransportStateBytes[FIELD_TRANSPORT_STATE_OFFSET(actor_model_id)];
            switch (field_transport_actor_model) {
            case 0x69: {
                register u32 selected asm("r2") = ZOID_GUSTAV;
                transport_model_id = selected;
                goto initialize_transport_unit;
            }
            case 0x6A: {
                register u32 selected asm("r0") = ZOID_HOVER_CARGO;
                transport_model_id = selected;
                goto initialize_transport_unit;
            }
            case 0x6B:
            case 0x6C:
                selected_transport_model = ZOID_DRAGOON_NEST;
                goto store_transport_model;
            default:
                goto initialize_transport_unit;
            }
        } else {
            selected_transport_model = ZOID_WHALE_KING;
store_transport_model:
            transport_model_id = selected_transport_model;
        }
initialize_transport_unit:
        {
            register u32 stack_value asm("r2") = (u32)&missing_auxiliary_zero;
            stack_value = *(u8 *)(stack_value + 0x10);
            battle_unit_bytes[0] = stack_value;
        }
        M2C_FIELD(battle_unit_bytes, s8 *, BATTLE_UNIT_OFFSET(palette_variant)) = 0;
        M2C_FIELD(battle_unit_bytes, u8 *, BATTLE_UNIT_OFFSET(pilot_slot_or_definition_id)) = (u8) unit_or_text_index;
        M2C_FIELD(battle_unit_bytes, s8 *, BATTLE_UNIT_OFFSET(form_flags)) = 0;
        {
            register u32 half_zero asm("r2") = 0;
            M2C_FIELD(battle_unit_bytes, s16 *, BATTLE_UNIT_OFFSET(flags)) = half_zero;
            M2C_FIELD(battle_unit_bytes, s16 *, BATTLE_UNIT_OFFSET(level)) = half_zero;
        }
        asm volatile("movs r3, #0" : : : "r3");
        {
            register u8 *p0 asm("r0");
            register u8 *p1 asm("r1");
            register u8 *p2 asm("r2");
            register u32 shift_seed asm("r0");

            p0 = battle_unit_bytes + 0x70;
            transport_pilot_record = (s8 *)p0;
            p1 = battle_unit_bytes + 0xB0;
            transport_auxiliary_record = p1;
            asm volatile(
                "mov %0, sp\n\t"
                "add %0, %0, #8"
                : "=r"(p2));
            transport_auxiliary_zero_source = (s32 *)p2;
            shift_seed = transport_model_id;
            shift_seed <<= 3;
            transport_model_times_eight = shift_seed;
            asm volatile(
                "movs r1, #80\n\t"
                "add r1, r1, r5\n\t"
                "mov %0, r1"
                : "=r"(equipment_destination)
                :
                : "r1");
            p2 = battle_unit_bytes + 0xA0;
            transport_pilot_level_address = p2;
            p0 = battle_unit_bytes + 0xA1;
            transport_auxiliary_id_address = (s8 *)p0;
            asm volatile("mov %0, %1" : "=r"(p1) : "r"(battle_unit_bytes));
            p1 += 0xA4;
            transport_pilot_hp_bonus_address = (u16 *)p1;
            p2 += 6;
            transport_pilot_mobility_bonus_address = (u16 *)p2;
            p0 += 7;
            transport_pilot_sensor_bonus_address = (u16 *)p0;
            p1 += 6;
            transport_pilot_weapon_accuracy_bonus_address = (u16 *)p1;
            p2 += 6;
            transport_pilot_dcp_bonus_address = (u16 *)p2;
            asm volatile(
                "movs r0, #18\n\t"
                "add r0, r0, r5\n\t"
                "mov %0, r0"
                : "=r"(form_ep_regen_bonuses)
                :
                : "r0");
        }
        {
        register u8 clear_zero asm("r1") = 0;
        form_weapon_power_bonuses = battle_unit_bytes + 0x1E;
        form_defense_bonuses = battle_unit_bytes + 0x18;
        asm volatile(
            "1:\n\t"
            "mov r2, ip\n\t"
            "add r0, r2, r3\n\t"
            "strb r1, [r0, #0]\n\t"
            "add r0, r7, r3\n\t"
            "strb r1, [r0, #0]\n\t"
            "movs r2, #0\n\t"
            "lsl r4, r3, #2\n"
            "2:\n\t"
            "add r0, r2, r4\n\t"
            "add r0, r6, r0\n\t"
            "strb r1, [r0, #0]\n\t"
            "add r0, r2, #1\n\t"
            "lsl r0, r0, #24\n\t"
            "lsr r2, r0, #24\n\t"
            "cmp r2, #3\n\t"
            "bls 2b\n\t"
            "add r0, r3, #1\n\t"
            "lsl r0, r0, #24\n\t"
            "lsr r3, r0, #24\n\t"
            "cmp r3, #5\n\t"
            "bls 1b"
            :
            : "r"(form_ep_regen_bonuses), "r"(clear_zero), "r"(form_weapon_power_bonuses), "r"(form_defense_bonuses)
            : "r0", "r2", "r3", "r4", "memory");
        }
        {
            register u32 table_addr asm("r0");
            register u32 r1_value asm("r1");
            register u32 r2_value asm("r2");
            register u32 copy_index asm("r3");
            register u8 *dst_base asm("r4");

            r1_value = transport_model_times_eight;
            asm volatile("" : "+r"(r1_value));
            r2_value = transport_model_id;
            table_addr = r1_value - r2_value;
            table_addr <<= 3;
            r1_value = (u32)gZoidBaseStatTable;
            table_addr += r1_value;
            asm volatile("" : "+r"(table_addr));
            copy_index = 0;
            dst_base = equipment_destination;
            r2_value = table_addr;
            r2_value += ZOID_BASE_RECORD_OFFSET(equipment);
            do {
                register u32 offset asm("r0") = copy_index << 2;
                register u8 *dst asm("r1") = dst_base + offset;
                register u8 *src asm("r0") = (u8 *)r2_value + offset;
                register u32 next asm("r0");
                *(u32 *)dst = *(u32 *)src;
                next = copy_index + 1;
                next <<= 24;
                copy_index = next >> 24;
            } while ((u32)copy_index <= 7U);
        }
        transport_pilot_definition_offset = M2C_FIELD(battle_unit_bytes, u8 *, BATTLE_UNIT_OFFSET(pilot_slot_or_definition_id)) * 0x10;
        transport_pilot_definition = transport_pilot_definition_offset + PILOT_BASE_RECORDS_ROM;
        {
            register u32 value asm("r0") = 0x64;
            register u8 *dst asm("r2");
            dst = (u8 *)transport_pilot_record;
            *dst = value;
        }
        {
            register u32 value asm("r0") = M2C_FIELD(transport_pilot_definition_offset, u8 *, PILOT_BASE_RECORDS_ROM);
            register u8 *dst asm("r2");
            dst = transport_pilot_level_address;
            *dst = value;
        }
        {
            register u32 zero asm("r2") = 0;
            register u8 *dst asm("r0");
            dst = (u8 *)transport_auxiliary_id_address;
            *dst = zero;
        }
        {
            register u32 value asm("r0") = M2C_FIELD(transport_pilot_definition, u16 *, PILOT_BASE_RECORD_OFFSET(max_hp_bonus_percent));
            register u16 *dst asm("r2");
            dst = transport_pilot_hp_bonus_address;
            *dst = value;
        }
        {
            register u32 value asm("r0") = M2C_FIELD(transport_pilot_definition, u16 *, PILOT_BASE_RECORD_OFFSET(mobility_bonus_percent));
            register u16 *dst asm("r2");
            dst = transport_pilot_mobility_bonus_address;
            *dst = value;
        }
        {
            register u32 value asm("r0") = M2C_FIELD(transport_pilot_definition, u16 *, PILOT_BASE_RECORD_OFFSET(sensor_accuracy_bonus_percent));
            register u16 *dst asm("r2");
            dst = transport_pilot_sensor_bonus_address;
            *dst = value;
        }
        {
            register u32 value asm("r0") = M2C_FIELD(transport_pilot_definition, u16 *, PILOT_BASE_RECORD_OFFSET(weapon_accuracy_bonus_percent));
            register u16 *dst asm("r2");
            dst = transport_pilot_weapon_accuracy_bonus_address;
            *dst = value;
        }
        {
            register u32 value asm("r0") = M2C_FIELD(transport_pilot_definition, u16 *, PILOT_BASE_RECORD_OFFSET(dcp_bonus_percent));
            register u16 *dst asm("r1");
            dst = transport_pilot_dcp_bonus_address;
            *dst = value;
        }
        UpdatePilotAbilities(transport_pilot_record, 1);
        {
            register u32 zero asm("r2") = 0;
            empty_auxiliary_zero = zero;
        }
        BiosCpuSet(transport_auxiliary_zero_source, transport_auxiliary_record, 0x0500000D);
        ApplyBattlePassiveEquipmentEffects(BATTLE_PLAYER_SIDE, unit_or_text_index);
        RecalculateBattleUnitStats(BATTLE_PLAYER_SIDE, unit_or_text_index);
        {
            register u8 *record asm("r1") = last_stored_zoid_record;
            M2C_FIELD(battle_unit_bytes, u16 *, BATTLE_UNIT_OFFSET(hp)) = M2C_FIELD(record, u16 *, PLAYER_ZOID_OFFSET(max_hp));
            transport_ep_source = M2C_FIELD(record, s16 *, PLAYER_ZOID_OFFSET(max_ep));
        }
        M2C_FIELD(battle_unit_bytes, s16 *, BATTLE_UNIT_OFFSET(ep)) = (s16) ((s32) (transport_ep_source + ((u32) transport_ep_source >> 0x1F)) >> 1);
    }
    asm volatile(
        "movs r1, #154\n\t"
        "lsl r1, r1, #2\n\t"
        "add r0, r5, r1\n\t"
        "movs r2, #0\n\t"
        "str r2, [r0, #0]\n\t"
        "add r1, #4\n\t"
        "add r0, r5, r1\n\t"
        "str r2, [r0, #0]\n\t"
        "movs r2, #153\n\t"
        "lsl r2, r2, #2\n\t"
        "add r0, r5, r2\n\t"
        "movs r1, #0\n\t"
        "strh r1, [r0, #0]"
        : : : "r0", "r1", "r2", "memory");
    asm volatile("" : "+r"(unit_or_text_index));
    next_unit_slot = unit_or_text_index + 1;
    unit_or_text_index = (u32) next_unit_slot;
    if ((u32) next_unit_slot <= BATTLE_UNIT_SLOT_COUNT - 1U) {
        goto initialize_player_unit;
    }
    {
        register u32 zero asm("r2");
        asm volatile("movs %0, #0" : "=r"(zero));
        unit_or_text_index = zero;
    }
    do {
        name_character_offset = unit_or_text_index * 2;
        M2C_FIELD(name_character_offset, u16 *, BATTLE_PARTY_IDENTITY_ADDRESS(pilot_names[BATTLE_PLAYER_SIDE])) = (u16) M2C_FIELD(name_character_offset, u16 *, PLAYER_NAME_RAM);
        next_name_character = unit_or_text_index + 1;
        unit_or_text_index = next_name_character;
    } while ((u32) next_name_character <= BATTLE_PARTY_NAME_CHARACTER_COUNT - 1U);
    unit_or_text_index = 0;
    do {
        quote_character_offset = unit_or_text_index * 2;
        M2C_FIELD(quote_character_offset, u16 *, BATTLE_PARTY_IDENTITY_ADDRESS(battle_quotes[BATTLE_PLAYER_SIDE])) = (u16) M2C_FIELD(quote_character_offset, u16 *, PLAYER_BATTLE_QUOTE_RAM);
        next_quote_character = unit_or_text_index + 1;
        unit_or_text_index = next_quote_character;
    } while ((u32) next_quote_character <= BATTLE_PARTY_QUOTE_CHARACTER_COUNT - 1U);
}
