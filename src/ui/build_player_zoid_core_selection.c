#include "player_selection.h"
void BuildPlayerZoidCoreSelection(void) asm("func_080B6728");

void BuildPlayerZoidCoreSelection(void) {
    void *core_id_as_address;

    *(u8 *)PLAYER_CORE_SELECTION_COUNT_RAM = 0;
    core_id_as_address = (void *)1;
    do {
        if (M2C_FIELD(core_id_as_address, u8 *, 0x020217FE) != 0) {
            M2C_FIELD(*(u8 *)PLAYER_CORE_SELECTION_COUNT_RAM, s8 *, PLAYER_CORE_SELECTION_IDS_RAM) = (s8) core_id_as_address;
            *(u8 *)PLAYER_CORE_SELECTION_COUNT_RAM += 1;
        }
        core_id_as_address = (void *) (u8) (core_id_as_address + 1);
    } while ((u32) core_id_as_address <= 0x59U);
}
