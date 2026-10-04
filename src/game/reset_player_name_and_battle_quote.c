#include "m2c_prelude.h"
extern void BiosCpuSet(void *, s32, s32) asm("func_080ECD2C");
extern void CopyString(s32, s32) asm("func_080ED128");
extern s32 gPilotNameTable[] asm("D_087EDFB4");
extern s32 gPlayerNameBuffer asm("D_02021774");
extern s32 gDefaultPlayerBattleQuote asm("D_080248A0");

void ResetPlayerNameAndBattleQuote(void) asm("func_08099F8C");

void ResetPlayerNameAndBattleQuote(void) {
    s32 zero = 0;
    s32 text_destination = (s32)&gPlayerNameBuffer;
    BiosCpuSet(&zero, text_destination, 0x05000010);
    CopyString(text_destination, gPilotNameTable[1]);
    text_destination += 0x12;
    CopyString(text_destination, (s32)&gDefaultPlayerBattleQuote);
}
