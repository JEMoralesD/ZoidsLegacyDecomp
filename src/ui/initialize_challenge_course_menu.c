#include "player_selection.h"
#include "../graphics/screen_effects.h"

void InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC");
void LoadMenuGradientBackground(s32, s32, s32, s32, s32) asm("func_0809AB44");
void RunMenuScript(s32) asm("func_08098BB4");
u8 TestEventFlag(void) asm("func_0809F818");
void CopyBytes(void *, s32, s32) asm("func_080ED038");
void FormatNumberText(s32, s32, s32, s32) asm("func_08098284");
void AppendString(s32, s32) asm("func_08099F5C");
void AppendWindowTextItem(s32, s32) asm("func_080988C8");
void *GetWindow(s32) asm("func_0809716C");
void ClearSpritePools(void) asm("func_08094330");
void StartScreenTransition(s32, s32) asm("func_08096308");

void InitializeChallengeCourseMenu(void) asm("func_080E47EC");

void InitializeChallengeCourseMenu(void) {
    register s32 course_index asm("r4");
    register s32 next asm("r5");
    register u16 *line asm("r8");
    register u8 *round_completion_flags asm("r9");
    u8 *scratch;

    *(u16 *)0x0300004C = 0x1140;
    InitializeWindowGraphics(0, 1, 0, 0x3C0, 0x3C0, 0, 14, 0, 0x3E6, 15);
    LoadMenuGradientBackground(1, 3, 0x40, 13, 24);
    RunMenuScript(0x080175A8);
    course_index = 0;
    line = (u16 *)0x02030564;
    round_completion_flags = (u8 *)CHALLENGE_ROUND_COMPLETION_RAM;
    asm volatile("" : "+r"(round_completion_flags));
    scratch = (u8 *)0x020305E4;
    do {
        u8 status;
        u8 flag;
        {
            register u8 *fp asm("r0");
            register u8 *fa asm("r1");
            fp = (u8 *)0x087A3EE4;
            asm volatile("" : "+r"(fp));
            fa = (u8 *)(course_index + (s32)fp);
            asm volatile("" : "+r"(fa));
            flag = *fa;
        }
        if (flag == 0 || (status = TestEventFlag()) != 0) {
            u8 one;
            register u8 *p asm("r0");
            register u16 *lp asm("r2");
            p = (u8 *)0x02032E79;
            asm volatile("" : "+r"(p));
            p = (u8 *)(course_index + (s32)p);
            *p = (one = 1);
            lp = line;
            asm volatile("" : "+r"(lp));
            *lp = one;
        } else {
            register u8 *q asm("r0");
            register s32 v401 asm("r0");
            register u16 *lq asm("r3");
            q = (u8 *)0x02032E79;
            asm volatile("" : "+r"(q));
            q = (u8 *)(course_index + (s32)q);
            *q = status;
            v401 = 0x401;
            lq = line;
            asm volatile("" : "+r"(lq));
            *lq = v401;
        }
        {
            register s32 n asm("r1") = 0;
            s32 offset = course_index * 5;
            u8 value = *(u8 *)(offset + (s32)round_completion_flags);
            register u16 *output asm("r6") = (u16 *)0x02030566;
            next = course_index + 1;
            if (value != 0) {
                u8 *b = (u8 *)CHALLENGE_ROUND_COMPLETION_RAM;
                do {
                    register s32 t asm("r0");
                    t = n + 1;
                    t <<= 24;
                    n = (u32)t >> 24;
                    if ((u32)n > 4) break;
                } while (*(u8 *)((n + offset) + (s32)b) != 0);
            }
            if (n == 5) {
                CopyBytes(output, 0x08109378, 13);
            } else {
                CopyBytes(output, 0x08109388, 13);
            }
        }
        {
            register u16 *output2 asm("r4");
            FormatNumberText(next, 2, 2, (s32)scratch);
            output2 = (u16 *)0x02030566;
            AppendString((s32)output2, (s32)scratch);
            {
                register s32 call0 asm("r0");
                register s32 call1 asm("r1");
                call0 = 1;
                call1 = (s32)(output2 - 1);
                AppendWindowTextItem(call0, call1);
            }
        }
        {
            register s32 sh asm("r0");
            sh = next << 24;
            course_index = (u32)sh >> 24;
        }
    } while ((u32)course_index <= 9);
    ((u8 *)GetWindow(1))[22] = *(u8 *)CHALLENGE_SELECTED_COURSE_RAM;
    ClearSpritePools();
    StartScreenTransition(SCREEN_TRANSITION_CHECKERBOARD_REVEAL, 0);
}
