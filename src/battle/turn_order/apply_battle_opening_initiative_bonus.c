#include "m2c_prelude.h"
#include "battle_turn_order.h"

extern u8 gBattleState[];

void AddBattleEffect(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE65C");
s32 FindAbilityValue(void *, s32, s32) asm("func_080E74F0");
void RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");

void ApplyBattleOpeningInitiativeBonus(void) asm("func_080BFEA4");

void ApplyBattleOpeningInitiativeBonus(void) {
    s16 opening_initiatives[2];
    u8 blocks_opponent_bonus[2];
    u8 side;
    u8 unit_slot;
    u8 *block_flags;

    side = 0;
    block_flags = blocks_opponent_bonus;
    {
        u8 *battle_state = gBattleState;

        do {
            s16 *side_initiative = &opening_initiatives[side];
            u8 *block_flag = &block_flags[side];
            s32 zero = 0;

            asm volatile("" : "+r"(zero));
            *block_flag = zero;
            *side_initiative = zero;
            unit_slot = 0;
            do {
                if ((IsBattleUnitActive(side, unit_slot) << 24) != 0) {
                    register s16 *current_side_initiative asm("r2");
                    register u32 unit_address asm("r0");

                    current_side_initiative = &opening_initiatives[side];
                    asm volatile("" : "+r"(current_side_initiative));
                    unit_address = unit_slot * 0x270;
                    asm volatile("" : "+r"(unit_address));
                    unit_address += side * 0x1380;
                    asm volatile("" : "+r"(unit_address));
                    unit_address += (u32)battle_state;
                    asm volatile("" : "+r"(unit_address));

                    {
                        register s32 highest_initiative asm("r1");
                        u16 unit_initiative;

                        highest_initiative = *current_side_initiative;
                        asm volatile("" : "+r"(highest_initiative));
                        unit_initiative = BATTLE_UNIT_FIELD(unit_address, u16, initiative);
                        if (highest_initiative < BATTLE_UNIT_FIELD(unit_address, s16, initiative)) {
                            *current_side_initiative = unit_initiative;
                        }
                    }
                }
                unit_slot++;
            } while (unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            side++;
        } while (side <= BATTLE_SIDE_COUNT - 1);
    }

    side = 0;
    do {
        s32 next_side;
        u16 *side_initiative;

        unit_slot = 0;
        next_side = side + 1;
        side_initiative = (u16 *)&opening_initiatives[side];
        do {
            if ((IsBattleUnitActive(side, unit_slot) << 24) != 0) {
                u8 *pilot_record = gBattleState
                    + side * 0x1380 + unit_slot * 0x270 + 0x70;

                if ((FindAbilityValue(pilot_record, PILOT_ABILITY_OPENING_INITIATIVE_200, 0) << 16) != 0) {
                    *side_initiative += 200;
                }
                if ((FindAbilityValue(pilot_record, PILOT_ABILITY_OPENING_INITIATIVE_500, 0) << 16) != 0) {
                    *side_initiative += 500;
                }
                if ((FindAbilityValue(pilot_record, PILOT_ABILITY_BLOCK_OPPONENT_OPENING_BONUS, 0) << 16) != 0) {
                    block_flags[side] = 1;
                }
            }
            unit_slot++;
        } while (unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        side = next_side;
    } while (side <= BATTLE_SIDE_COUNT - 1);

    if (opening_initiatives[0] / 2 >= opening_initiatives[1] && block_flags[1] == 0) {
        side = 0;
    } else if (opening_initiatives[1] / 2 >= opening_initiatives[0] && block_flags[0] == 0) {
        side = 1;
    } else {
        return;
    }

    {
        s32 zero;
        register u8 *battle_state asm("r8");

        unit_slot = 0;
        zero = 0;
        battle_state = gBattleState;
        asm volatile("" : "+r"(battle_state));
        do {
            if ((IsBattleUnitActive(side, unit_slot) << 24) != 0) {
                AddBattleEffect(side, unit_slot, -1, 0,
                    zero, zero, BATTLE_EFFECT_INITIATIVE,
                    ({
                        register u32 unit_field_address asm("r0");

                        unit_field_address = unit_slot * 0x270;
                        asm volatile("" : "+r"(unit_field_address));
                        unit_field_address += side * 0x1380;
                        asm volatile("" : "+r"(unit_field_address));
                        unit_field_address += (u32)battle_state;
                        asm volatile("" : "+r"(unit_field_address));
                        BATTLE_UNIT_FIELD(unit_field_address, s16, mobility);
                    }),
                    1, zero);
                RecalculateBattleUnitStats(side, unit_slot);
            }
            unit_slot++;
        } while (unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
    }
}
