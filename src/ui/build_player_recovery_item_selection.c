#include "player_selection.h"
extern u8 gRecoveryItemSelectionCount asm("D_020322B1");
extern u8 gPlayerRecoveryItemQuantities[] asm("D_020217F4");
extern u8 gRecoveryItemSelectionIds[] asm("D_020322A8");

void BuildPlayerRecoveryItemSelection(void) asm("func_080B654C");

void BuildPlayerRecoveryItemSelection(void) {
    u8 item_id;
    u8 item_order_index;

    gRecoveryItemSelectionCount = 0;
    item_order_index = 1;
    do {
        if ((u32) (u8) (item_order_index - 1) <= 2U) {
            item_id = 4 - item_order_index;
        } else {
            item_id = item_order_index;
        }
        if (gPlayerRecoveryItemQuantities[item_id] != 0) {
            gRecoveryItemSelectionIds[gRecoveryItemSelectionCount] = item_id;
            gRecoveryItemSelectionCount += 1;
        }
        item_order_index += 1;
    } while ((u32) item_order_index <= 9U);
}
