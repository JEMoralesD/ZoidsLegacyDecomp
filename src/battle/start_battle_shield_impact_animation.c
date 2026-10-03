#include "m2c_prelude.h"
#include "battle_animation.h"
extern void StartBattleAnimation(s32, u8, u8, u16, s32, s32) asm("func_080D0B60");
extern void ResetBattleAnimationResourceAllocation(void) asm("func_080D2328");
extern struct BattleAnimationState gBattleAnimationState asm("D_02033FD0");
extern u8 gBattleSetup[];
void StartBattleShieldImpactAnimation(u8 side, u8 zoid_id, u16 item_id, u8 variant) asm("func_080D0F08");

void StartBattleShieldImpactAnimation(u8 side, u8 zoid_id, u16 item_id, u8 variant) {
    gBattleAnimationState.presentation_kind = BATTLE_ANIMATION_SHIELD_IMPACT;
    ResetBattleAnimationResourceAllocation();
    gBattleAnimationState.sprite_flags = 0x40;
    if (gBattleSetup[2] != 0xD) {
        StartBattleAnimation((item_id * 0x60) + 0x087D81D4, side, zoid_id, item_id, 0, variant);
    } else {
        StartBattleAnimation((item_id * 0x60) + 0x087D81E0, side, zoid_id, item_id, 0, variant);
    }
}
