#include "player_selection.h"
extern u8 gPlayerZoidSelectionCount asm("D_02032272");
extern u8 gPlayerZoidSelectionSlots[] asm("D_020321A4");
extern u8 gBattleState[];
extern u16 D_02030564;
extern u8 D_02030566[];
extern void D_081061C4;
extern u32 gZoidNameTable[];

extern void CopyBytes(void *, void *, int) asm("func_80ED038");
extern void AppendString(void *, void *) asm("func_08099F5C");
extern void AppendWindowTextItem(int, void *) asm("func_080988C8");

void AppendBattleUnitSelectionRows(u8 window_id, u16 reject_flags) asm("func_080B44A0");

void AppendBattleUnitSelectionRows(u8 window_id, u16 reject_flags)
{
    u8 i;
    u8 *unit_record;
    u8 *zoid_name;

    i = 0;
    while (i < gPlayerZoidSelectionCount) {
        unit_record = &gBattleState[gPlayerZoidSelectionSlots[i] * sizeof(struct BattleUnit)];
        if ((reject_flags & ZOID_SELECTION_REJECT_WITHOUT_PILOT) && BATTLE_UNIT_FIELD(unit_record, u8, pilot_slot_or_definition_id) == 0)
            goto rejected;
        if ((reject_flags & ZOID_SELECTION_REJECT_DESTROYED) && (BATTLE_UNIT_FIELD(unit_record, u16, flags) & 8))
            goto rejected;
        if ((reject_flags & ZOID_SELECTION_REJECT_IN_TEAM) && (BATTLE_UNIT_FIELD(unit_record, u16, flags) & 4))
            goto rejected;
        if (reject_flags & ZOID_SELECTION_REJECT_TEMPORARY_PAIR) {
            if (reject_flags & ZOID_SELECTION_ALLOW_TEMPORARY_ALTERNATE_FORM) {
                if (BATTLE_UNIT_FIELD(unit_record, u8, form_flags) & 0xfe)
                    goto after_field_check;
            }
            if (BATTLE_UNIT_FIELD(unit_record, u16, flags) & 2)
                goto rejected;
        }
    after_field_check:
        if ((reject_flags & ZOID_SELECTION_REJECT_SIZE_S) && BATTLE_UNIT_FIELD(unit_record, u8, size_class) == 0)
            goto rejected;
        if ((reject_flags & ZOID_SELECTION_REJECT_SIZE_M) && BATTLE_UNIT_FIELD(unit_record, u8, size_class) == 1)
            goto rejected;
        if ((reject_flags & ZOID_SELECTION_REJECT_SIZE_L) && BATTLE_UNIT_FIELD(unit_record, u8, size_class) == 2)
            goto rejected;
        if ((reject_flags & ZOID_SELECTION_REJECT_SIZE_XL) && BATTLE_UNIT_FIELD(unit_record, u8, size_class) == 3)
            goto rejected;
        if ((reject_flags & ZOID_SELECTION_REJECT_SIZE_DOUBLE_ICON) && BATTLE_UNIT_FIELD(unit_record, u8, size_class) == 4)
            goto rejected;

        D_02030564 = 1;
        goto dispatch;
    rejected:
        {
            register u16 *rejected_dest_r1 asm("r1") = &D_02030564;
            register u32 rejected_r0 asm("r0") = 0x401;
            asm volatile("" : "+r"(rejected_dest_r1));
            asm volatile("" : "+r"(rejected_r0));
            *rejected_dest_r1 = rejected_r0;
        }
    dispatch:
        CopyBytes(D_02030566, &D_081061C4, 3);
        zoid_name = (u8 *)gZoidNameTable[BATTLE_UNIT_FIELD(unit_record, u8, zoid_id)];
        AppendString(D_02030566, zoid_name);
        AppendWindowTextItem(window_id, D_02030566 - 2);

        i++;
    }
}
