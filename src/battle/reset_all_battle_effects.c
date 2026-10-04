#include "m2c_prelude.h"
#include "battle.h"

M2C_UNK ClearBattleUnitEffects(u8, u8) asm("func_080BE560");
extern u8 gBattleState;

void ResetAllBattleEffects(void) asm("func_080BE600");

void ResetAllBattleEffects(void) {
    u8 side;
    u8 unit_slot;
    s32 next_side;
    s32 side_stride_work;
    s32 side_offset;
    s32 unit_address;
    u8 *battle_state;

    side = 0;
    battle_state = &gBattleState;
    do {
        unit_slot = 0;
        side_stride_work = side << 2;
        next_side = side + 1;
        side_stride_work += side;
        side_stride_work <<= 3;
        side_stride_work -= side;
        side_offset = side_stride_work << 7;
        do {
            ClearBattleUnitEffects(side, unit_slot);
            unit_address = unit_slot << 2;
            unit_address += unit_slot;
            unit_address <<= 3;
            unit_address -= unit_slot;
            unit_address <<= 4;
            unit_address += side_offset;
            unit_address += (s32)battle_state;
            BATTLE_UNIT_FIELD(unit_address, u16, effect_allocation_cursor) = 0;
            unit_slot += 1;
        } while ((u32)unit_slot <= BATTLE_ACTIVE_UNIT_COUNT - 1);
        side = next_side;
    } while ((u32)side <= BATTLE_SIDE_COUNT - 1);
}
