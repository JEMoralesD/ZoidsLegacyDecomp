#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState;

enum BattleEffectLowKindBits {
    BATTLE_EFFECT_LOW_KIND_BITS_TO_CLEAR = 0x17
};

void ClearBattleEffectsByLowKindBits(u8 side, u8 unit_slot) asm("func_080BF804");

void ClearBattleEffectsByLowKindBits(u8 side, u8 unit_slot) {
    u8 effect_slot;
    struct BattleEffect *effect;

    effect_slot = 0;
    do {
        effect = &((struct BattleSide *)&gBattleState)[side].units[unit_slot].effects[effect_slot];
        if (BATTLE_EFFECT_LOW_KIND_BITS_TO_CLEAR & effect->kind_flags) {
            effect->kind_flags = 0;
        }
        effect_slot += 1;
    } while ((u32)effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1);
}
