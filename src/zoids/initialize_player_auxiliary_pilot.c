#include "m2c_prelude.h"
#include "../game/player_state.h"

void LoadAuxiliaryPilotEffects(void *) asm("func_080E77FC");

void InitializePlayerAuxiliaryPilot(s32 auxiliary_pilot_id, void *pilot_record) asm("func_080E6E04");

void InitializePlayerAuxiliaryPilot(s32 auxiliary_pilot_id, void *pilot_record) {
    register s32 work_value_or_address asm("r0");
    register s32 scaled_id_or_offset asm("r1");
    register s32 auxiliary_record_or_catalog_address asm("r2");
    register s32 definition_or_pilot_id asm("r3");
    register s32 emotion_growth_address asm("r4");
    register s32 auxiliary_id asm("r5");
    register s32 zero_state asm("r6");
    void *pilot;

    pilot = pilot_record;
    auxiliary_pilot_id <<= 24;
    auxiliary_id = (u32)auxiliary_pilot_id >> 24;
    work_value_or_address = 0x34;
    scaled_id_or_offset = auxiliary_id;
    scaled_id_or_offset *= work_value_or_address;
    work_value_or_address = 0x020280B8;
    auxiliary_record_or_catalog_address = scaled_id_or_offset + work_value_or_address;
    work_value_or_address = auxiliary_id << 1;
    work_value_or_address += auxiliary_id;
    work_value_or_address <<= 2;
    scaled_id_or_offset = 0x087B7774;
    definition_or_pilot_id = work_value_or_address + scaled_id_or_offset;

    work_value_or_address = 0;
    M2C_FIELD(auxiliary_record_or_catalog_address, u8 *, 0) = auxiliary_id;
    M2C_FIELD(auxiliary_record_or_catalog_address, u8 *, 1) = work_value_or_address;
    scaled_id_or_offset = 0;
    emotion_growth_address = auxiliary_record_or_catalog_address + 2;
    zero_state = 0;
clear_loop:
    work_value_or_address = emotion_growth_address + scaled_id_or_offset;
    M2C_FIELD(work_value_or_address, u8 *, 0) = zero_state;
    work_value_or_address = scaled_id_or_offset + 1;
    work_value_or_address <<= 24;
    scaled_id_or_offset = (u32)work_value_or_address >> 24;
    if ((u32)scaled_id_or_offset <= 3) {
        goto clear_loop;
    }

    scaled_id_or_offset = M2C_FIELD(definition_or_pilot_id, u8 *, 0);
    work_value_or_address = auxiliary_record_or_catalog_address;
    work_value_or_address += 0x28;
    M2C_FIELD(work_value_or_address, u8 *, 0) = scaled_id_or_offset;
    work_value_or_address = M2C_FIELD(definition_or_pilot_id, u16 *, 2);
    M2C_FIELD(auxiliary_record_or_catalog_address, u16 *, PLAYER_AUXILIARY_PILOT_OFFSET(hp_recovery_percent)) = work_value_or_address;
    work_value_or_address = M2C_FIELD(definition_or_pilot_id, u16 *, 4);
    M2C_FIELD(auxiliary_record_or_catalog_address, u16 *, 0x2C) = work_value_or_address;
    work_value_or_address = M2C_FIELD(definition_or_pilot_id, u16 *, 6);
    M2C_FIELD(auxiliary_record_or_catalog_address, u16 *, 0x2E) = work_value_or_address;
    work_value_or_address = M2C_FIELD(definition_or_pilot_id, u16 *, 8);
    M2C_FIELD(auxiliary_record_or_catalog_address, u16 *, 0x30) = work_value_or_address;
    work_value_or_address = M2C_FIELD(definition_or_pilot_id, u16 *, 0x0A);
    M2C_FIELD(auxiliary_record_or_catalog_address, u16 *, 0x32) = work_value_or_address;
    work_value_or_address = (s32)pilot;
    work_value_or_address += 0x31;
    M2C_FIELD(work_value_or_address, u8 *, 0) = auxiliary_id;
    work_value_or_address = auxiliary_record_or_catalog_address;
    LoadAuxiliaryPilotEffects((void *)work_value_or_address);

    scaled_id_or_offset = 0x020217B4;
    asm volatile("" : "+r"(scaled_id_or_offset));
    auxiliary_record_or_catalog_address = 0x087AF5F8;
    asm volatile("" : "+r"(auxiliary_record_or_catalog_address));
    work_value_or_address = auxiliary_id;
    work_value_or_address <<= 1;
    work_value_or_address += auxiliary_record_or_catalog_address;
    definition_or_pilot_id = M2C_FIELD(work_value_or_address, u16 *, 0);
    auxiliary_record_or_catalog_address = (u32)definition_or_pilot_id >> 5;
    auxiliary_record_or_catalog_address <<= 2;
    scaled_id_or_offset += 0x14;
    auxiliary_record_or_catalog_address += scaled_id_or_offset;
    work_value_or_address = 0x1F;
    definition_or_pilot_id &= work_value_or_address;
    scaled_id_or_offset = 1;
    scaled_id_or_offset <<= definition_or_pilot_id;
    work_value_or_address = M2C_FIELD(auxiliary_record_or_catalog_address, s32 *, 0);
    work_value_or_address |= scaled_id_or_offset;
    M2C_FIELD(auxiliary_record_or_catalog_address, s32 *, 0) = work_value_or_address;
}
