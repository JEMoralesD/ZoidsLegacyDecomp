#include "m2c_prelude.h"
#include "battle_display.h"

extern s32 IsBattleUnitActive(u8, u8) asm("func_080E9D88");
extern void BiosCpuFastSet(u8 *, u8 *, s32) asm("func_080ECD28");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern void DrawBattleGaugeFill(u8, u8 *) asm("func_080BAA40");

extern u8 *gBattleUnitGaugeSprites[2][6] asm("D_02032EBC");

void UpdateBattleUnitGaugeGraphics(void) asm("func_080BAB3C");

void UpdateBattleUnitGaugeGraphics(void) {
    u8 side;
    u8 unit;
    u8 hp_level;
    u8 ep_level;
    u8 *hp_gauge_pixels;

    side = 0;
    hp_gauge_pixels = (u8 *)0x020028A9;
    do {
        u32 next;
        unit = 0;
        next = side + 1;
        do {
            if ((IsBattleUnitActive(side, unit) << 24) != 0) {
                u8 *battle_unit;
                u8 *ep_unit;
                BiosCpuFastSet((u8 *)0x08277BE4, (u8 *)0x02002880, 0x40);
                battle_unit = (u8 *)(unit * 0x270 + side * 0x1380 + 0x02034B4C);
                if ((0x40 & BATTLE_UNIT_FIELD(battle_unit, u16, flags)) == 0) {
                    hp_level = DivideSigned32(BATTLE_UNIT_FIELD(battle_unit, s16, hp) * 20, BATTLE_UNIT_FIELD(battle_unit, s16, max_hp));
                } else {
                    hp_level = BATTLE_GAUGE_GRAY;
                }
                ep_unit = (u8 *)(unit * 0x270 + side * 0x1380 + 0x02034B4C);
                ep_level = DivideSigned32(BATTLE_UNIT_FIELD(ep_unit, s16, ep) * 20, BATTLE_UNIT_FIELD(ep_unit, s16, max_ep));
                DrawBattleGaugeFill(hp_level, hp_gauge_pixels);
                DrawBattleGaugeFill(ep_level, hp_gauge_pixels + 0x80);
                BiosCpuFastSet(hp_gauge_pixels - 41, (u8 *)((*(u16 *)(gBattleUnitGaugeSprites[side][unit] + 14) << 5) + 0x06016000), 0x40);
            }
            asm volatile("" :: "r"(unit));
            unit++;
        } while (unit <= 5);
        side = next;
    } while (side <= 1);
}
