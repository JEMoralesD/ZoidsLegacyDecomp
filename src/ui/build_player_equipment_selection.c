#include "player_selection.h"
extern u8 gPlayerEquipmentSelectionCount asm("D_02032379");
extern u8 gPlayerEquipmentQuantities[] asm("D_02028218");
extern s8 gPlayerEquipmentSelectionIds[] asm("D_020322B2");

void BuildPlayerEquipmentSelection(void) asm("func_080B65A4");

void BuildPlayerEquipmentSelection(void) {
    u8 equipment_id;
    gPlayerEquipmentSelectionCount = 0;
    equipment_id = 1;
    do {
        if (gPlayerEquipmentQuantities[equipment_id] != 0) {
            gPlayerEquipmentSelectionIds[gPlayerEquipmentSelectionCount] = equipment_id;
            gPlayerEquipmentSelectionCount += 1;
        }
        equipment_id += 1;
    } while ((u32)equipment_id <= 0xC7U);
}
