#include "m2c_prelude.h"
#include "battle_animation.h"

struct BattleWindowSpriteGroup {
    u32 flags;
    u8 pad_04[8];
    struct BattleWindowSpriteGroup *children[16];
};

extern void SetHBlankCallback(s32, s32) asm("func_08094290");
extern void StopBattleScanlineWindow(void) asm("func_080D1D5C");

extern volatile u8 gBattleScanlineWindowDisplayRequest asm("D_02034862");
extern volatile u8 gBattleScanlineWindowBufferIndex asm("D_02034868");
extern volatile u16 D_0300004C;
extern volatile u16 D_0300004E;
extern volatile u16 D_03000050;
extern volatile u16 gDisplayWindowOutsideLayers asm("D_03005EFA");
extern struct BattleWindowSpriteGroup *D_02033F3C;
extern struct BattleWindowSpriteGroup *D_02033F40[3];

void UpdateBattleScanlineWindowDisplay(s32 begin_frame) asm("func_080D2050");

void UpdateBattleScanlineWindowDisplay(s32 begin_frame)
{
    u8 i;
    u8 status;

    if ((begin_frame << 24) != 0) {
        status = gBattleScanlineWindowDisplayRequest;
        if (status == BATTLE_SCANLINE_WINDOW_DISPLAY_START) {
            *(volatile u16 *)0x04000208 = 0;
            SetHBlankCallback(1, 0x0300605C);
            {
                register volatile u16 *mirror asm("r2") = &gDisplayWindowOutsideLayers;
                register volatile u16 *source asm("r1") =
                    (volatile u16 *)0x0400004A;
                register u32 value asm("r3") = 0x3E3F;

                asm volatile("" : "+r"(value));
                *source = ({
                    register u32 store asm("r0") = value;
                    asm volatile("" : "+r"(store));
                    store;
                });
                *mirror = *source;
            }
            D_03000050 = 0x1000;
            for (i = 0; i <= 2; i++) {
                if (D_02033F40[i] != 0) {
                    D_02033F40[i]->flags |= 0x100400;
                }
            }
            {
                register struct BattleWindowSpriteGroup **owner_slot asm("r1") =
                    &D_02033F3C;

                if (*owner_slot != 0) {
                    register u32 child_index asm("r2") = 0;
                    register struct BattleWindowSpriteGroup **owner_view asm("r4") =
                        owner_slot;
                    register u32 child_mask asm("r3") = 0x100400;

                    do {
                        struct BattleWindowSpriteGroup *owner = *owner_view;
                        struct BattleWindowSpriteGroup *child =
                            owner->children[child_index];

                        if (child != 0) {
                            child->flags |= child_mask;
                        }
                        {
                            register u32 next asm("r0") = child_index + 1;

                            next <<= 24;
                            child_index = next >> 24;
                        }
                    } while (child_index <= 15);
                }
            }
            *(volatile u16 *)0x04000208 = 1;
            gBattleScanlineWindowDisplayRequest = 0;
        } else if (status == BATTLE_SCANLINE_WINDOW_DISPLAY_STOP) {
            StopBattleScanlineWindow();
            return;
        }
        gBattleScanlineWindowBufferIndex ^= 1;
    }

    {
        register u32 current_r3 asm("r3");

        current_r3 = gBattleScanlineWindowDisplayRequest;
        if (current_r3 == 0) {
            register volatile u8 *table asm("r1") =
                (volatile u8 *)0x0203EE70;
            register u32 index asm("r0");

            asm volatile("" : "+r"(table));
            index = gBattleScanlineWindowBufferIndex << 7;
            index += (u32)table;
            if (*(volatile u8 *)index != 0) {
                D_0300004C &= 0x7DFF;
                D_0300004E = current_r3;
            } else {
                D_0300004C |= 0x8200;
                D_0300004E = 0x1643;
            }
        }
    }
}
