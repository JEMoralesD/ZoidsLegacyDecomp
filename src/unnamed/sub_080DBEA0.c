#include "m2c_prelude.h"
extern s32 Sin256(u8) asm("func_08092A90");
extern s32 Cos256(u8) asm("func_08092ADC");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void *func_080D2450();
extern void *func_080D2660();
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern s32 BiosArcTan2(s32, s32) asm("func_080ECD24");
extern u16 BiosSqrt(s32) asm("func_080ECD3C");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 D_03000010;

void sub_080DBEA0(void *arg0) {
    char *owner = arg0;
    register s32 *p8C asm("r4") = (s32 *)(owner + 0x8C);
    register s32 state asm("r1") = *p8C;
    register s32 index asm("r8");

    if (state == 0) {
        register s32 *children asm("r10");
        register u32 *rng asm("r9");
        s32 *state_slot;
        void *created;
        register s32 zero asm("r0");

        created = func_080D2450(owner, 0, 0,
            *(s16 *)(owner + 4), (s32)*(s16 *)(owner + 8),
            0x500, state, state);
        *(void **)(owner + 0xC) = created;
        zero = 0;
        index = zero;
        state_slot = p8C;
        children = (s32 *)(owner + 0xC);
        rng = &D_03000010;
        do {
            register s32 value_r6 asm("r6");
            register s32 value_r5 asm("r5");
            register s32 value_r4 asm("r4");
            register s32 scaled asm("r0");
            register s32 base_angle asm("r2");
            register s32 perturb asm("r1");
            void *spawned;

            {
                register u32 *rng1 asm("r1") = rng;
                register u32 random asm("r0");
                register s32 index_low asm("r1");
                random = CallFunctionR0(*rng1);
                index_low = index;
                __asm__ volatile ("" : "+r" (index_low));
                base_angle = index_low << 4;
                perturb = (random * 0x11) >> 15;
                perturb -= 8;
                base_angle += perturb;
                base_angle <<= 24;
                value_r6 = (u32)base_angle >> 24;
            }
            {
                register s32 cosine asm("r0");
                cosine = Sin256(value_r6);
                cosine = (s16)cosine;
                value_r5 = (cosine * 2) + cosine;
            }
            value_r4 = Cos256(value_r6);
            __asm__ volatile ("" : "+r" (value_r4));
            value_r4 <<= 16;
            value_r4 >>= 16;
            {
                register s32 half asm("r0");
                half = value_r5 + ((u32)value_r5 >> 31);
                half >>= 1;
                value_r4 -= half;
            }
            value_r6 = (u32)(BiosArcTan2((s16)value_r4,
                (s16)value_r5) << 16) >> 24;
            value_r4 = (u16)BiosSqrt(
                (value_r4 * value_r4) + (value_r5 * value_r5));
            {
                register u32 *rng_view asm("r2") = rng;
                scaled = value_r4 *
                    ((CallFunctionR0(*rng_view) * 0x81) >> 15);
            }
            if (scaled < 0) {
                scaled += 0xFF;
            }
            value_r4 -= scaled >> 8;
            spawned = func_080D2660(owner, 1, 0,
                *(s16 *)(owner + 4), (s32)*(s16 *)(owner + 8),
                0x500, value_r6, value_r4, 0);
            {
                register s32 next asm("r2") = index;
                s32 child_off;
                next += 1;
                child_off = next << 2;
                *(s32 *)((char *)children + child_off) = (s32)spawned;
                next <<= 24;
                next = (u32)next >> 24;
                index = next;
            }
        } while ((u32)index <= 0xF);
        PlayBattleAnimationSound(0);
        *state_slot = *state_slot + 1;
        return;
    }
    {
        register s32 zero asm("r0") = 0;
        index = zero;
        if (*(s32 *)(owner + 0xC) == 0) {
            s32 *children = (s32 *)(owner + 0xC);
            s32 off;
            do {
                register s32 next asm("r0");
                next = index + 1;
                next <<= 24;
                next = (u32)next >> 24;
                index = next;
                if ((u32)next > 0x10) {
                    break;
                }
                off = next << 2;
            } while (*(s32 *)((char *)children + off) == 0);
        }
        {
            register s32 final_index asm("r1") = index;
            if (final_index == 0x11) {
                DestroySpriteGroup(owner);
            }
        }
    }
}
