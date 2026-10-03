#include "m2c_prelude.h"

extern u8 gBattleSetup[];
extern u8 gBattleState[];

s32 func_080ECD98(s32, s32);

u8 sub_080BB660(void) {
    u8 counts[2];
    s32 sums[2];
    u8 i;
    u8 j;
    s16 result;

    if (gBattleSetup[0] != 2) {
        for (i = 0; i < 2; i++) {
            s32 *sump = &sums[i];
            u8 *cntp = &counts[i];
            s32 z = 0;
            asm volatile("" : "+r"(z));
            *cntp = z;
            *sump = z;
            for (j = 0; j < 6; j++) {
                u8 *entry = &gBattleState[i * 0x1380 + j * 0x270];
                if (entry[0] != 0) {
                    sums[i] += *(s16 *)(entry + 10);
                    counts[i]++;
                }
            }
            {
                s32 *fp = &sums[i];
                u8 c = counts[i];
                *fp = func_080ECD98(*fp, c);
            }
        }
        {
            s32 a = sums[0];
            s32 b = sums[1];
            result = 50;
            if (a > b) {
                result = 90;
            }
        }
        {
            s32 d;
            {
                register s32 c0 asm("r0");
                c0 = counts[0];
                asm volatile("" : "+r"(c0));
                d = c0;
            }
            result += (d - counts[1]) * 10;
        }
        result = (s16)result;
        {
            u8 *p = gBattleState;
            s32 w = p[0x270C] * 10;
            s32 v = result + w;
            asm volatile("" :: "r"(p));
            result = v;
            if ((s16)v <= 0) {
                result = 1;
            }
        }
        if (result > 99) {
            result = 99;
        }
    } else {
        result = 100;
    }
    return result;
}
