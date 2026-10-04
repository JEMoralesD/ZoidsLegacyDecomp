#include "field_actor.h"
void UpdateBlinkingFieldSprite(int sprite_address) asm("func_080AA59C");

void UpdateBlinkingFieldSprite(int sprite_address) {
    if (M2C_FIELD(sprite_address, u32 *, (s32)&((struct FieldSpriteBlinkStateView *)0)->blink_tick) & 1) {
        M2C_FIELD(sprite_address, u32 *, (s32)&((struct FieldSpriteBlinkStateView *)0)->flags) ^= FIELD_SPRITE_HIDDEN;
    }
    M2C_FIELD(sprite_address, u32 *, (s32)&((struct FieldSpriteBlinkStateView *)0)->blink_tick) += 1;
}
