#include "m2c_prelude.h"
#include "../game/player_state.h"


u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");

s32 FindActiveBattleAuxiliaryPilotEffectValue(s32 side_word, s32 unit_slot_word, s32 effect_kind_word) asm("func_080E7AE0");

s32 FindActiveBattleAuxiliaryPilotEffectValue(s32 side_word, s32 unit_slot_word, s32 effect_kind_word) {
    u32 side;
    u32 unit_slot;
    u32 effect_kind;

    side_word <<= 24;
    side = (u32)side_word >> 24;
    unit_slot_word <<= 24;
    unit_slot = (u32)unit_slot_word >> 24;
    effect_kind_word <<= 24;
    effect_kind = (u32)effect_kind_word >> 24;
    if ((u8)FindBattleEffect(side, unit_slot, BATTLE_EFFECT_AUXILIARY_PILOT_ACTIVE) != BATTLE_EFFECT_NOT_FOUND) {
        register s32 side_record_offset asm("r1");
        register s32 unit_record_offset asm("r0");
        register u8 *base asm("r2");
        register struct BattleUnitPilotStateView *unit_pilot_state asm("r1");
        register u8 *auxiliary_record_base asm("r3");
        register u32 effect_slot asm("r2");
        register u8 *first_kind_address asm("r0");
        register s32 current_kind asm("r0");

        side_record_offset = side << 2;
        side_record_offset += side;
        side_record_offset <<= 3;
        side_record_offset -= side;
        side_record_offset <<= 7;
        unit_record_offset = unit_slot << 2;
        unit_record_offset += unit_slot;
        unit_record_offset <<= 3;
        unit_record_offset -= unit_slot;
        unit_record_offset <<= 4;
        base = (u8 *)0x02034B4C;
        unit_record_offset += (s32)base;
        unit_pilot_state = (struct BattleUnitPilotStateView *)(side_record_offset + unit_record_offset);
        auxiliary_record_base = (u8 *)unit_pilot_state;
        auxiliary_record_base += BATTLE_UNIT_PILOT_OFFSET(auxiliary_pilot);
        effect_slot = 0;
        first_kind_address = (u8 *)unit_pilot_state;
        first_kind_address += BATTLE_UNIT_PILOT_OFFSET(auxiliary_pilot.effect_kinds);
        current_kind = *first_kind_address;
        if (current_kind != effect_kind) {
            register u8 *effect_kinds_base asm("r1");

            effect_kinds_base = (u8 *)unit_pilot_state;
            effect_kinds_base += BATTLE_UNIT_PILOT_OFFSET(auxiliary_pilot.effect_kinds);
            do {
                register u32 next_effect_slot asm("r0");
                register u8 *kind_address asm("r0");

                next_effect_slot = effect_slot + 1;
                next_effect_slot <<= 24;
                effect_slot = next_effect_slot >> 24;
                if (effect_slot > PLAYER_PILOT_ABILITY_COUNT - 1) {
                    return 0;
                }
                kind_address = (u8 *)((s32)effect_kinds_base + effect_slot);
                current_kind = *kind_address;
            } while (current_kind != effect_kind);
        }
        if (effect_slot <= PLAYER_PILOT_ABILITY_COUNT - 1) {
            register s32 effect_value_offset asm("r0");
            register u8 *effect_value_address asm("r1");
            register s32 zero asm("r2");
            register s32 effect_value asm("r0");

            effect_value_offset = effect_slot << 1;
            effect_value_address = auxiliary_record_base;
            effect_value_address += PLAYER_AUXILIARY_PILOT_OFFSET(effect_values);
            asm volatile("" : "+r"(effect_value_address));
            effect_value_address = (u8 *)((s32)effect_value_address + effect_value_offset);
            asm volatile("" : "+r"(effect_value_address));
            zero = 0;
            effect_value = *(s16 *)(effect_value_address + zero);
            /* A stored zero returns one to signal that the effect exists. */
            if (effect_value == 0) {
                return 1;
            }
            return effect_value;
        }
    }
    return 0;
}
