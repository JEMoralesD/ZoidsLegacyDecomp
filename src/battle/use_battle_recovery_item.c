#include "m2c_prelude.h"
#include "battle.h"

extern void QueueBattleEffectDisplay(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE9D8");
extern void ApplyRecoveryItem(u8, void *, u8, u8) asm("func_080E66C8");

void UseBattleRecoveryItem(s32 recovery_kind_arg, s32 side_arg, s32 unit_slot_arg) {
    s32 side_offset;
    s32 unit_offset;
    u32 temp_r0;
    u8 unit_slot;
    u8 side;
    u8 recovery_kind;
    void *unit;

    recovery_kind_arg <<= 24;
    recovery_kind = (u32)recovery_kind_arg >> 24;
    side_arg <<= 24;
    side = (u32)side_arg >> 24;
    unit_slot_arg <<= 24;
    unit_slot = (u32)unit_slot_arg >> 24;
    side_offset = side * 0x1380;
    unit_offset = unit_slot * 0x270;
    unit_offset += 0x02034B4C;
    unit = (void *)(side_offset + unit_offset);
    temp_r0 = recovery_kind - 1;
    switch (temp_r0) {
    case 0:
        QueueBattleEffectDisplay(side, unit_slot, -1, 0, 0, 0, 1, 0x12C, 0, 0);
        break;
    case 1:
        QueueBattleEffectDisplay(side, unit_slot, -1, 0, 0, 0, 1, 0x96, 0, 0);
        break;
    case 2:
        QueueBattleEffectDisplay(side, unit_slot, -1, 0, 0, 0, 1, 0x32, 0, 0);
        break;
    case 3:
        QueueBattleEffectDisplay(side, unit_slot, -1, 0, 0, 0, 1,
            ({ s32 value = M2C_FIELD(unit, s16 *, 0x3A);
               (s32)(value + ((u32)value >> 31)) >> 1; }), 0, 0);
        break;
    case 6:
        QueueBattleEffectDisplay(side, unit_slot, -1, 0, 0, 0, 1,
            (s32) (s16) ((u16) M2C_FIELD(unit, s16 *, 0x3A) - M2C_FIELD(unit, u16 *, 6)), 0, 0);
        break;
    case 8:
        break;
    }
    ApplyRecoveryItem(recovery_kind, unit, side, unit_slot);
}
