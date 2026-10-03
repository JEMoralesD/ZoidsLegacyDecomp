#include "m2c_prelude.h"
#include "battle.h"

M2C_UNK RemoveBattleEffect(u8, u8, u8) asm("func_080BF514");
M2C_UNK RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");
extern u8 gBattleState;


void RemoveBattleAttackEffects(s32 side_arg, s32 unit_slot_arg) {
    u8 effect_slot;
    u8 unit_slot;
    u8 side;
    struct BattleEffect *effect;

    side_arg = side_arg << 24;
    side = (u32)side_arg >> 24;
    unit_slot_arg = unit_slot_arg << 24;
    unit_slot = (u32)unit_slot_arg >> 24;
    if ((IsBattleUnitActive(side, unit_slot) << 24) != 0) {
        effect_slot = 0;
        do {
            effect = &((struct BattleSide *)&gBattleState)[side].units[unit_slot].effects[effect_slot];
            if (effect->kind_flags != 0 &&
                (BATTLE_EFFECT_LIFETIME_MASK & effect->kind_flags) == BATTLE_EFFECT_UNTIL_ATTACK_END) {
                RemoveBattleEffect(side, unit_slot, effect_slot);
            }
            effect_slot += 1;
        } while ((u32)effect_slot <= (BATTLE_EFFECT_SLOT_COUNT - 1));
        RecalculateBattleUnitStats(side, unit_slot);
    }
}
