#include "m2c_prelude.h"
#include "battle_display.h"
struct BattleGaugeSpriteView {
    u32 flags;
    u16 x;
    u16 y;
    u8 pad[0x28 - 8];
    struct BattleGaugeSpriteView *unit_sprite;
    u32 override_flags;
};

void UpdateLeftBattleGaugeSprite(struct BattleGaugeSpriteView *gauge_sprite) asm("func_080BAEAC");

void UpdateLeftBattleGaugeSprite(struct BattleGaugeSpriteView *gauge_sprite) {
    u32 v = gauge_sprite->flags & 0xFFFDFFFF;
    gauge_sprite->flags = v;
    if (gauge_sprite->override_flags == 0) {
        struct BattleGaugeSpriteView *unit_sprite = gauge_sprite->unit_sprite;
        u32 unit_flags = unit_sprite->flags;
        u32 hidden_mask = 0x20000;
        if ((unit_flags & hidden_mask) == 0) {
            gauge_sprite->x = unit_sprite->x - 0x30;
            gauge_sprite->y = unit_sprite->y;
        } else {
            gauge_sprite->flags = v | hidden_mask;
        }
    } else {
        gauge_sprite->flags = v | gauge_sprite->override_flags;
    }
}
