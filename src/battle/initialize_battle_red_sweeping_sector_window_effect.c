#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
#include "../graphics/screen_effects.h"

void ConfigurePolygonScanlineWindows(s32, s32, s32, s32) asm("func_080955A0");
void PlayBattleAnimationSound(s32) asm("func_080D2790");

void InitializeBattleRedSweepingSectorWindowEffect(struct BattleAnimationGroup *group) asm("func_080DBA90");

void InitializeBattleRedSweepingSectorWindowEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state = &group->state;
    s32 *shape_progress_slot = (s32 *)&group->effect.sector_window.shape_progress;
    *shape_progress_slot = 0;
    *effect_state = 0;
    *(s16 *)0x03000050 = BATTLE_SECTOR_WINDOW_BLEND_ALPHA;
    ConfigurePolygonScanlineWindows(0x02034910, BATTLE_SECTOR_WINDOW_INSIDE_LAYERS, BATTLE_SECTOR_WINDOW_OUTSIDE_LAYERS, SCANLINE_WINDOW_HBLANK_CALLBACK);
    *(s16 *)0x0300004E = BATTLE_SECTOR_WINDOW_BLEND_CONTROL;
    *(s16 *)0x05000000 = BATTLE_SECTOR_WINDOW_BACKDROP_COLOR;
    PlayBattleAnimationSound(0);
}
