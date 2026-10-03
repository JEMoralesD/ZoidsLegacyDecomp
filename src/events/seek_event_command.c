#include "m2c_prelude.h"
#include "event_script.h"

extern u8 gEventCommandLengths[];

void SeekEventCommand(u16 script_slot, s32 target_opcode, u8 choice_id)
{
    u8 **cursor;
    u32 slot_offset;
    u8 *command_lengths;
    u8 nesting[4];
    u8 depth;
    u8 terminators;
    u16 selector_class;
    register s32 selector_shifted asm("r5");
    s32 selector_value;

    slot_offset = (u32)script_slot << 2;
    cursor = (u8 **)0x020314C4;
    asm volatile("" : "+r"(cursor));
    cursor = (u8 **)(slot_offset - (0 - (u32)cursor));
    terminators = 0;
    depth = 0;
    selector_shifted = (u32)(u16)target_opcode << 16;
    selector_value = selector_shifted >> 16;
    selector_class = (u32)(selector_shifted + 0xFFF10000) >> 16;
    command_lengths = gEventCommandLengths;
    goto check_selector_class;

decrement_terminators:
    terminators--;

check_selected_opcode:
{
    register s32 selected_selector asm("r1");

    if (selector_value == EVENT_CHOICE_CASE && (*cursor)[0] == EVENT_CHOICE_CASE) {
        if (depth == 1) {
            if ((*cursor)[1] == choice_id)
                return;
        } else if (--nesting[depth - 1] == 0) {
            depth--;
        }
    }

    selected_selector = selector_shifted >> 16;
    if (selected_selector == EVENT_SCAN_NEXT || (*cursor)[0] == selected_selector) {
        if ((u16)(selected_selector - EVENT_BRANCH_TRUE) <= 1)
            goto scan_terminators;
        if (selected_selector != EVENT_CHOICE_CASE)
            return;
    }
}

check_selector_class:
    if (selector_class > 1)
        goto check_nesting;

scan_terminators:
    if ((*cursor)[0] == EVENT_YES_NO || (*cursor)[0] == EVENT_IF_PARTY_RESTRICTION ||
        (*cursor)[0] == 0x1A || (*cursor)[0] == 0x1B ||
        (*cursor)[0] == 0x1C)
        terminators++;

check_nesting:
    if (selector_value == EVENT_CHOICE_CASE && (*cursor)[0] == EVENT_CHOICE) {
        nesting[depth] = 0;
        depth++;
    }

    switch ((*cursor)[0]) {
    case EVENT_START_SCRIPTS: {
        u32 i;
        u8 count = (*cursor)[1];

        *cursor += 2;
        i = 0;
        while (i < count) {
            *cursor += 4;
            i++;
        }
        break;
    }
    case EVENT_TEXT:
        *cursor += 1;
        if ((*cursor)[0] != 0) {
            do {
                if ((*cursor)[0] == 1)
                    *cursor += 1;
                else if ((*cursor)[0] == 2)
                    *cursor += 2;
                else if ((*cursor)[0] == 0xD && depth != 0)
                    nesting[depth - 1]++;
                *cursor += 1;
            } while ((*cursor)[0] != 0);
        }
        *cursor += 1;
        break;
    default:
        *cursor += command_lengths[(*cursor)[0]];
        break;
    }

    {
        register s32 tail_selector asm("r1");

        asm volatile("" : "+r"(selector_shifted));
        tail_selector = selector_shifted >> 16;
        if ((u32)(selector_shifted + 0xFFF10000) >> 16 > 1)
            goto check_selected_opcode;
        if ((*cursor)[0] != tail_selector)
            goto check_selected_opcode;
        if (terminators == 1)
            return;
    }
    goto decrement_terminators;
}
