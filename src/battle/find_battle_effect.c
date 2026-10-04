#include "m2c_prelude.h"
#include "battle.h"

u8 FindBattleEffect(u8 side, u8 unit_slot, u16 effect_kind) {
    u8 effect_slot;
    u8 result;
    struct BattleUnit *unit;
    struct BattleEffect *effect;
    effect_slot = 0;
    unit = &((struct BattleSide *)0x02034B4C)[side].units[unit_slot];
loop_1:
    effect = &unit->effects[effect_slot];
    if ((effect->kind_flags & BATTLE_EFFECT_KIND_MASK) == effect_kind) {
        result = effect_slot;
        goto done;
    }
    effect_slot += 1;
    if ((u32) effect_slot <= (BATTLE_EFFECT_SLOT_COUNT - 1)) {
        goto loop_1;
    }
    result = BATTLE_EFFECT_NOT_FOUND;
done:
    return result;
}
