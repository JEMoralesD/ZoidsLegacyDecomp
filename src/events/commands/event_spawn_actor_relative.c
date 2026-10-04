#include "m2c_prelude.h"
#include "../event_script.h"
extern s32 SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern void *CreateFieldActor(u8, u8, s32, s32, s32, s32, s32, s32) asm("func_080A9D78");
extern u8 FindFieldActorSlot(s32) asm("func_080A9EF0");
extern u8 D_020218E4[];
extern s32 D_020325A0[][0x12];

s32 EventSpawnActorRelative(s32 script_slot, void **cursor) {
    register void **saved_cursor asm("r9") = cursor;
    u8 *input;
    u8 *pre;
    u8 arg0n;
    s32 scale;
    void *result;
    u32 index;
    u8 flag;
    s32 zero;
    u8 *base;

    arg0n = (u8)script_slot;
    pre = (u8 *)*cursor;

    if (pre[1] <= 0xC) {
        flag = *(u8 *)0x020316F4;
        scale = 8;
        if (flag == 0) {
            scale = 0x10;
        }
        index = FindFieldActorSlot(pre[3]);
        if (index != 0xFF) {
            input = (u8 *)*saved_cursor;
            result = CreateFieldActor(input[2], input[1],
                D_020325A0[index][2] + ((input[4] * scale) << 8),
                D_020325A0[index][3] + ((input[5] * scale) << 8),
                (s32)input[6], 0, (s32)input[7], 0);
            if (result != 0) {
                register void **check_arg1 asm("r1") = saved_cursor;
                if (((u8 *)*check_arg1)[7] != 0) {
                    goto done;
                }
                *(void **)0x02032990 = result;
                if (((u8 *)result)[4] == 1) {
                    index = 1;
                    base = D_020218E4;
                    zero = 0;
                loop_7:
                    {
                        u8 *p = (u8 *)((index << 6) + (u32)base);
                        if (p[0x5A94] == 1 && p[0x5AC5] == 1) {
                            index = FindFieldActorSlot(0xD);
                            if (index == 0xFF) {
                                *(void **)0x02032994 = CreateFieldActor(
                                    0x4B, 0xD,
                                    *(s32 *)((u8 *)result + 8),
                                    *(s32 *)((u8 *)result + 12),
                                    (s32)((u8 *)result)[26],
                                    zero, 8, zero);
                            }
                        } else {
                            index += 1;
                            if (index <= 0x34) {
                                goto loop_7;
                            }
                        }
                    }
                }
            }
        }
    }
done:
    SeekEventCommand(arg0n, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
