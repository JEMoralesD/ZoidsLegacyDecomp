#include "m2c_prelude.h"
#include "battle.h"

M2C_UNK ConfigureBattleUnitSprites(u8, u8, u8, u8, s32, s32, s32) asm("func_080BAF2C");
M2C_UNK ClearBattleUnitEffects(u8, u8) asm("func_080BE560");
M2C_UNK RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
M2C_UNK ApplyBattlePassiveEquipmentEffects(u8, u8) asm("func_080E90AC");
extern u8 gBattleState[];

void ReviveBattleUnit(u8 side, u8 unit_slot) asm("func_080C052C");

void ReviveBattleUnit(u8 side, u8 unit_slot) {
    u8 side_byte;
    u8 unit_slot_byte;
    u8 *unit_record;
    u32 side_offset;
    u32 unit_address;
    u16 unit_flags;
    u16 restored_hp;
    s32 revived_flags;
    s32 zero;

    side_byte = side;
    unit_slot_byte = unit_slot;
    side_offset = side_byte * 0x1380;
    unit_address = unit_slot_byte * 0x270;
    unit_address += (u32)gBattleState;
    unit_record = (u8 *)(side_offset + unit_address);
    unit_flags = BATTLE_UNIT_FIELD(unit_record, u16, flags);
    if (unit_flags & BATTLE_UNIT_DESTROYED) {
        restored_hp = BATTLE_UNIT_FIELD(unit_record, u16, max_hp);
        zero = 0;
        BATTLE_UNIT_FIELD(unit_record, u16, hp) = restored_hp;
        revived_flags = 0xFFFF & ~BATTLE_UNIT_DESTROYED;
        revived_flags &= unit_flags;
        BATTLE_UNIT_FIELD(unit_record, u16, flags) = revived_flags;
        ClearBattleUnitEffects(side_byte, unit_slot_byte);
        ApplyBattlePassiveEquipmentEffects(side_byte, unit_slot_byte);
        RecalculateBattleUnitStats(side_byte, unit_slot_byte);
        ConfigureBattleUnitSprites(BATTLE_UNIT_FIELD(unit_record, u8, zoid_id), BATTLE_UNIT_FIELD(unit_record, u8, palette_variant), BATTLE_UNIT_FIELD(unit_record, u8, size_class), side_byte,
                     unit_slot_byte, zero, zero);
    }
}
