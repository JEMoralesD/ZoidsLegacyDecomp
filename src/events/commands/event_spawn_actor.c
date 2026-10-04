#include "m2c_prelude.h"
#include "../event_script.h"
extern s32 SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void *CreateFieldActor(u8, u8, s32, s32, s32, s32, s32, s32) asm("func_080A9D78");
extern u8 FindFieldActorSlot(s32) asm("func_080A9EF0");
extern u8 D_020218E4[];

s32 EventSpawnActor(u8 script_slot, void **cursor) {
    u8 *r5 = (u8 *)*cursor;
    s32 r4v;
    void *r0res;
    u32 var_r2;
    u8 flag;
    s32 zero;
    u8 *base;

    if (r5[1] <= 0xC) {
        flag = *(u8 *)0x020316F4;
        r4v = 8;
        if (flag == 0) {
            r4v = 0x10;
        }
        r0res = CreateFieldActor(r5[2], r5[1], (r5[3] * r4v) << 8, (r5[4] * r4v) << 8,
                              (s32)r5[5], 0, (s32)r5[6], (s32)r5[7]);
        if (r0res != 0 && ((u8 *)*cursor)[6] == 0) {
            *(void **)0x02032990 = r0res;
            if (((u8 *)r0res)[4] == 1) {
                var_r2 = 1;
                base = D_020218E4;
                zero = 0;
            loop_7:
                {
                    u8 *p = (u8 *)((var_r2 << 6) + (u32)base);
                    if (p[0x5A94] == 1 && p[0x5AC5] == 1) {
                        var_r2 = FindFieldActorSlot(0xD);
                        if (var_r2 == 0xFF) {
                            *(void **)0x02032994 = CreateFieldActor(
                                0x4B, 0xD,
                                *(s32 *)((u8 *)r0res + 8),
                                *(s32 *)((u8 *)r0res + 12),
                                (s32)((u8 *)r0res)[26],
                                zero, 8, zero);
                        }
                    } else {
                        var_r2 += 1;
                        if (var_r2 <= 0x34) {
                            goto loop_7;
                        }
                    }
                }
            }
        }
    }
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
