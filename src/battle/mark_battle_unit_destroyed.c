#include "m2c_prelude.h"
#include "popups/battle_popup.h"
M2C_UNK RemoveBattleUnitFromTurnOrder(u8, u8) asm("func_080C02B4");
M2C_UNK StartBattleUnitPopup(s32, u8, u8, s32) asm("func_080CA0AC");
extern s8 gBattleState;

void MarkBattleUnitDestroyed(u8 side, u8 unit_slot) asm("func_080C04DC");

void MarkBattleUnitDestroyed(u8 side, u8 unit_slot) {
    u8 side_byte;
    u8 unit_slot_byte;
    void *unit_record;
    s32 unit_address;
    register u16 unit_flags asm("r2");
    register u16 destroyed_flags asm("r0");

    side_byte = side;
    unit_slot_byte = unit_slot;
    unit_record = side_byte * 0x1380;
    unit_address = unit_slot_byte * 0x270;
    unit_address += (s32)&gBattleState;
    unit_record += unit_address;
    unit_flags = BATTLE_UNIT_FIELD(unit_record, u16, flags);
    destroyed_flags = BATTLE_UNIT_DESTROYED;
    destroyed_flags |= unit_flags;
    BATTLE_UNIT_FIELD(unit_record, u16, flags) = destroyed_flags;
    RemoveBattleUnitFromTurnOrder(side_byte, unit_slot_byte);
    StartBattleUnitPopup(BATTLE_POPUP_DESTROYED_UNIT, side_byte, unit_slot_byte, 0);
}
