#include "m2c_prelude.h"
#include "battle_popup.h"
extern void *CreateSpriteGroup(s32, s32, s32) asm("func_8095098");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern s32 gBattleUnitPopupCallbacks[][2] asm("D_087A28B0");
extern void *gBattleUnitPopupGroups[] asm("D_02032F64");
extern void *gBattleUnitSprites[][6] asm("D_02032E8C");

void StartBattleUnitPopup(s32 popup_kind, s32 side, s32 unit_index, s32 value) asm("func_080CA0AC");

void StartBattleUnitPopup(s32 popup_kind, s32 side, s32 unit_index, s32 value) {
    u8 popup_side;
    u8 popup_unit;
    void *popup;
    void *unit_sprite;
    u32 callback_index;

    popup_kind = popup_kind << 0x10;
    popup_side = side;
    popup_unit = unit_index;
    callback_index = (u32)popup_kind >> 0x10;
    popup = CreateSpriteGroup(0, gBattleUnitPopupCallbacks[callback_index][0], gBattleUnitPopupCallbacks[callback_index][1]);
    gBattleUnitPopupGroups[popup_unit] = popup;
    unit_sprite = gBattleUnitSprites[popup_side][popup_unit];
    BATTLE_POPUP_FIELD(popup, s32, x) = BATTLE_SPRITE_FIELD(unit_sprite, s16, x);
    BATTLE_POPUP_FIELD(popup, s32, y) = BATTLE_SPRITE_FIELD(unit_sprite, s16, y);
    BATTLE_POPUP_FIELD(popup, s32, value) = value;
    BATTLE_POPUP_FIELD(popup, s32, side) = popup_side;
    BATTLE_POPUP_FIELD(popup, s32, unit_index) = popup_unit;
}

void ClearBattleUnitPopups(void) asm("func_080CA114");

void ClearBattleUnitPopups(void) {
    u8 unit_index;
    void **popup_slots;
    void **popup_slot;

    unit_index = 0;
    popup_slots = gBattleUnitPopupGroups;
    do {
        popup_slot = (void **)((unit_index * 4) + (s32)popup_slots);
        if (*popup_slot != 0) {
            DestroySpriteGroup(*popup_slot);
            *popup_slot = 0;
        }
        unit_index += 1;
    } while ((u32)unit_index <= 5U);
}
