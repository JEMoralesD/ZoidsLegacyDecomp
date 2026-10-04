#include "m2c_prelude.h"
void DestroySprite(s32 *sprite) {
    *sprite = (*sprite & ~1) | 2;
}
