#include "m2c_prelude.h"
#include "battle_display.h"

struct BattleUnitSpriteView {
    s32 flags;
    u8 pad4[10];
    u16 tile_offset;
    u8 palette_bank;
    u8 pad11[19];
    s32 update_callback;
    s32 world_x;
    s32 world_y;
    s32 world_z;
};

extern void LoadZoidIconGraphics(u8, u8, u16, u8) asm("func_0809A4CC");

extern struct BattleUnitSpriteView *gBattleUnitSprites[2][6] asm("D_02032E8C");
extern struct BattleUnitSpriteView *gBattleUnitGaugeSprites[2][6] asm("D_02032EBC");
extern u8 gBattleUnitSpriteMotionStates[2][6] asm("D_02032EEC");
extern s32 gBattleUnitWorldPositions[][3] asm("D_087A2790");

void ConfigureBattleUnitSprites(u8 zoid_id, u8 palette_variant, u8 size_class, u8 side, u8 unit, s32 x_offset, s32 z_offset) asm("func_080BAF2C");

void ConfigureBattleUnitSprites(u8 zoid_id, u8 palette_variant, u8 size_class, u8 side, u8 unit, s32 x_offset, s32 z_offset) {
    u8 position_index = side * 6 + unit;
    struct BattleUnitSpriteView **slot;
    u8 *baseS;
    struct BattleUnitSpriteView *p2;
    struct BattleUnitSpriteView *q2;
    struct BattleUnitSpriteView *p3;

    baseS = (u8 *)0x02032E8C;
    asm volatile("" : "+r"(baseS));
    slot = (struct BattleUnitSpriteView **)(unit * 4 + side * 24 + (s32)baseS);
    LoadZoidIconGraphics(zoid_id, palette_variant, (*slot)->tile_offset, (*slot)->palette_bank);
    if (size_class == ZOID_SIZE_CLASS_DOUBLE_ICON) {
        struct BattleUnitSpriteView *t = *slot;
        t->update_callback = 0x080BAE19;
        t->flags |= 0x80000;
    } else {
        struct BattleUnitSpriteView *t = *slot;
        t->update_callback = 0x080BADD5;
        t->flags &= 0xFFF7FFFF;
    }
    {
        register u32 scratch_r3 asm("r3");
        asm volatile("" : "=&r"(scratch_r3) : "r"(&gBattleUnitSprites[0][0]));
    }
    p2 = gBattleUnitSprites[side][unit];
    p2->flags &= 0xFFFDFFFF;
    if (side == 0) {
        gBattleUnitGaugeSprites[0][unit]->update_callback = 0x080BAEED;
    } else {
        gBattleUnitGaugeSprites[side][unit]->update_callback = 0x080BAEAD;
    }
    q2 = gBattleUnitGaugeSprites[side][unit];
    q2->flags &= 0xFFFDFFFF;
    q2->world_y = 0x20000;
    p3 = gBattleUnitSprites[side][unit];
    p3->world_x = gBattleUnitWorldPositions[position_index][0] + x_offset;
    p3->world_y = gBattleUnitWorldPositions[position_index][1];
    p3->world_z = gBattleUnitWorldPositions[position_index][2] + z_offset;
    gBattleUnitSpriteMotionStates[side][unit] = 0;
}
