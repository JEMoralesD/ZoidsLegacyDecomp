#include "m2c_prelude.h"
#include "../event_script.h"
extern int SeekEventCommand(u8, int, int) asm("func_080A016C");
extern void *gEventScriptStarts[];

s32 EventIfPlayerInRect(u8 script_slot, void **cursor) {
    register u8 *temp_r4 asm("r4");
    register int temp_r5 asm("r5");
    register int temp_r1 asm("r1");
    register int var_r0 asm("r0");
    register int temp_r4_2 asm("r4");
    u8 *temp_r2;
    int var_r3;
    int neg1;
    int var_flag;

    temp_r4 = *(u8 **)0x02032990;
    if (temp_r4 != 0) {
        var_flag = *(u8 *)0x020316F4;
        var_r3 = 8;
        if (var_flag == 0) {
            var_r3 = 0x10;
        }
        if (temp_r4[4] != 0x6C) {
            temp_r1 = *(s32 *)(temp_r4 + 8);
            var_r0 = temp_r1;
            if (temp_r1 < 0) {
                var_r0 += 0xFF;
            }
            temp_r5 = var_r0 >> 8;
            temp_r2 = *(u8 **)cursor;
            if ((temp_r5 >= (int)(temp_r2[1] * var_r3)) && (temp_r5 <= (int)((temp_r2[3] * var_r3) + var_r3))) {
                temp_r1 = *(s32 *)(temp_r4 + 12);
                var_r0 = temp_r1;
                if (temp_r1 < 0) {
                    var_r0 += 0xFF;
                }
                temp_r4_2 = var_r0 >> 8;
                if ((temp_r4_2 >= (int)(temp_r2[2] * var_r3)) && (temp_r4_2 <= (int)((temp_r2[4] * var_r3) + var_r3))) {
                    if (*(u8 *)0x020316F7 != 0) {
                        neg1 = -1;
                        goto mid_16;
                    top_16:
                        SeekEventCommand(script_slot, neg1, 0);
                    mid_16:
                        SeekEventCommand(script_slot, neg1, 0);
                        if (((u8 *)*cursor)[0] == 0x18) {
                            goto top_16;
                        }
                    block_17:
                        return EVENT_CONTINUE;
                    }
                    goto block_19;
                }
                goto block_18;
            }
            goto block_18;
        }
    block_18:
        *(u8 *)0x020316F7 = 1;
    block_19:
        ;
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    if (((u8 *)*cursor)[0] == 0x18) {
        goto block_17;
    }
    *cursor = gEventScriptStarts[script_slot];
    return EVENT_YIELD;
}
