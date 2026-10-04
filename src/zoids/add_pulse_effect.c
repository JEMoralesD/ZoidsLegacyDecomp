#include "m2c_prelude.h"
#include "../game/player_state.h"


s32 AddPulseEffect(u8 effect_kind, u16 effect_value) asm("func_080E7868");

s32 AddPulseEffect(u8 effect_kind, u16 effect_value)
{
    struct PlayerAuxiliaryPilotRecordView *pulse_record = (struct PlayerAuxiliaryPilotRecordView *)PULSE_PLAYER_RECORD_RAM;
    u8 effect_slot;

    switch ((u32)effect_kind - AUXILIARY_EFFECT_MAX_HP_UP_1) {
    case AUXILIARY_EFFECT_MAX_HP_UP_1 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_MAX_HP_UP_2 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_MAX_HP_UP_3 - AUXILIARY_EFFECT_MAX_HP_UP_1:
        for (effect_slot = 0; effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1; effect_slot++) {
            if ((u8)(pulse_record->effect_kinds[effect_slot] - AUXILIARY_EFFECT_MAX_HP_UP_1) <= 2) {
                pulse_record->effect_kinds[effect_slot] = 0;
            }
        }
        break;
    case AUXILIARY_EFFECT_SELF_REPAIR_1 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_SELF_REPAIR_2 - AUXILIARY_EFFECT_MAX_HP_UP_1:
        for (effect_slot = 0; effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1; effect_slot++) {
            if ((u8)(pulse_record->effect_kinds[effect_slot] - AUXILIARY_EFFECT_SELF_REPAIR_1) <= 1) {
                pulse_record->effect_kinds[effect_slot] = 0;
            }
        }
        break;
    case AUXILIARY_EFFECT_MAX_EP_UP_1 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_MAX_EP_UP_2 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_MAX_EP_UP_3 - AUXILIARY_EFFECT_MAX_HP_UP_1:
        for (effect_slot = 0; effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1; effect_slot++) {
            if ((u8)(pulse_record->effect_kinds[effect_slot] - AUXILIARY_EFFECT_MAX_EP_UP_1) <= 2) {
                pulse_record->effect_kinds[effect_slot] = 0;
            }
        }
        break;
    case AUXILIARY_EFFECT_GEP_UP_1 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_GEP_UP_2 - AUXILIARY_EFFECT_MAX_HP_UP_1:
        for (effect_slot = 0; effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1; effect_slot++) {
            if ((u8)(pulse_record->effect_kinds[effect_slot] - AUXILIARY_EFFECT_GEP_UP_1) <= 1) {
                pulse_record->effect_kinds[effect_slot] = 0;
            }
        }
        break;
    case AUXILIARY_EFFECT_ZOS_1 - AUXILIARY_EFFECT_MAX_HP_UP_1:
    case AUXILIARY_EFFECT_ZOS_2 - AUXILIARY_EFFECT_MAX_HP_UP_1:
        for (effect_slot = 0; effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1; effect_slot++) {
            if ((u8)(pulse_record->effect_kinds[effect_slot] - AUXILIARY_EFFECT_ZOS_1) <= 1) {
                pulse_record->effect_kinds[effect_slot] = 0;
            }
        }
        break;
    default:
        for (effect_slot = 0; effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1; effect_slot++) {
            if (pulse_record->effect_kinds[effect_slot] == effect_kind) {
                pulse_record->effect_kinds[effect_slot] = 0;
            }
        }
        break;
    }

    effect_slot = 0;
    if (pulse_record->effect_kinds[0] != 0) {
        do {
            effect_slot++;
            if (effect_slot > PLAYER_PILOT_ABILITY_COUNT - 1) {
                goto effect_list_full;
            }
        } while (pulse_record->effect_kinds[effect_slot] != 0);
    }

    if (effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1) {
        goto store_effect;
    }

effect_list_full:
    return 0;

store_effect:
    pulse_record->effect_kinds[effect_slot] = effect_kind;
    pulse_record->effect_values[effect_slot] = effect_value;
    return 1;
}
