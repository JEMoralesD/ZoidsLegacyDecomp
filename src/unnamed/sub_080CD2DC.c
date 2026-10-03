#include "m2c_prelude.h"

extern u8 gBattleSetup[];
extern u16 D_0300004E;
extern u16 D_03000050;
extern u32 D_03000010;
extern u8 D_02033F36;

void UpdateBattleAnimation(void) asm("func_080D1B78");
void UpdateBattleAnimationCamera(void) asm("func_080D12DC");
void UpdateBattleScanlineWindow(void) asm("func_080D1E58");
void UpdateBattleBackgroundShake(void) asm("func_080D2244");
void UpdateZoidEquipmentAnimation(void) asm("func_080D0644");
u32 CallFunctionR0(u32) asm("func_080ECD5C");
void *CreateSpriteFromTable(u32, u32, u32, s32, s32, s32, s32, s32, s32) asm("func_08094374");
void func_080ED17C(s32);

void sub_080CD2DC(void) {
    void *slots[8];
    u8 i;

    for (i = 0; i <= 7; i++) {
        slots[i] = 0;
    }

    for (;;) {
        UpdateBattleAnimation();
        UpdateBattleAnimationCamera();
        UpdateBattleScanlineWindow();
        UpdateBattleBackgroundShake();
        UpdateZoidEquipmentAnimation();

        if (gBattleSetup[2] == 0xFD) {
            D_0300004E = 0x740;
            D_03000050 = 0x810;

            for (i = 0; i <= 7; i++) {
                if (slots[i] != 0 && (*(u32 *)slots[i] & 1) == 0) {
                    slots[i] = 0;
                }
            }

            for (i = 0; i <= 7; i++) {
                register s32 offset asm("r5");
                register s32 offset_source asm("r0");
                register void *entry asm("r1");

                offset_source = i * 4;
                entry = *(void **)((u8 *)slots + offset_source);
                offset = offset_source;
                if (entry == 0) {
                    register s32 x asm("r4");
                    register s32 x_source asm("r1");
                    register u32 random asm("r0");
                    register s32 layer asm("r2");
                    register s32 priority asm("r1");
                    register s32 x_raw asm("r0");

                    random = CallFunctionR0(D_03000010);
                    asm volatile(".short 0x0041, 0x1809, 0x0109"
                                 : "=r"(x_source) : "r"(random));
                    x_source -= random;
                    x_source <<= 4;
                    x_source = (u32)x_source >> 15;
                    if (D_02033F36 == 0) {
                        x_raw = (x_source - 0x200) << 16;
                    } else {
                        x_raw = x_source << 16;
                    }
                    x = x_raw >> 16;

                    random = CallFunctionR0(D_03000010);
                    random >>= 14;
                    layer = 0xC8;
                    if (random != 0) {
                        layer = 0x48;
                    }
                    if (D_02033F36 != 0) {
                        priority = 0x8400;
                    } else {
                        priority = 0x400;
                    }
                    priority |= layer;

                    entry = CreateSpriteFromTable(0x087AFA94, 9, 0, x,
                        0xC0, 0x180, 3, priority, 0x080CD259);
                    *(void **)((u8 *)slots + offset) = entry;
                    asm volatile("" : : "r"(offset));
                    break;
                }
            }
        }

        func_080ED17C(1);
    }
}
