#include "m2c_prelude.h"
#include "battle_animation.h"
u8 IsZoidEquipmentAnimationReady(void) asm("func_080D0AE4");

u8 IsZoidEquipmentAnimationReady(void) {
    return *(u8 *)0x02034058;
}
