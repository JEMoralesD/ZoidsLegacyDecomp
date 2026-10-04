#include "m2c_prelude.h"
#include "battle_animation.h"

struct BattleWindowSpriteGroup {
    u32 flags;
    u8 pad_04[8];
    struct BattleWindowSpriteGroup *children[16];
};

void ClearHBlankCallback(s32) asm("func_080942E0");

extern volatile u16 D_0300004C;
extern volatile u16 gDisplayWindowOutsideLayers asm("D_03005EFA");
extern volatile u16 D_0400004A;
extern volatile u16 D_03000052;
extern volatile u16 D_0300004E;
extern volatile u16 D_03000050;
extern struct BattleWindowSpriteGroup *D_02033F3C;
extern volatile u8 D_03000074;
extern volatile u8 gBattleScanlineWindowMode asm("D_02034863");
extern struct BattleWindowSpriteGroup *D_02033F40[3];

void StopBattleScanlineWindow(void) asm("func_080D1D5C");

void StopBattleScanlineWindow(void)
{
    register u32 zero asm("r4");

    {
        register volatile u16 *interrupt asm("r0") =
            (volatile u16 *)0x04000208;

        zero = 0;
        *interrupt = zero;
    }
    ClearHBlankCallback(1);
    D_0300004C = (D_0300004C & 0x7DFF) | 0x400;
    asm volatile("" : : : "memory");
    {
        register volatile u16 *mirror asm("r3") = &gDisplayWindowOutsideLayers;
        register volatile u16 *source asm("r1") = &D_0400004A;

        asm volatile("" : "+r"(mirror) : : "memory");
        *source &= 0xFF;
        *mirror = *source;
    }
    if (D_03000052 != 0x10) {
        D_0300004E = zero;
    }
    D_03000050 = zero;

    {
        register u32 index asm("r2") = 0;
        register struct BattleWindowSpriteGroup **owner_slot asm("r5") = &D_02033F3C;
        register volatile u8 *status asm("r6") = &D_03000074;
        volatile u8 *window_mode = &gBattleScanlineWindowMode;
        register struct BattleWindowSpriteGroup **objects asm("r4") = D_02033F40;
        register u32 clear_mask asm("r3") = 0xFFEFFBFF;

        do {
            struct BattleWindowSpriteGroup *object = objects[index];

            if (object != 0) {
                object->flags &= clear_mask;
            }
            {
                register u32 next asm("r0") = index + 1;

                next <<= 24;
                index = next >> 24;
            }
        } while (index <= 2);

        if (*owner_slot != 0) {
            register u32 child_index asm("r2") = 0;
            register struct BattleWindowSpriteGroup **owner_view asm("r4") = owner_slot;
            register u32 child_mask asm("r3") = 0xFFEFFBFF;

            do {
                struct BattleWindowSpriteGroup *owner = *owner_view;
                struct BattleWindowSpriteGroup *child = owner->children[child_index];

                if (child != 0) {
                    child->flags &= child_mask;
                }
                {
                    register u32 next asm("r0") = child_index + 1;

                    next <<= 24;
                    child_index = next >> 24;
                }
            } while (child_index <= 0xF);
        }

        *status &= 0xFB;
        *(volatile u16 *)0x04000208 = 1;
        *window_mode = 0;
    }
}
