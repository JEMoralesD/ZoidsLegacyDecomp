#include "m2c_prelude.h"
#include "battle.h"
extern void *CreateSpriteGroup(s32, s32, s32) asm("func_8095098");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern s32 gBattleUnitPopupCallbacks[][2] asm("D_087A28B0");
extern void *gBattleUnitPopupGroups[] asm("D_02032F64");
extern void *D_02032E8C[][6];

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
    unit_sprite = D_02032E8C[popup_side][popup_unit];
    *(s32 *)((u8 *)popup + 4) = *(s16 *)((u8 *)unit_sprite + 4);
    *(s32 *)((u8 *)popup + 8) = *(s16 *)((u8 *)unit_sprite + 6);
    *(s32 *)((u8 *)popup + 0xA0) = value;
    *(s32 *)((u8 *)popup + 0xA4) = popup_side;
    *(s32 *)((u8 *)popup + 0xA8) = popup_unit;
}

void ClearBattleUnitPopups(void) asm("func_080CA114");

void ClearBattleUnitPopups(void) {
    u8 i;
    void **base;
    void **p;

    i = 0;
    base = gBattleUnitPopupGroups;
    do {
        p = (void **)((i * 4) + (s32)base);
        if (*p != 0) {
            DestroySpriteGroup(*p);
            *p = 0;
        }
        i += 1;
    } while ((u32)i <= 5U);
}
