#include "m2c_prelude.h"
#include "battle.h"
#include "../game/player_state.h"

extern u8 gBattleState[];
extern u8 gBattleEffectRemovalMasks[];
extern s32 gRandomNumberCallback asm("D_03000010");
extern s32 RollBattleStatusResistance(s32, s32, s32) asm("func_080BE488");
extern s32 ScaleByPercent(s32, s32) asm("func_80E522C");
extern s32 CallFunctionR0(s32) asm("func_80ECD5C");

void RollBattleEffectRemovalMasks(void) asm("func_080BFB90");

void RollBattleEffectRemovalMasks(void)
{
    s32 mask_bank;
    s32 side_mask_offset;
    s32 bank_mask_offset;
    s32 unit_slot_times_four;
    u32 *round_mask;
    s32 side_times_four;
    s32 next_bank;
    s32 next_side;
    s32 next_unit_slot;
    u32 *action_mask;
    s32 side_record_offset;
    s32 unit_record_offset;
    u32 roll;
    register u32 side asm("r10");
    register u32 unit_slot asm("r9");
    register u32 effect_slot asm("r8");
    u8 *effect;

    mask_bank = 0;
    do {
        register s32 bank_times_two asm("r0");

        side = 0;
        bank_times_two = mask_bank << 1;
        next_bank = mask_bank + 1;
        {
            register s32 bank_reload asm("r1") = mask_bank;

            asm volatile("" : "+r"(bank_reload));
            bank_mask_offset = (bank_times_two + bank_reload) << 4;
        }
        for (;;) {
            register s32 side_times_two asm("r0");
            register s32 side_source asm("r1");
            register s32 unit_slot_zero asm("r2");
            register s32 next_side_carrier asm("r2");

            asm volatile("1:");
            unit_slot_zero = 0;
            asm volatile("" : "+r"(unit_slot_zero));
            unit_slot = unit_slot_zero;
            side_source = side;
            side_times_two = side_source << 1;
            side_times_four = side_source << 2;
            next_side_carrier = side + 1;
            asm volatile("" : "+r"(next_side_carrier));
            next_side = next_side_carrier;
            side_mask_offset = (side_times_two + side) << 3;
            do {
                register u32 mask_address_work asm("r0");
                register s32 side_mask_reload asm("r1");
                register s32 unit_slot_times_four_now asm("r2");
                register s32 mask_offset asm("r3");
                register u32 *unit_mask asm("r1");
                register u8 *mask_table_reload asm("r2");

                mask_address_work = unit_slot;
                unit_slot_times_four_now = mask_address_work << 2;
                side_mask_reload = side_mask_offset;
                asm volatile("" : "+r"(side_mask_reload));
                mask_offset = unit_slot_times_four_now + side_mask_reload;
                mask_address_work = bank_mask_offset;
                unit_mask = (u32 *)(mask_offset + mask_address_work);
                mask_address_work = (u32)gBattleEffectRemovalMasks;
                unit_mask = (u32 *)((u32)unit_mask + mask_address_work);
                *unit_mask = 0;
                effect_slot = 0;
                mask_address_work = unit_slot;
                next_unit_slot = mask_address_work + 1;
                unit_slot_times_four = unit_slot_times_four_now;
                mask_table_reload = gBattleEffectRemovalMasks;
                mask_offset += (u32)mask_table_reload;
                asm volatile("" : "+r"(mask_offset));
                round_mask = (u32 *)mask_offset;
                action_mask = unit_mask;
                do {
                    register u8 *unit_record asm("r1");
                    register s32 effect_offset asm("r0");

                    asm volatile("" : "+r"(side), "+r"(unit_slot));
                    side_record_offset = (((side_times_four + side) * 8) - side) << 7;
                    unit_record_offset = (((unit_slot_times_four + unit_slot) * 8) - unit_slot) << 4;
                    unit_record = gBattleState + side_record_offset + unit_record_offset;
                    effect_offset = (effect_slot * 0xC) + BATTLE_UNIT_OFFSET(effects);
                    effect = unit_record + effect_offset;
                    if (mask_bank == BATTLE_EFFECT_REMOVAL_ROUND) {
                        if ((RollBattleStatusResistance(side, unit_slot, 0) << 24) != 0)
                            *round_mask |= 1 << effect_slot;
                    } else {
                        register s32 effect_lifetime asm("r0");
                        register s32 pilot_mobility_bonus asm("r1");
                        register u32 pilot_mobility_address asm("r1");
                        register u8 *battle_state_base asm("r2");

                        roll = (CallFunctionR0(gRandomNumberCallback) * 100U) >> 15;
                        effect_lifetime = BATTLE_EFFECT_FIELD(effect, s16, lifetime_value);
                        pilot_mobility_address = unit_record_offset + side_record_offset;
                        battle_state_base = gBattleState;
                        pilot_mobility_address += (u32)battle_state_base;
                        pilot_mobility_address += BATTLE_UNIT_OFFSET(pilot_id) + PLAYER_PILOT_OFFSET(mobility_bonus_percent);
                        pilot_mobility_bonus = *(s16 *)pilot_mobility_address;
                        if ((s32)roll > ScaleByPercent(effect_lifetime,
                                pilot_mobility_bonus + 100))
                            *action_mask |= 1 << effect_slot;
                    }
                    effect_slot = (u8)(effect_slot + 1);
                } while (effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1);
                unit_slot = (u8)next_unit_slot;
            } while (unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
            asm volatile("" : "+g"(next_side));
            side = (u8)next_side;
            asm volatile(".macro bhi target\n\tbls 1b\n\t.endm\n\t"
                         ".macro b target\n\t.endm");
            switch (side) {
            case 0:
            case 1:
                continue;
            }
            break;
        }
        asm volatile(".purgem bhi\n\t.purgem b");
        asm volatile("" : "+g"(next_bank));
        mask_bank = (u8)next_bank;
    } while ((u32)mask_bank <= BATTLE_EFFECT_REMOVAL_BANK_COUNT - 1);
}
