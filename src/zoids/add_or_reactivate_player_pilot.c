#include "m2c_prelude.h"
#include "../game/player_state.h"

extern u8 gPlayerPilotRecordsBytes[] asm("D_02027378");
extern u8 gPilotDefinitionsBytes[] asm("D_087B70E4");
extern s32 gPilotExperienceThresholds[] asm("D_087B77C8");
extern u8 gPilotGrowthTableBytes[] asm("D_087B7958");
extern u8 gPlayerCatalogFlagsBytes[] asm("D_020217B4");

void InitializePlayerAuxiliaryPilot(u8, void *) asm("func_080E6E04");
void UpdatePilotAbilities(void *, s32) asm("func_080E705C");
s32 TestEventFlag(s32) asm("func_0809F818");

u8 AddOrReactivatePlayerPilot(u8 pilot_id) asm("func_080E6C78");

u8 AddOrReactivatePlayerPilot(u8 pilot_id) {
    register u8 first_pilot_slot asm("r6");
    u32 stored_pilot_slot;
    u8 requested_pilot_id;
    u32 new_slot;
    u8 *pilot_records_base;
    register u8 *pilot_records_seed asm("r0");
    u8 *pilot;
    u8 first_active_pilot_id;
    u8 *pilot_definition;
    register u32 pilot_flags_or_record_word asm("r0");
    register u32 pilot_flag_mask asm("r1");
    register u32 stored_pilot_id_bits asm("r2");
    register u32 *pilot_catalog_word asm("r2");

    requested_pilot_id = pilot_id;
    new_slot = 0;
    first_pilot_slot = 1;
    stored_pilot_slot = first_pilot_slot;
    pilot_records_seed = gPlayerPilotRecordsBytes;
    pilot = pilot_records_seed;
    pilot += 0x40;
    first_active_pilot_id = *pilot;
    pilot_records_base = pilot_records_seed;
    if (first_active_pilot_id != 0) {
advance_inactive_pilot:
        {
            register u32 next_pilot_slot asm("r0");
            next_pilot_slot = stored_pilot_slot + 1;
            next_pilot_slot <<= 24;
            stored_pilot_slot = next_pilot_slot >> 24;
        }
        if (stored_pilot_slot >= PLAYER_STORED_PILOT_COUNT) {
            goto inactive_search_done;
        }
        {
            register u32 pilot_record_offset asm("r0");
            pilot_record_offset = stored_pilot_slot << 6;
            pilot = (u8 *)(pilot_record_offset + (u32)pilot_records_base);
        }
        if (*pilot != 0) {
            goto advance_inactive_pilot;
        }
    }
test_inactive_pilot_id:
    if ((*(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id)) >> 8) != requested_pilot_id) {
        goto advance_inactive_pilot;
    }
inactive_search_done:
    if (stored_pilot_slot == PLAYER_STORED_PILOT_COUNT) {
        stored_pilot_slot = 1;
        pilot = pilot_records_base + 0x40;
        pilot_flags_or_record_word = *(u32 *)(pilot_records_base + 0x40);
        pilot_flag_mask = 0xFF0000FF;
        goto test_empty_pilot_slot;
advance_empty_pilot_slot:
        {
            register u32 next_pilot_slot asm("r0");
            next_pilot_slot = stored_pilot_slot + 1;
            next_pilot_slot <<= 24;
            stored_pilot_slot = next_pilot_slot >> 24;
        }
        if (stored_pilot_slot >= PLAYER_STORED_PILOT_COUNT) {
            goto empty_slot_search_done;
        }
        {
            register u32 pilot_record_offset asm("r0");
            pilot_record_offset = stored_pilot_slot << 6;
            pilot = (u8 *)(pilot_record_offset + (u32)pilot_records_base);
        }
        pilot_flags_or_record_word = *(u32 *)pilot;
test_empty_pilot_slot:
        if ((pilot_flags_or_record_word & pilot_flag_mask) != 0) {
            goto advance_empty_pilot_slot;
        }
empty_slot_search_done:
        if (stored_pilot_slot == PLAYER_STORED_PILOT_COUNT) {
            return PLAYER_STORAGE_SLOT_NOT_FOUND;
        }
        new_slot = 1;
    }

    {
        register u32 pilot_definition_offset asm("r0");
        register u8 *pilot_definition_table asm("r1");
        pilot_definition_offset = requested_pilot_id << 4;
        pilot_definition_table = gPilotDefinitionsBytes;
        pilot_definition = (u8 *)(pilot_definition_offset + (u32)pilot_definition_table);
    }
    pilot_flags_or_record_word = 0;
    pilot[PLAYER_PILOT_OFFSET(active_pilot_id)] = requested_pilot_id;
    pilot[PLAYER_PILOT_OFFSET(zoid_slot)] = pilot_flags_or_record_word;
    stored_pilot_id_bits = requested_pilot_id << 8;
    if (new_slot == 0) {
        pilot_flag_mask = *(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id));
        pilot_flags_or_record_word = PLAYER_RECORD_NEW;
        pilot_flags_or_record_word &= pilot_flag_mask;
    } else {
        pilot_flags_or_record_word = PLAYER_RECORD_NEW;
    }
    *(u16 *)(pilot + PLAYER_PILOT_OFFSET(flags_and_pilot_id)) = pilot_flags_or_record_word | stored_pilot_id_bits;

    if (new_slot != 0) {
        *(u32 *)(pilot + PLAYER_PILOT_OFFSET(experience)) = gPilotExperienceThresholds[pilot_definition[0] - 1];
        {
            register u8 *pilot_level_address asm("r0");
            register u32 initial_level asm("r1");
            initial_level = pilot_definition[0];
            pilot_level_address = pilot;
            pilot_level_address += 0x30;
            *pilot_level_address = initial_level;
        }
        if (pilot_definition[1] != 0) {
            InitializePlayerAuxiliaryPilot(pilot_definition[1], pilot);
        }
        {
            register u8 *growth_class_address asm("r3");
            u32 growth_class;
            growth_class = pilot_definition[2];
            growth_class_address = pilot;
            growth_class_address += 0x32;
            *growth_class_address = growth_class;
        }
        {
            register u8 *growth_table asm("r2");
            growth_table = gPilotGrowthTableBytes;
            asm volatile("" : : "r"(growth_table));
        }
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MAX_HP])) = *(u16 *)(gPilotGrowthTableBytes + (pilot[PLAYER_PILOT_OFFSET(growth_class)] * 12));
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_MOBILITY])) = *(u16 *)(gPilotGrowthTableBytes + (pilot[PLAYER_PILOT_OFFSET(growth_class)] * 12) + 2);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_SENSOR_ACCURACY])) = *(u16 *)(gPilotGrowthTableBytes + (pilot[PLAYER_PILOT_OFFSET(growth_class)] * 12) + 4);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_WEAPON_ACCURACY])) = *(u16 *)(gPilotGrowthTableBytes + (pilot[PLAYER_PILOT_OFFSET(growth_class)] * 12) + 6);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(stat_growth[PILOT_GROWTH_DCP])) = *(u16 *)(gPilotGrowthTableBytes + (pilot[PLAYER_PILOT_OFFSET(growth_class)] * 12) + 8);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(max_hp_bonus_percent)) = *(u16 *)(pilot_definition + 4);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(mobility_bonus_percent)) = *(u16 *)(pilot_definition + 6);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(sensor_accuracy_bonus_percent)) = *(u16 *)(pilot_definition + 8);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(weapon_accuracy_bonus_percent)) = *(u16 *)(pilot_definition + 10);
        *(u16 *)(pilot + PLAYER_PILOT_OFFSET(dcp_bonus_percent)) = *(u16 *)(pilot_definition + 12);
        UpdatePilotAbilities(pilot, 1);
    }

    {
        register u8 *catalog_flags_base asm("r4");
        register u32 pilot_catalog_word_offset asm("r2");
        register u8 *pilot_catalog_flags_base asm("r0");
        catalog_flags_base = gPlayerCatalogFlagsBytes;
        pilot_catalog_word_offset = requested_pilot_id >> 5;
        pilot_catalog_word_offset <<= 2;
        pilot_catalog_flags_base = catalog_flags_base;
        pilot_catalog_flags_base += PLAYER_CATALOG_OFFSET(pilots);
        pilot_catalog_word_offset += (u32)pilot_catalog_flags_base;
        pilot_catalog_word = (u32 *)pilot_catalog_word_offset;
        *pilot_catalog_word |= 1 << (requested_pilot_id & 0x1F);
        if ((u8)(requested_pilot_id - 0x60) <= 1 && (TestEventFlag(2) << 24) != 0) {
            *(u32 *)(catalog_flags_base + PLAYER_CATALOG_OFFSET(pilots)) |= 4;
        }
        if (requested_pilot_id == 0x5F) {
            register u8 *catalog_flags_base_copy asm("r0");
            register u32 revealed_pilot_flags asm("r1");
            catalog_flags_base_copy = gPlayerCatalogFlagsBytes;
            revealed_pilot_flags = *(u32 *)(catalog_flags_base_copy + PLAYER_CATALOG_OFFSET(pilots));
            revealed_pilot_flags |= 0x40000000;
            *(u32 *)(catalog_flags_base_copy + PLAYER_CATALOG_OFFSET(pilots)) = revealed_pilot_flags;
        }
    }
    return stored_pilot_slot;
}
