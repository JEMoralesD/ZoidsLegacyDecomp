#include "m2c_prelude.h"
#include "battle_display.h"
extern void *gBattleUnitSprites[][6] asm("D_02032E8C");
extern void *gBattleUnitGaugeSprites[][6] asm("D_02032EBC");
extern s8 gBattleUnitSpriteMotionStates[][6] asm("D_02032EEC");
void HideBattleUnitSprites(u8 side, u8 unit) asm("func_080BB05C");

void HideBattleUnitSprites(u8 side, u8 unit) {
    void *unit_sprite = gBattleUnitSprites[side][unit];
    void *gauge_sprite;
    *(s32 *)((char*)unit_sprite + 0x24) = 0;
    *(s32 *)unit_sprite |= 0x20000;
    gauge_sprite = gBattleUnitGaugeSprites[side][unit];
    *(s32 *)((char*)gauge_sprite + 0x24) = 0;
    *(s32 *)gauge_sprite |= 0x20000;
    gBattleUnitSpriteMotionStates[side][unit] = 0;
}
