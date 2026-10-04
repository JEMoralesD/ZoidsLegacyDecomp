#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void PlayBattleAnimationSound() asm("func_080D2790");

void InitializeBattleRisingPurpleSlashWhiteFlashEffect(int group_address) asm("func_080DC560");

void InitializeBattleRisingPurpleSlashWhiteFlashEffect(int group_address) {
    *(u32 *)(group_address + BATTLE_ANIMATION_OFFSET(state)) = 0;
    *(u16 *)0x03000050 = 0x1010;
    PlayBattleAnimationSound(0);
}
