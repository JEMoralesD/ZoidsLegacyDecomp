#include "m2c_prelude.h"

extern u8 gBattleState[];

void AddBattleEffect(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080BE65C");
s32 FindAbilityValue(void *, s32, s32) asm("func_080E74F0");
void RecalculateBattleUnitStats(u8, u8) asm("func_080E8B08");
s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");

void sub_080BFEA4(void) {
    s16 values[2];
    u8 blocked[2];
    u8 outer;
    u8 inner;
    u8 *blocked_base;

    outer = 0;
    blocked_base = blocked;
    {
        u8 *base = gBattleState;

        do {
            s16 *value = &values[outer];
            u8 *flag = &blocked_base[outer];
            s32 zero = 0;

            asm volatile("" : "+r"(zero));
            *flag = zero;
            *value = zero;
            inner = 0;
            do {
                if ((IsBattleUnitActive(outer, inner) << 24) != 0) {
                    register s16 *current_value asm("r2");
                    register u32 record_addr asm("r0");

                    current_value = &values[outer];
                    asm volatile("" : "+r"(current_value));
                    record_addr = inner * 0x270;
                    asm volatile("" : "+r"(record_addr));
                    record_addr += outer * 0x1380;
                    asm volatile("" : "+r"(record_addr));
                    record_addr += (u32)base;
                    asm volatile("" : "+r"(record_addr));

                    {
                        register s32 old_value asm("r1");
                        u16 new_value;

                        old_value = *current_value;
                        asm volatile("" : "+r"(old_value));
                        new_value = *(u16 *)(record_addr + 10);
                        if (old_value < *(s16 *)(record_addr + 10)) {
                            *current_value = new_value;
                        }
                    }
                }
                inner++;
            } while (inner <= 5);
            outer++;
        } while (outer <= 1);
    }

    outer = 0;
    do {
        s32 next_outer;
        u16 *value;

        inner = 0;
        next_outer = outer + 1;
        value = (u16 *)&values[outer];
        do {
            if ((IsBattleUnitActive(outer, inner) << 24) != 0) {
                u8 *effects = gBattleState
                    + outer * 0x1380 + inner * 0x270 + 0x70;

                if ((FindAbilityValue(effects, 12, 0) << 16) != 0) {
                    *value += 200;
                }
                if ((FindAbilityValue(effects, 13, 0) << 16) != 0) {
                    *value += 500;
                }
                if ((FindAbilityValue(effects, 14, 0) << 16) != 0) {
                    blocked_base[outer] = 1;
                }
            }
            inner++;
        } while (inner <= 5);
        outer = next_outer;
    } while (outer <= 1);

    if (values[0] / 2 >= values[1] && blocked_base[1] == 0) {
        outer = 0;
    } else if (values[1] / 2 >= values[0] && blocked_base[0] == 0) {
        outer = 1;
    } else {
        return;
    }

    {
        s32 zero;
        register u8 *base asm("r8");

        inner = 0;
        zero = 0;
        base = gBattleState;
        asm volatile("" : "+r"(base));
        do {
            if ((IsBattleUnitActive(outer, inner) << 24) != 0) {
                AddBattleEffect(outer, inner, -1, 0,
                    zero, zero, 13,
                    ({
                        register u32 field_ptr asm("r0");

                        field_ptr = inner * 0x270;
                        asm volatile("" : "+r"(field_ptr));
                        field_ptr += outer * 0x1380;
                        asm volatile("" : "+r"(field_ptr));
                        field_ptr += (u32)base;
                        asm volatile("" : "+r"(field_ptr));
                        *(s16 *)(field_ptr + 0x44);
                    }),
                    1, zero);
                RecalculateBattleUnitStats(outer, inner);
            }
            inner++;
        } while (inner <= 5);
    }
}
