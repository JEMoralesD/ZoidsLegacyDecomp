#include "m2c_prelude.h"
#include "../game/player_state.h"
s32 ScaleByPercent(s16, s16) asm("func_080E522C");                        /* extern */
s32 FindAbilityValue(void *, s32, u8) asm("func_080E74F0");                 /* extern */
s16 DivideSigned32(s32, u8) asm("func_080ECD98");                         /* extern */

void ApplyPilotZoidStatModifiers(void *zoid, void *pilot, void *auxiliary_pilot) asm("func_080E6830");

void ApplyPilotZoidStatModifiers(void *zoid, void *pilot, void *auxiliary_pilot) {
    u8 effective_pilot_level;
    void *auxiliary_stat_address;

    if (pilot != (void *)0) {
        effective_pilot_level = M2C_FIELD(pilot, u8 *, PLAYER_PILOT_OFFSET(level));
        if ((FindAbilityValue(pilot, PILOT_ABILITY_ZOID_PROFICIENCY_1, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(model_id))) << 0x10) != 0) {
            effective_pilot_level += 5;
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_ZOID_PROFICIENCY_2, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(model_id))) << 0x10) != 0) {
            effective_pilot_level += 0xA;
        }
        if ((FindAbilityValue(pilot, PILOT_ABILITY_ZOID_PROFICIENCY_3, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(model_id))) << 0x10) != 0) {
            effective_pilot_level += 0x14;
        }
        if (auxiliary_pilot != (void *)0) {
            effective_pilot_level += M2C_FIELD(auxiliary_pilot, u8 *, PLAYER_AUXILIARY_PILOT_OFFSET(level_bonus));
        }
        if ((u32) effective_pilot_level > PLAYER_PILOT_LEVEL_LIMIT) {
            effective_pilot_level = PLAYER_PILOT_LEVEL_LIMIT;
        }
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)) = (s16) ((u16) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)) + ScaleByPercent(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)), M2C_FIELD(pilot, s16 *, PLAYER_PILOT_OFFSET(max_hp_bonus_percent))));
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) = (s16) ((u16) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) + ScaleByPercent(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)), M2C_FIELD(pilot, s16 *, PLAYER_PILOT_OFFSET(sensor_accuracy_bonus_percent))));
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)) = (s16) ((u16) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)) + ScaleByPercent(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)), M2C_FIELD(pilot, s16 *, PLAYER_PILOT_OFFSET(mobility_bonus_percent))));
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)) = (s16) ((u16) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)) + ScaleByPercent(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)), M2C_FIELD(pilot, s16 *, PLAYER_PILOT_OFFSET(dcp_bonus_percent))));
        if ((u32) effective_pilot_level < (u32) M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(required_pilot_level))) {
            M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)) = DivideSigned32(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)) * effective_pilot_level, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(required_pilot_level)));
            M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) = DivideSigned32(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) * effective_pilot_level, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(required_pilot_level)));
            M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)) = DivideSigned32(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)) * effective_pilot_level, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(required_pilot_level)));
            M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)) = DivideSigned32(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)) * effective_pilot_level, M2C_FIELD(zoid, u8 *, PLAYER_ZOID_OFFSET(required_pilot_level)));
        }
    }
    if (auxiliary_pilot != (void *)0) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) = (s16) ((u16) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) + ScaleByPercent(M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)), M2C_FIELD(auxiliary_pilot, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(sensor_accuracy_bonus_percent))));
        auxiliary_stat_address = (zoid + 0x4A) - 8;
        M2C_FIELD(auxiliary_stat_address, s16 *, 0) = (s16) ((u16) M2C_FIELD(auxiliary_stat_address, s16 *, 0) + ScaleByPercent(M2C_FIELD(auxiliary_stat_address, s16 *, 0), M2C_FIELD(auxiliary_pilot, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(speed_bonus_percent))));
        auxiliary_stat_address += 4;
        M2C_FIELD(auxiliary_stat_address, s16 *, 0) = (s16) ((u16) M2C_FIELD(auxiliary_stat_address, s16 *, 0) + ScaleByPercent(M2C_FIELD(auxiliary_stat_address, s16 *, 0), M2C_FIELD(auxiliary_pilot, s16 *, PLAYER_AUXILIARY_PILOT_OFFSET(defense_bonus_percent))));
    }
}
