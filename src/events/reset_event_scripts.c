#include "m2c_prelude.h"
#include "event_script.h"

void InitializeEventSpritePool(void) asm("func_0809F850");

void ResetEventScripts(void) {
    register u32 index asm("r2");
    register u8 *secondary_flag asm("r3");
    register s32 *state asm("r4");
    register u8 *third_flag asm("r6");
    u8 *fourth_flag;

    *(u8 *)0x02030664 = 0;
    index = 0;
    secondary_flag = (u8 *)0x02030666;
    asm volatile("" : "+r"(secondary_flag));
    state = (s32 *)0x02031744;
    asm volatile("" : "+r"(state));
    third_flag = (u8 *)0x02031749;
    asm volatile("" : "+r"(third_flag));
    fourth_flag = (u8 *)0x020316F6;
    asm volatile("" : "+r"(fourth_flag));
    {
        register s32 zero asm("r0");
        register s32 *script_cursors asm("r1");

        zero = 0;
        script_cursors = (s32 *)0x020314C4;
        do {
            *script_cursors++ = zero;
            index += 1;
        } while (index <= 69);
    }

    *secondary_flag = 0;
    *state = 0;
    index = 0;
    {
        register u8 *grid asm("r5");
        register s32 *row_flags asm("r4");
        register s32 zero asm("r3");

        grid = (u8 *)0x02030668;
        asm volatile("" : "+r"(grid));
        zero = 0;
        row_flags = (s32 *)0x02031468;
        asm volatile("" : "+r"(row_flags));
        do {
            register u32 inner asm("r1");
            register s32 row_offset asm("r0");
            register u8 *entry asm("r0");

            inner = 0;
            asm volatile("" : "+r"(grid));
            row_offset = index << 8;
            entry = (u8 *)(row_offset + (s32)grid);
            do {
                *(s16 *)entry = zero;
                entry += 8;
                inner += 1;
            } while (inner <= 31);
            *row_flags++ = zero;
            index += 1;
        } while (index <= 13);
    }

    *third_flag = 0;
    *fourth_flag = 0;
    index = 0;
    {
        register u8 *flag_conditions asm("r3");
        register s32 one asm("r1");

        flag_conditions = (u8 *)0x020316FC;
        one = 1;
        do {
            register u8 *condition asm("r0");

            condition = (u8 *)(index + (s32)flag_conditions);
            *condition = one;
            index += 1;
        } while (index <= 69);
    }
    InitializeEventSpritePool();
}
