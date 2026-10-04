#include "m2c_prelude.h"
#include "../game/player_state.h"

void RecalculateZoidStats(void *, void *) asm("func_080E5880");

void ChangeZoidModel(u8 *zoid_record, s32 zoid_model_id, s32 palette_variant) asm("func_080E5B44");

void ChangeZoidModel(u8 *zoid_record, s32 zoid_model_id, s32 palette_variant)
{
    volatile u32 catalog_word_index;
    register u8 *zoid asm("r4") = zoid_record;
    register u32 shifted_model_id asm("r1") = zoid_model_id << 24;
    register u32 model_id asm("r8") = shifted_model_id >> 24;
    u8 zero_bonus;
    register u32 form_or_equipment_index asm("r2");
    u8 weapon_slot_index;
    u8 *weapon_power_bonuses;
    register u8 *ep_regen_bonuses asm("ip");
    u8 *defense_bonuses;
    register u8 *equipment_destination asm("sl");
    register u32 model_id_times_eight asm("r9");

    {
        register u32 zero_level asm("r3") = 0;

        zoid[PLAYER_ZOID_OFFSET(model_id)] = model_id;
        zoid[PLAYER_ZOID_OFFSET(palette_variant)] = palette_variant;
        {
            register u32 existing_flags_or_mask asm("r2") = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags));
            register u32 updated_flags_or_pilot_address asm("r0") = 1;

            updated_flags_or_pilot_address |= existing_flags_or_mask;
            existing_flags_or_mask = 0xFFF7;
            updated_flags_or_pilot_address &= existing_flags_or_mask;
            *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags)) = updated_flags_or_pilot_address;
        }
        *(u16 *)(zoid + PLAYER_ZOID_OFFSET(level)) = zero_level;
    }

    form_or_equipment_index = 0;
    zero_bonus = 0;
    weapon_power_bonuses = zoid + 0x1E;
    model_id_times_eight = model_id << 3;
    asm volatile("" : "+r"(model_id_times_eight));
    equipment_destination = zoid + PLAYER_ZOID_OFFSET(equipment);
    catalog_word_index = shifted_model_id >> 29;
    ep_regen_bonuses = zoid + PLAYER_ZOID_OFFSET(form_ep_regen_bonuses);
    defense_bonuses = zoid + PLAYER_ZOID_OFFSET(form_defense_bonuses);
    do {
        ep_regen_bonuses[form_or_equipment_index] = zero_bonus;
        defense_bonuses[form_or_equipment_index] = zero_bonus;
        weapon_slot_index = 0;
        do {
            weapon_power_bonuses[weapon_slot_index + (form_or_equipment_index << 2)] = zero_bonus;
            weapon_slot_index++;
        } while (weapon_slot_index < 4);
        {
            register u32 next_index asm("r0") = form_or_equipment_index + 1;
            form_or_equipment_index = (u8)next_index;
        }
    } while (form_or_equipment_index < PLAYER_ZOID_FORM_COUNT);

    {
        register u32 scaled_model_id_or_table_base asm("r2");
        register u32 model_id_copy asm("r1");
        register u32 equipment_or_catalog_offset asm("r0");
        register u8 *default_equipment_source asm("r3");
        register u8 *equipment_destination_copy asm("r5");

        asm volatile("" : "+r"(model_id_times_eight), "+r"(model_id));
        scaled_model_id_or_table_base = model_id_times_eight;
        model_id_copy = model_id;
        equipment_or_catalog_offset = scaled_model_id_or_table_base - model_id_copy;
        equipment_or_catalog_offset <<= 3;
        scaled_model_id_or_table_base = 0x087AFCC4;
        asm volatile("" : "+r"(scaled_model_id_or_table_base));
        equipment_or_catalog_offset += scaled_model_id_or_table_base;
        asm volatile("" : "+r"(equipment_or_catalog_offset));

        form_or_equipment_index = 0;
        equipment_destination_copy = equipment_destination;
        default_equipment_source = (u8 *)equipment_or_catalog_offset;
        default_equipment_source += ZOID_BASE_RECORD_OFFSET(equipment);
        do {
            u32 equipment_or_catalog_offset = form_or_equipment_index << 2;
            *(u32 *)(equipment_destination_copy + equipment_or_catalog_offset) = *(u32 *)(default_equipment_source + equipment_or_catalog_offset);
            {
                register u32 next_index asm("r0") = form_or_equipment_index + 1;
                form_or_equipment_index = (u8)next_index;
            }
        } while (form_or_equipment_index < 4);
    }

    {
        register u32 pilot_slot_offset asm("r0") = zoid[PLAYER_ZOID_OFFSET(pilot_slot)];
        register u32 updated_flags_or_pilot_address asm("r1");

        if (pilot_slot_offset != 0) {
            pilot_slot_offset <<= 6;
            asm volatile("" : "+r"(pilot_slot_offset));
            updated_flags_or_pilot_address = 0x02027378;
            asm volatile("" : "+r"(updated_flags_or_pilot_address));
            updated_flags_or_pilot_address = pilot_slot_offset + updated_flags_or_pilot_address;
        } else {
            updated_flags_or_pilot_address = 0;
        }
        RecalculateZoidStats(zoid, (void *)updated_flags_or_pilot_address);
    }

    *(u16 *)(zoid + PLAYER_ZOID_OFFSET(current_hp)) = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(max_hp));
    *(u16 *)(zoid + PLAYER_ZOID_OFFSET(current_ep)) = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(max_ep));
    {
        register u32 catalog_flags_base asm("r0") = 0x020217B4;
        register u32 catalog_word asm("r1");
        register u32 equipment_or_catalog_offset asm("r2");

        asm volatile("" : "+r"(catalog_flags_base));
        catalog_word = catalog_word_index;
        equipment_or_catalog_offset = catalog_word << 2;
        asm volatile("" : "+r"(equipment_or_catalog_offset));
        equipment_or_catalog_offset += catalog_flags_base;
        *(u32 *)equipment_or_catalog_offset |= 1 << (model_id & 31);
    }
}
