#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
void InitializeBattleLightningExplosionWhiteFlashEffect(struct BattleAnimationGroup *group) asm("func_080DF310");

void InitializeBattleLightningExplosionWhiteFlashEffect(struct BattleAnimationGroup *group) {
    int *effect_state_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(state));
    int *palette_restore_updates_slot = (int *)((u8 *)group + BATTLE_ANIMATION_OFFSET(effect.lightning_explosion_white_flash.palette_restore_updates));
    *palette_restore_updates_slot = 0;
    *effect_state_slot = 0;
}
