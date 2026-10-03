#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];

void ApplyBattleDerivedStatEffects(u8 side, u8 unit_slot) {
    s32 type;
    u32 offset;
    u32 row;
    u8 i;
    u8 *loop_base;
    u8 *entry;

    offset = side * 0x1380;
    row = unit_slot * 0x270;
    row += (u32)gBattleState;
    offset += row;
    i = 0;
    loop_base = (u8 *)offset;
    do {
        entry = &loop_base[i * 0xC + 0xE4];
        type = *(u16 *)&entry[4] & BATTLE_EFFECT_KIND_MASK;
        if (type != BATTLE_EFFECT_INITIATIVE) {
            if (type == BATTLE_EFFECT_EVASION_SCORE && !(*(u32 *)entry & BATTLE_CONDITION_EVASION_MASK)) {
                *(u16 *)((u8 *)offset + 0xC) += *(u16 *)&entry[6];
            }
        } else {
            *(u16 *)((u8 *)offset + 0xA) += *(u16 *)&entry[6];
        }
        i++;
    } while (i <= (BATTLE_EFFECT_SLOT_COUNT - 1));
}
