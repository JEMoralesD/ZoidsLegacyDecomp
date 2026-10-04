#include "m2c_prelude.h"
#include "battle_display.h"
extern u8 gBattleState[];
extern u8 *gBattleSceneSelectedUnit asm("D_02033F38");
extern s16 gBattleEquipmentCursorPositions[] asm("D_087EC38C");

void MoveBattleSelectionCursorTo(s32, s16, s16) asm("func_080CC400");

void PositionBattleEquipmentSelectionCursor(void) asm("func_080CC44C");

void PositionBattleEquipmentSelectionCursor(void) {
    s16 *equipment_cursor_positions;
    u8 *battle_state;
    u8 *equipment_slots;
    u8 action_index;
    s32 equipment_position_offset;
    equipment_cursor_positions = gBattleEquipmentCursorPositions;
    battle_state = gBattleState;
    equipment_slots = battle_state + BATTLE_SELECTION_OFFSET(equipment_slots);
    action_index = battle_state[BATTLE_SELECTION_OFFSET(action_count)];
    equipment_position_offset = (equipment_slots[action_index] << 2) + (*gBattleSceneSelectedUnit << 5);
    MoveBattleSelectionCursorTo(0, *(s16 *)((s8 *)equipment_cursor_positions + equipment_position_offset), *(s16 *)((s8 *)(equipment_cursor_positions += 1) + equipment_position_offset));
}
