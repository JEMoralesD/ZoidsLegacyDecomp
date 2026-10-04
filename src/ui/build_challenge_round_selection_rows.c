#include "player_selection.h"

void *GetWindow(s32) asm("func_0809716C");
void FormatNumberText(s32, s32, s32, s32) asm("func_08098284");
void AppendWindowTextItem(s32, s32) asm("func_080988C8");
void RunMenuScript(s32) asm("func_08098BB4");
void AppendString(s32, s32) asm("func_08099F5C");
void CopyBytes(void *, s32, s32) asm("func_080ED038");

void BuildChallengeRoundSelectionRows(void) asm("func_080E4948");

void BuildChallengeRoundSelectionRows(void) {
    register s32 round_index asm("r4");
    register s16 *line asm("r6");
    s16 *output;
    register u8 *selectable_rounds asm("r8");
    register s32 one asm("r9");
    register u8 *round_completion_flags asm("r10");
    register u8 *scratch asm("r5");

    RunMenuScript(0x0801765B);
    round_index = 0;
    round_completion_flags = (u8 *)CHALLENGE_ROUND_COMPLETION_RAM;
    line = (s16 *)0x02030564;
    one = 1;
    selectable_rounds = (u8 *)0x02032E83;
    output = line + 1;
loop:
    {
        register s32 option asm("r1");
        option = *(u8 *)CHALLENGE_SELECTED_COURSE_RAM;
        option = option * 5;
        if (round_completion_flags[round_index + option] != 0) {
            *line = 1;
            *(u8 *)(round_index + (s32)selectable_rounds) = one;
            CopyBytes(line + 1, 0x08109398, 3);
        } else {
            register s32 previous asm("r0");
            if (round_index == 0) {
                goto enabled_case;
            }
            previous = option - 1;
            previous = round_completion_flags[round_index + previous];
            if (previous != 0) {
enabled_case:
                *line = 1;
                *(u8 *)(round_index + (s32)selectable_rounds) = one;
            } else {
                register s32 disabled asm("r1");
                register s32 zero asm("r0");
                zero = 0;
                disabled = 0x401;
                *line = disabled;
                *(u8 *)(round_index + (s32)selectable_rounds) = zero;
            }
            CopyBytes((void *)0x02030566, 0x0810939C, 3);
        }
    }
    {
        register s32 destination asm("r0");
        register s32 text asm("r1");
        destination = (s32)output;
        asm volatile("" : "+r"(destination));
        text = 0x081093A0;
        AppendString(destination, text);
    }
    round_index++;
    scratch = (u8 *)0x020305E4;
    FormatNumberText(round_index, 2, 2, (s32)scratch);
    AppendString((s32)output, (s32)scratch);
    {
        register s32 call0 asm("r0");
        register s32 call1 asm("r1");
        call0 = 2;
        call1 = (s32)(output - 1);
        AppendWindowTextItem(call0, call1);
    }
    round_index <<= 24;
    round_index = (u32)round_index >> 24;
    if ((u32)round_index <= 4) {
        goto loop;
    }
    ((u8 *)GetWindow(2))[22] = *(u8 *)0x02032E78;
}
