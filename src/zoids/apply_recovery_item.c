#include "m2c_prelude.h"
#include "../battle/battle.h"

struct RecoveryUnitHealth {
    u8 pad0[6];
    u16 hp;
    u8 pad1[50];
    s16 max_hp;
};

u8 FindBattleEffect(u8, u8, s32) asm("func_080BF464");
void RemoveBattleEffect(u8, u8, u8) asm("func_080BF514");

void ApplyRecoveryItem(s32 recovery_kind, struct RecoveryUnitHealth *unit, s32 side_arg, s32 unit_slot_arg)
{
    register struct RecoveryUnitHealth *health asm("r4") = unit;
    register s32 v asm("r0");
    s32 effect_slot;
    u16 max_hp;
    u8 side;
    u8 unit_slot;

    recovery_kind = (u8)recovery_kind;
    side = side_arg;
    unit_slot = unit_slot_arg;
    switch (recovery_kind) {
    case BATTLE_RECOVER_HP_300:
        {
            register s32 t asm("r1") = 300;
            asm volatile("" : "+r"(t));
            v = t;
        }
        goto add_value;
    case BATTLE_RECOVER_HP_150:
        v = health->hp;
        v += 150;
        goto store_value;
    case BATTLE_RECOVER_HP_50:
        v = health->hp;
        v += 50;
        goto store_value;
    case BATTLE_RECOVER_HALF_MAX_HP:
        v = health->max_hp;
        v /= 2;
    add_value:
        {
            register u16 t asm("r3") = health->hp;
            v += t;
        }
    store_value:
        health->hp = v;
        v = (s16)v;
        max_hp = health->max_hp;
        if (v > health->max_hp) {
            health->hp = max_hp;
        }
        break;
    case BATTLE_RECOVER_FREEZE:
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_FREEZE);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        break;
    case BATTLE_RECOVER_STATUS:
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_FREEZE);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_CONFUSION);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_PILOT_INACTIVE);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        break;
    case BATTLE_RECOVER_FULL:
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_FREEZE);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_CONFUSION);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        effect_slot = FindBattleEffect(side, unit_slot, BATTLE_EFFECT_PILOT_INACTIVE);
        effect_slot = (u8)effect_slot;
        RemoveBattleEffect(side, unit_slot, effect_slot);
        health->hp = health->max_hp;
        break;
    case BATTLE_RECOVER_TOWN_TRAVEL:
    case BATTLE_RECOVER_EVACUATION:
        break;
    }
}
