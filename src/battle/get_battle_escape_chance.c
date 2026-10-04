#include "m2c_prelude.h"
#include "battle_display.h"

extern u8 gBattleSetup[];
extern u8 gBattleState[];

s32 DivideSigned32(s32, s32) asm("func_080ECD98");

u8 GetBattleEscapeChance(void) asm("func_080BB660");

u8 GetBattleEscapeChance(void) {
    u8 unit_counts[2];
    s32 initiative_sums[2];
    u8 side;
    u8 unit;
    s16 escape_chance;

    if (gBattleSetup[0] != 2) {
        for (side = 0; side < 2; side++) {
            s32 *sump = &initiative_sums[side];
            u8 *cntp = &unit_counts[side];
            s32 z = 0;
            asm volatile("" : "+r"(z));
            *cntp = z;
            *sump = z;
            for (unit = 0; unit < 6; unit++) {
                u8 *battle_unit = &gBattleState[side * 0x1380 + unit * 0x270];
                if (battle_unit[0] != 0) {
                    initiative_sums[side] += BATTLE_UNIT_FIELD(battle_unit, s16, initiative);
                    unit_counts[side]++;
                }
            }
            {
                s32 *fp = &initiative_sums[side];
                u8 c = unit_counts[side];
                *fp = DivideSigned32(*fp, c);
            }
        }
        {
            s32 player_initiative = initiative_sums[0];
            s32 enemy_initiative = initiative_sums[1];
            escape_chance = 50;
            if (player_initiative > enemy_initiative) {
                escape_chance = 90;
            }
        }
        {
            s32 d;
            {
                register s32 c0 asm("r0");
                c0 = unit_counts[0];
                asm volatile("" : "+r"(c0));
                d = c0;
            }
            escape_chance += (d - unit_counts[1]) * 10;
        }
        escape_chance = (s16)escape_chance;
        {
            u8 *p = gBattleState;
            s32 round_bonus = BATTLE_ESCAPE_FIELD(p, u8, elapsed_rounds) * 10;
            s32 v = escape_chance + round_bonus;
            asm volatile("" :: "r"(p));
            escape_chance = v;
            if ((s16)v <= 0) {
                escape_chance = 1;
            }
        }
        if (escape_chance > 99) {
            escape_chance = 99;
        }
    } else {
        escape_chance = 100;
    }
    return escape_chance;
}
