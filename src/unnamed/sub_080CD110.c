#include "m2c_prelude.h"

extern u16 D_0300004C;
extern u16 D_0300004E;
extern u16 D_03000050;
extern u32 D_03000054[];
extern u8 gBattleState[];
extern u8 D_000027BE[];
extern u8 D_02033F36;
extern u16 D_087A2A1C[];
extern void *D_087A2A10[];
extern u8 D_08108228[];
extern u8 D_0600A740[];
extern u8 D_05000100[];
extern u8 D_08108878[];
extern u8 gSaveBuffer[];
extern u8 D_080CCF3D[];
extern u8 D_080CD05D[];
extern u8 D_00000442[];

void BiosLz77ToVram(void *, void *) asm("func_080ECD34");
void func_0809AC30(void *, u8, u8, void *);
u32 *CreateSpriteGroup(s32, void *, void *) asm("func_08095098");
void PlaySong(s32) asm("func_08092E84");
void func_080ED17C(s32);
void DestroySpriteGroup(void *) asm("func_08095114");

void sub_080CD110(void) {
    register u8 *base asm("r4");
    register u8 *state asm("r5");
    register u32 *control asm("r6");
    u32 *object;
    register u32 frame asm("r5");
    register u16 *table asm("r1");
    register s32 id asm("r0");
    register s32 offset asm("r3");

    D_0300004C |= 0x200;
    *(u16 *)0x0400000A = 0x170B;
    control = D_03000054;
    control[2] = 0;
    table = D_087A2A1C;
    base = gBattleState;
    state = &D_02033F36;
    id = *state;
    offset = (s32)D_000027BE;
    base += offset;
    id += (s32)base;
    id = *(u8 *)id;
    id -= 0x2E;
    control[3] = table[id];
    BiosLz77ToVram(D_08108228, D_0600A740);
    BiosLz77ToVram(D_087A2A10[*(u8 *)(*state + (s32)base) - 0x2E], D_05000100);
    func_0809AC30(D_08108878, 0x17, *state, gSaveBuffer);
    object = CreateSpriteGroup(0, D_080CCF3D, D_080CD05D);
    object[0x23] = *(u8 *)(*state + (s32)base) - 0x2E;
    {
        u16 *setup = &D_0300004E;
        register u32 value asm("r2") = (u32)D_00000442;
        u32 copy = value;
        asm volatile("" : "+r"(value));
        *setup = copy;
    }
    D_03000050 = 0x10;
    PlaySong(0x6B);

    frame = 0;
    do {
        if (D_02033F36 == 0) {
            u32 current = control[2];
            s32 delta = -0x1000;
            control[2] = current + delta;
        } else {
            u32 current = control[2];
            s32 delta;
            delta = 0x1000;
            asm volatile("" : "+r"(delta));
            control[2] = current + delta;
        }
        if (frame > 0x77) {
            D_03000050 = (((frame - 0x78) >> 1) << 8) |
                          ((0x98 - frame) >> 1);
        }
        func_080ED17C(1);
    } while (++frame <= 0x97);
    D_0300004C &= 0xFDFF;
    D_0300004E = 0;
    DestroySpriteGroup(object);
}
