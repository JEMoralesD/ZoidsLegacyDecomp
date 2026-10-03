#include "m2c_prelude.h"
extern void BiosCpuSet(void *, s32, s32) asm("func_080ECD2C");
extern void CopyString(s32, s32) asm("func_080ED128");
extern s32 D_087EDFB4[];
extern s32 D_02021774;
extern s32 D_080248A0;

void sub_08099F8C(void) {
    s32 subroutine_arg0 = 0;
    s32 var_r4 = (s32)&D_02021774;
    BiosCpuSet(&subroutine_arg0, var_r4, 0x05000010);
    CopyString(var_r4, D_087EDFB4[1]);
    var_r4 += 0x12;
    CopyString(var_r4, (s32)&D_080248A0);
}
