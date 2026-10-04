#include "player_selection.h"
M2C_UNK PlaySong(s32) asm("func_08092E84");

extern u16 gRepeatedKeysForLeft[] asm("gRepeatedKeys_A");
extern u16 gRepeatedKeysForRight[] asm("gRepeatedKeys_B");
extern u16 gRepeatedKeysForL[] asm("gRepeatedKeys_C");
extern u16 gRepeatedKeysForR[] asm("gRepeatedKeys_D");

s32 UpdateMenuQuantityFromKeys(s32 quantity, s32 maximum_quantity) asm("func_080B684C");

s32 UpdateMenuQuantityFromKeys(s32 quantity, s32 maximum_quantity) {
    register s32 updated_quantity asm("r4");
    register s32 quantity_limit asm("r5");

    updated_quantity = quantity;
    quantity_limit = maximum_quantity;
    if (gRepeatedKeysForLeft[0] & MENU_QUANTITY_KEY_LEFT) {
        if (updated_quantity > 1) {
            updated_quantity--;
        } else {
            updated_quantity = quantity_limit;
        }
        PlaySong(MENU_QUANTITY_CHANGE_SOUND);
    }
    if (gRepeatedKeysForRight[0] & MENU_QUANTITY_KEY_RIGHT) {
        if (updated_quantity < quantity_limit) {
            updated_quantity++;
        } else {
            updated_quantity = 1;
        }
        PlaySong(MENU_QUANTITY_CHANGE_SOUND);
    }
    if (gRepeatedKeysForL[0] & MENU_QUANTITY_KEY_L) {
        if (updated_quantity > 10) {
            updated_quantity -= 10;
        } else if (updated_quantity > 1) {
            updated_quantity = 1;
        } else {
            updated_quantity = quantity_limit;
        }
        PlaySong(MENU_QUANTITY_CHANGE_SOUND);
    }
    if (gRepeatedKeysForR[0] & MENU_QUANTITY_KEY_R) {
        if (updated_quantity < quantity_limit - 10) {
            updated_quantity += 10;
        } else if (updated_quantity < quantity_limit) {
            updated_quantity = quantity_limit;
        } else {
            updated_quantity = 1;
        }
        PlaySong(MENU_QUANTITY_CHANGE_SOUND);
    }
    return updated_quantity;
}
