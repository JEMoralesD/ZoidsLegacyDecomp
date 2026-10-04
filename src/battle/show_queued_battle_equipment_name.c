#include "m2c_prelude.h"
#include "battle_display.h"

extern u8 gBattleState[];
extern s32 gEquipmentNameTable[] asm("D_087EE170");
extern void PrintWindowTextAt(s32, s32, u8, s32, s32) asm("func_080981F0");
extern void RunMenuScript(s32) asm("func_08098BB4");

void ShowQueuedBattleEquipmentName(u8 action_index_arg) asm("func_080CCE24");

void ShowQueuedBattleEquipmentName(u8 action_index_arg) {
    u8 action_index;
    s32 *equipment_names;
    struct BattleQueuedEquipmentStateView *queued_equipment_state;
    s32 acting_unit_address;
    u8 equipment_slot;
    u16 equipment_id;
    s32 equipment_name;

    action_index = action_index_arg;
    switch (action_index) {
    case 0:
        RunMenuScript(BATTLE_QUEUED_EQUIPMENT_OPEN_FIRST_MENU);
        break;
    case 1:
        RunMenuScript(BATTLE_QUEUED_EQUIPMENT_OPEN_SECOND_MENU);
        break;
    case 2:
        RunMenuScript(BATTLE_QUEUED_EQUIPMENT_OPEN_THIRD_MENU);
        break;
    }
    equipment_names = gEquipmentNameTable;
    queued_equipment_state = (struct BattleQueuedEquipmentStateView *)gBattleState;
    acting_unit_address = queued_equipment_state->acting_unit_address;
    equipment_slot = queued_equipment_state->equipment_slots[action_index];
    equipment_id = *(u16 *)(acting_unit_address + (equipment_slot << 2) + BATTLE_UNIT_OFFSET(equipment[0].item_id));
    equipment_name = equipment_names[equipment_id];
    PrintWindowTextAt(equipment_name, 0, (u8)(action_index + BATTLE_QUEUED_EQUIPMENT_FIRST_WINDOW), 0, 0);
}
