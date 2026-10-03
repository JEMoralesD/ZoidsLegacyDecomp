#include "m2c_prelude.h"
extern void CopyBytes(int, int, int) asm("func_080ED038");
extern void func_08098284(int, int, int, int);
extern void func_08099F5C(int, int);
extern void func_080C8F48(int, void *, int);

void ShowBattleEvasionBonus(void *display_state) {
    CopyBytes(0x02030564, 0x0810817C, 8);
    func_08098284(*(int *)((char *)display_state + 0xA0), 4, 12, 0x020305E4);
    func_08099F5C(0x02030564, 0x020305E4);
    func_080C8F48(0x02030564, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleDoubleAttackPower(void *display_state) {
    func_080C8F48(0x08108184, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleHalfEvasionRate(void *display_state) {
    func_080C8F48(0x0810818C, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleHalfAttackPower(void *display_state) {
    func_080C8F48(0x08108198, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleDoubleEvasionRate(void *display_state) {
    func_080C8F48(0x081081A0, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleSensorAccuracyOverride(void *display_state) {
    CopyBytes(0x02030564, 0x081081AC, 4);
    func_08098284(*(int *)((char *)display_state + 0xA0), 4, 8, 0x020305E4);
    func_08099F5C(0x02030564, 0x020305E4);
    func_080C8F48(0x02030564, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleEnergyShieldOn(void *display_state) {
    func_080C8F48(0x081081B0, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}

void ShowBattleEnergyShieldOff(void *display_state) {
    func_080C8F48(0x081081BC, display_state, 0);
    *(int *)((char *)display_state + 0x8C) = 0;
}
