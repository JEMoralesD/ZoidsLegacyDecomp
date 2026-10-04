#include "m2c_prelude.h"
#include "../game/player_state.h"

u8 GetZoidFormIndex(u8) asm("func_080E523C");
void RecalculateZoidStats(void *, void *) asm("func_080E5880");

s32 AddPlayerZoid(s32 zoid_model_id, s32 palette_variant) asm("func_080E5A18");

s32 AddPlayerZoid(s32 zoid_model_id, s32 palette_variant)
{
    volatile u32 catalog_word_index;
    u8 * volatile default_equipment_destination;
    register u32 model_id asm("r9");
    register u32 stored_zoid_slot;
    register u8 *zoid asm("r5");
    register u32 form_or_equipment_index asm("r4");

    zoid_model_id <<= 24;
    model_id = (u32)zoid_model_id >> 24;
    palette_variant <<= 24;
    palette_variant = (u32)palette_variant >> 24;
    stored_zoid_slot = 1;
    {
        register u8 *zoid_records_base asm("r2") = (u8 *)0x020218E8;

        zoid = zoid_records_base + 0x70;
        goto test_free_slot;
next_slot:
        {
            register u32 next_index asm("r0") = stored_zoid_slot + 1;

            next_index <<= 24;
            stored_zoid_slot = next_index >> 24;
            if (stored_zoid_slot >= PLAYER_STORED_ZOID_COUNT) {
                goto slot_search_done;
            }
            {
                register u32 record_or_catalog_offset asm("r0");

                record_or_catalog_offset = stored_zoid_slot << 3;
                record_or_catalog_offset -= stored_zoid_slot;
                record_or_catalog_offset <<= 4;
                zoid = (u8 *)(record_or_catalog_offset + (u32)zoid_records_base);
            }
        }
test_free_slot:
        if (zoid[PLAYER_ZOID_OFFSET(model_id)] != 0) {
            goto next_slot;
        }
    }
slot_search_done:
    if (stored_zoid_slot == PLAYER_STORED_ZOID_COUNT) {
        return PLAYER_STORAGE_SLOT_NOT_FOUND;
    }

    form_or_equipment_index = 0;
    zoid[PLAYER_ZOID_OFFSET(model_id)] = model_id;
    zoid[PLAYER_ZOID_OFFSET(palette_variant)] = palette_variant;
    zoid[PLAYER_ZOID_OFFSET(pilot_slot)] = form_or_equipment_index;
    {
        register u32 new_record_flag_or_form_bit asm("r2");
        register u32 form_flags asm("r1");
        register u32 form_index asm("r0");

        form_index = GetZoidFormIndex(zoid[PLAYER_ZOID_OFFSET(model_id)]);
        form_index <<= 24;
        form_index >>= 24;
        new_record_flag_or_form_bit = PLAYER_RECORD_NEW;
        form_flags = new_record_flag_or_form_bit;
        form_flags <<= form_index;
        form_flags |= 1;
        zoid[PLAYER_ZOID_OFFSET(form_flags)] = form_flags;
        *(u16 *)(zoid + PLAYER_ZOID_OFFSET(flags)) = new_record_flag_or_form_bit;
    }
    *(u16 *)(zoid + PLAYER_ZOID_OFFSET(level)) = form_or_equipment_index;

    {
        register u32 model_id_times_eight asm("r10");
        register u8 *ep_regen_bonuses asm("r8");
        register u8 *weapon_power_bonuses asm("r6");
        register u8 *defense_bonuses asm("r12");
        register u32 zero_bonus asm("r1");
        u8 weapon_slot_index;

        model_id_times_eight = model_id << 3;
        {
            register u8 *equipment_destination_seed asm("r2") = zoid;

            equipment_destination_seed += 0x50;
            asm volatile("" : "+r"(equipment_destination_seed));
            default_equipment_destination = equipment_destination_seed;
        }
        {
            register u32 catalog_word_seed asm("r0") = model_id;

            asm volatile("" : "+r"(catalog_word_seed));
            catalog_word_seed >>= 5;
            catalog_word_index = catalog_word_seed;
        }
        {
            register u8 *ep_regen_bonus_seed asm("r1") = (u8 *)0x12;

            asm volatile("" : "+r"(ep_regen_bonus_seed));
            ep_regen_bonus_seed += (u32)zoid;
            ep_regen_bonuses = ep_regen_bonus_seed;
        }
        zero_bonus = 0;
        weapon_power_bonuses = zoid + 0x1E;
        defense_bonuses = zoid + PLAYER_ZOID_OFFSET(form_defense_bonuses);
        do {
            ep_regen_bonuses[form_or_equipment_index] = zero_bonus;
            defense_bonuses[form_or_equipment_index] = zero_bonus;
            weapon_slot_index = 0;
            do {
                weapon_power_bonuses[weapon_slot_index + (form_or_equipment_index << 2)] = zero_bonus;
                weapon_slot_index += 1;
            } while (weapon_slot_index < 4);
            {
                register u32 next_index asm("r0") = form_or_equipment_index + 1;

                form_or_equipment_index = (u8)next_index;
            }
        } while (form_or_equipment_index < PLAYER_ZOID_FORM_COUNT);

        {
            register u32 scaled_model_id asm("r1") = model_id_times_eight;
            register u32 model_id_copy asm("r2") = model_id;
            register u32 record_or_catalog_offset asm("r0");
            register u8 *base_stat_table asm("r1");
            register u8 *default_equipment_source asm("r2");
            register u8 *equipment_destination asm("r3");

            record_or_catalog_offset = scaled_model_id - model_id_copy;
            record_or_catalog_offset <<= 3;
            base_stat_table = (u8 *)0x087AFCC4;
            asm volatile("" : "+r"(base_stat_table));
            record_or_catalog_offset += (u32)base_stat_table;
            asm volatile("" : "+r"(record_or_catalog_offset));
            form_or_equipment_index = 0;
            equipment_destination = default_equipment_destination;
            default_equipment_source = (u8 *)record_or_catalog_offset + ZOID_BASE_RECORD_OFFSET(equipment);
            do {
                register u32 equipment_slot_offset asm("r0") = form_or_equipment_index << 2;

                *(u32 *)(equipment_destination + equipment_slot_offset) = *(u32 *)(default_equipment_source + equipment_slot_offset);
                {
                    register u32 next_index asm("r0") = form_or_equipment_index + 1;

                    form_or_equipment_index = (u8)next_index;
                }
            } while (form_or_equipment_index < 4);
        }
    }

    RecalculateZoidStats(zoid, 0);
    *(u16 *)(zoid + PLAYER_ZOID_OFFSET(current_hp)) = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(max_hp));
    *(u16 *)(zoid + PLAYER_ZOID_OFFSET(current_ep)) = *(u16 *)(zoid + PLAYER_ZOID_OFFSET(max_ep));
    {
        register u8 *player_state_bytes asm("r1") = (u8 *)0x020218E4;

        player_state_bytes[PLAYER_STATE_OFFSET(stored_zoid_count)] += 1;
    }
    {
        register u32 catalog_flags_base asm("r0") = 0x020217B4;
        register u32 catalog_word asm("r1");
        register u32 catalog_word_offset asm("r2");

        asm volatile("" : "+r"(catalog_flags_base));
        catalog_word = catalog_word_index;
        catalog_word_offset = catalog_word << 2;
        asm volatile("" : "+r"(catalog_word_offset));
        catalog_word_offset += catalog_flags_base;
        *(u32 *)catalog_word_offset |= 1 << (model_id & 31);
    }
    return stored_zoid_slot;
}
