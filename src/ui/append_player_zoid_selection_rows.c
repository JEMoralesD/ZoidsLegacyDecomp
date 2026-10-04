#include "player_selection.h"
extern u8 gPlayerZoidSelectionCount asm("D_02032272");
extern u8 gPlayerZoidSelectionSlots[] asm("D_020321A4");
extern struct PlayerZoidRecordView gPlayerZoidRecords[] asm("D_020218E8");
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u16 gPlayerSelectionTextPrefix asm("D_02030564");
extern u8 gPlayerSelectionRowText[] asm("D_02030566");
extern void gPlayerSelectionSpaceText asm("D_081061C4");
extern u32 gZoidNameTable[];

extern void CopyBytes(void *, void *, int) asm("func_80ED038");
extern void AppendString(void *, void *) asm("func_08099F5C");
extern void AppendWindowTextItem(int, void *) asm("func_080988C8");

void AppendPlayerZoidSelectionRows(u8 window_id, u16 reject_filters) asm("func_080AC214");

void AppendPlayerZoidSelectionRows(u8 window_id, u16 reject_filters)
{
    u8 selection_index;
    struct PlayerZoidRecordView *zoid;
    u8 *zoid_name;

    selection_index = 0;
    while (selection_index < gPlayerZoidSelectionCount) {
        zoid = &gPlayerZoidRecords[gPlayerZoidSelectionSlots[selection_index]];
        if ((reject_filters & ZOID_SELECTION_REJECT_WITHOUT_PILOT) && zoid->pilot_slot == 0)
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_DESTROYED) && (zoid->flags & PLAYER_ZOID_DESTROYED))
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_IN_TEAM) && (zoid->flags & PLAYER_RECORD_IN_TEAM))
            goto rejected;
        if (reject_filters & ZOID_SELECTION_REJECT_TEMPORARY_PAIR) {
            if (reject_filters & ZOID_SELECTION_ALLOW_TEMPORARY_ALTERNATE_FORM) {
                if (zoid->form_flags & 0xfe)
                    goto after_field_check;
            }
            if (zoid->flags & PLAYER_RECORD_TEMPORARY_PAIR)
                goto rejected;
        }
    after_field_check:
        if ((reject_filters & 0x20) && (zoid->flags & 0x10))
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_SIZE_S) && zoid->size_class == ZOID_SIZE_CLASS_S)
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_SIZE_M) && zoid->size_class == ZOID_SIZE_CLASS_M)
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_SIZE_L) && zoid->size_class == ZOID_SIZE_CLASS_L)
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_SIZE_XL) && zoid->size_class == ZOID_SIZE_CLASS_XL)
            goto rejected;
        if ((reject_filters & ZOID_SELECTION_REJECT_SIZE_DOUBLE_ICON) && zoid->size_class == ZOID_SIZE_CLASS_DOUBLE_ICON)
            goto rejected;
        if (reject_filters & ZOID_SELECTION_REJECT_PROTAGONIST) {
            s32 player_state_address = (s32)gPlayerStateBytes;
            s32 pilot_record_address = zoid->pilot_slot << 6;

            pilot_record_address += player_state_address;
            pilot_record_address += PLAYER_STATE_OFFSET(pilots);
            if (*(u8 *)pilot_record_address == PLAYER_PROTAGONIST_PILOT_ID)
                goto rejected;
        }

        gPlayerSelectionTextPrefix = PLAYER_SELECTION_NORMAL_TEXT_PREFIX;
        goto dispatch;
    rejected:
        {
            u16 *text_prefix_address = &gPlayerSelectionTextPrefix;
            u32 rejected_text_prefix = PLAYER_SELECTION_REJECTED_TEXT_PREFIX;
            *text_prefix_address = rejected_text_prefix;
        }
    dispatch:
        CopyBytes(gPlayerSelectionRowText, &gPlayerSelectionSpaceText, 3);
        zoid_name = (u8 *)gZoidNameTable[zoid->model_id];
        AppendString(gPlayerSelectionRowText, zoid_name);
        AppendWindowTextItem(window_id, gPlayerSelectionRowText - 2);

        selection_index++;
    }
}
