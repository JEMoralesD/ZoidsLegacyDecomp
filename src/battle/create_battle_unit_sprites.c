#include "m2c_prelude.h"
#include "battle_display.h"

extern void *CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
extern u8 ModuloUnsigned32(u8, s32) asm("func_080ECF78");

extern u8 *gBattleUnitGaugeSprites[2][6] asm("D_02032EBC");
extern u8 *gBattleUnitSprites[2][6] asm("D_02032E8C");
extern u8 gBattleUnitSpriteMotionStates[2][6] asm("D_02032EEC");

void CreateBattleUnitSprites(void) asm("func_080BAC54");

void CreateBattleUnitSprites(void) {
    u8 side;
    u8 unit;
    u8 zero;

    side = 0;
    zero = 0;
    do {
        u32 next_side;
        unit = 0;
        do {
            u8 index = side * 6 + unit;
            if (side == 0) {
                gBattleUnitGaugeSprites[0][unit] = CreateSprite(0x08106F50, 0x08106F5C, 0, 0, side, index << 3, 14, 0x20048, side);
                ({ u8 **pA = gBattleUnitSprites[0]; asm volatile("" : "+r"(pA)); pA; })[unit] =
                    CreateSprite(0x0821024C, 0x08210258, 0, 0, side, index << 6, index,
                        ({ s32 v = 3; v -= ModuloUnsigned32(unit, 3); v <<= 6; v |= 0x20308; v; }), side);
            } else {
                gBattleUnitGaugeSprites[side][unit] = CreateSprite(0x08106F50, 0x08106F5C, 0, 0, zero, index << 3, 14, 0x20048, zero);
                gBattleUnitSprites[side][unit] = CreateSprite(0x0821024C, 0x08210258, 0, 0, zero, index << 6, index,
                    ({ s32 w = ((ModuloUnsigned32(unit, 3) << 6) + 0x40) | 0x28308; w; }), zero);
            }
            *(u8 **)(gBattleUnitGaugeSprites[side][unit] + 40) = gBattleUnitSprites[side][unit];
            gBattleUnitSpriteMotionStates[side][unit] = zero;
            next_side = side + 1;
            unit++;
        } while (unit <= 5);
        side = next_side;
    } while (side <= 1);
}
