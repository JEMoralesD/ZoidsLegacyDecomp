#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];

void ClearBattleUnitEffects(u8 side, u8 unit_slot) asm("func_080BE560");

void ClearBattleUnitEffects(u8 side, u8 unit_slot) {
    u8 effect_slot;
    s8 *battle_state;
    s32 unit_offset, side_offset;
    effect_slot = 0;
    battle_state = (s8 *)gBattleState;
    unit_offset = unit_slot * 0x270;
    side_offset = side * 0x1380;
    do {
        *(s16 *)((effect_slot * 0xC) + unit_offset + side_offset + (s32)battle_state + BATTLE_UNIT_OFFSET(effects[0].kind_flags)) = 0;
        effect_slot += 1;
    } while ((u32)effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1);
}

void ClearBattlePassiveEquipmentEffects(u8 side, u8 unit_slot) asm("func_080BE5A8");

void ClearBattlePassiveEquipmentEffects(u8 side, u8 unit_slot) {
    u8 effect_slot;
    s8 *unit_record;
    s32 side_offset, unit_offset, battle_state_address;
    register s32 zero asm("r0");
    effect_slot = 0;
    side_offset = side * 0x1380;
    unit_offset = unit_slot * 0x270;
    battle_state_address = 0x02034B4C;
    unit_record = (s8 *)(side_offset + (unit_offset + battle_state_address));
    do {
        struct BattleEffect *effect = (struct BattleEffect *)(unit_record + ((effect_slot * 0xC) + BATTLE_UNIT_OFFSET(effects)));
        if (effect->kind_flags & BATTLE_EFFECT_PASSIVE_EQUIPMENT) {
            zero = 0;
            effect->kind_flags = zero;
        }
        effect_slot += 1;
    } while ((u32)effect_slot <= BATTLE_EFFECT_SLOT_COUNT - 1);
}
