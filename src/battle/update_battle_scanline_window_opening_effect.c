#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
M2C_UNK StartBattleScanlineWindowOpening() asm("func_080D1C70");
u8 GetBattleScanlineWindowPhase() asm("func_080D1E38");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleScanlineWindowOpeningEffect(struct BattleAnimationGroup *group) asm("func_080DBE60");

void UpdateBattleScanlineWindowOpeningEffect(struct BattleAnimationGroup *group) {
    if (BATTLE_ANIMATION_FIELD(group, s32, state) == BATTLE_CONTACT_EFFECT_CREATE) {
        StartBattleScanlineWindowOpening();
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state) = (s32) (BATTLE_ANIMATION_FIELD(group, s32, state) + 1);
        return;
    }
    if (GetBattleScanlineWindowPhase() == BATTLE_SCANLINE_WINDOW_PHASE_ACTIVE) {
        DestroySpriteGroup(group);
    }
}
