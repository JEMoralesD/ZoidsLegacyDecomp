#include "m2c_prelude.h"
#include "battle.h"

void RemoveBattleEffect(u8, u8, u8) asm("func_080BF514");
void RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");
extern u8 gBattleState[];
extern u8 gBattleEffectRemovalMasks[];

void UpdateBattleRoundEffects(void) {
    s32 next_side;
    s32 next_unit;
    u8 side;
    u8 unit_slot;
    u8 effect_slot;

    side = 0;
    do {
    unit_slot = 0;
    next_side = side + 1;
    do {
        s32 active = IsBattleUnitActive(side, unit_slot) << 24;
        next_unit = unit_slot + 1;
        if (active != 0) {
            register u32 side_times_four asm("r10");
            register u32 unit_times_four asm("r8");
            register u8 *base asm("r9");

            effect_slot = 0;
            side_times_four = side << 2;
            unit_times_four = unit_slot << 2;
            base = gBattleState;
            do {
                u8 *unit = (u8 *)(((((side_times_four + side) << 3) - side) << 7)
                    + (((((unit_times_four + unit_slot) << 3) - unit_slot) << 4) + (u32)base));
                u8 *effect = unit + ((effect_slot * 0xC) + 0xE4);
                u16 kind_flags = *(u16 *)(effect + 4);

                if (kind_flags != 0) {
                    switch (kind_flags & BATTLE_EFFECT_LIFETIME_MASK) {
                    case BATTLE_EFFECT_TURN_EP_COST:
                        break;
                    case BATTLE_EFFECT_ROUND_COUNTDOWN: {
                        u16 turns_remaining = *(u16 *)(effect + 8);
                        if (*(s16 *)(effect + 8) != 0) {
                            u32 next_turns = turns_remaining - 1;
                            *(u16 *)(effect + 8) = next_turns;
                            if ((next_turns << 16) == 0) {
                                RemoveBattleEffect(side, unit_slot, effect_slot);
                            }
                        }
                        break;
                    }
                    case BATTLE_EFFECT_ROUND_MASK:
                        if ((*(u32 *)(((side * 0x18) + unit_times_four) + (u32)gBattleEffectRemovalMasks)
                             & (1 << effect_slot)) != 0) {
                            RemoveBattleEffect(side, unit_slot, effect_slot);
                        }
                        break;
                    case BATTLE_EFFECT_ROUND_EP_COST: {
                        u16 ep_cost = *(u16 *)(effect + 8);
                        s16 signed_ep_cost = *(s16 *)(effect + 8);
                        if (signed_ep_cost >= 0) {
                            u16 available_ep = *(u16 *)(unit + 8);
                            if (*(s16 *)(unit + 8) < signed_ep_cost) {
                                RemoveBattleEffect(side, unit_slot, effect_slot);
                            } else {
                                *(u16 *)(unit + 8) = available_ep - ep_cost;
                            }
                        } else {
                            *(u16 *)(effect + 8) = -ep_cost;
                        }
                        break;
                    }
                    }
                }
                effect_slot++;
            } while ((u32)effect_slot <= (BATTLE_EFFECT_SLOT_COUNT - 1));
            RecalculateBattleUnitStats(side, unit_slot);
        }
        unit_slot = (u8)next_unit;
    } while ((u32)unit_slot <= 5);
    {
        s32 t = next_side;
        asm volatile("" : "+r"(t));
        side = (u8)t;
    }
    } while ((u32)side <= 1);
}
