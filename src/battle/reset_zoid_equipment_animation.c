#include "m2c_prelude.h"
#include "battle_animation.h"
extern u8 gZoidEquipmentAnimationSlot asm("D_02034056"), gZoidEquipmentAnimationState asm("D_02034057"), gZoidEquipmentAnimationReady asm("D_02034058");
void ResetZoidEquipmentAnimation(void) asm("func_080D0480");

void ResetZoidEquipmentAnimation(void) {
    gZoidEquipmentAnimationSlot = 0xFF;
    gZoidEquipmentAnimationState = 0;
    gZoidEquipmentAnimationReady = 1;
}
