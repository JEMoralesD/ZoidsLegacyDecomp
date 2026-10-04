#include "m2c_prelude.h"
void SetSpriteAnimation(void *sprite, s16 animation_id) {
    s32 zero = 0;
    *(s16 *)((u8 *)sprite + 0x12) = animation_id;
    *(s16 *)((u8 *)sprite + 0x14) = zero;
    *(s16 *)((u8 *)sprite + 0x16) = zero;
    *(s32 *)sprite = *(s32 *)sprite & -5;
}
