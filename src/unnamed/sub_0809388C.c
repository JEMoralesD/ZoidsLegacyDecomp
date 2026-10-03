#include "m2c_prelude.h"

s16 Sin256(s16) asm("func_8092A90");
s16 Cos256(s16) asm("func_8092ADC");
void func_80934DC(s16 *, s16 *, s16 *);

void sub_0809388C(s16 *angles, s16 *output) {
    s16 matrices[4][16];
    register s16 *second asm("r5");
    s16 *third;
    s16 *combined;
    s32 zero;
    s32 one;

    {
        register s16 *first_base asm("r0");

        first_base = matrices[0];
        asm volatile("" : "+r"(first_base));
        zero = 0;
        one = 0x100;
        first_base[0] = one;
        first_base[3] = zero;
        first_base[6] = zero;
        first_base[1] = zero;
    }
    matrices[0][4] = Cos256(angles[0]);
    matrices[0][7] = -Sin256(angles[0]);
    matrices[0][2] = zero;
    matrices[0][5] = Sin256(angles[0]);
    matrices[0][8] = Cos256(angles[0]);

    {
        s16 value;

        value = Cos256(angles[1]);
        second = matrices[1];
        second[0] = value;
    }
    second[3] = zero;
    second[6] = Sin256(angles[1]);
    second[1] = zero;
    second[4] = one;
    second[7] = zero;
    second[2] = -Sin256(angles[1]);
    second[5] = zero;
    second[8] = Cos256(angles[1]);

    {
        s16 value;

        value = Cos256(angles[2]);
        third = matrices[2];
        third[0] = value;
    }
    third[3] = -Sin256(angles[2]);
    third[6] = zero;
    third[1] = Sin256(angles[2]);
    third[4] = Cos256(angles[2]);
    third[7] = zero;
    third[2] = zero;
    third[5] = zero;
    third[8] = one;

    combined = matrices[3];
    func_80934DC(third, second, combined);
    func_80934DC(combined, matrices[0], output);
}
