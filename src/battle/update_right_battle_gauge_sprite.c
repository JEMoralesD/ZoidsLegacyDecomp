#include "m2c_prelude.h"
#include "battle_display.h"
struct LinkedUnitSpriteView {
    u32 flags;
    u16 x;
    u16 y;
};

struct BattleGaugeSpriteView {
    u32 flags;
    u16 x;
    u16 y;
    u8 pad[0x20];
    struct LinkedUnitSpriteView *unit_sprite;
    u32 override_flags;
};

void UpdateRightBattleGaugeSprite(struct BattleGaugeSpriteView *gauge_sprite) asm("func_080BAEEC");

void UpdateRightBattleGaugeSprite(struct BattleGaugeSpriteView *gauge_sprite) {
    u32 v;
    u32 t;
    u32 mask;
    u32 fl;
    struct LinkedUnitSpriteView *unit_sprite;

    v = gauge_sprite->flags;
    v &= 0xFFFDFFFF;
    gauge_sprite->flags = v;
    t = gauge_sprite->override_flags;
    if (t == 0) {
        unit_sprite = gauge_sprite->unit_sprite;
        fl = unit_sprite->flags;
        mask = 0x20000;
        if ((fl & mask) == 0) {
            gauge_sprite->x = unit_sprite->x + 16;
            gauge_sprite->y = unit_sprite->y;
        } else {
            v |= mask;
            gauge_sprite->flags = v;
        }
    } else {
        v |= t;
        gauge_sprite->flags = v;
    }
}
