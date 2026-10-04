#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern s32 gBattleZoidScrollX asm("D_02034034");
void CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
void DestroySpriteGroup(void *) asm("func_08095114");
extern struct SpriteBackgroundScrollOffsets gSpriteBackgroundScroll[] asm("D_03000054");

void SpawnBattleScrollingLoopYellowStreakEffect(struct BattleAnimationGroup *group) asm("func_080DE54C");

void SpawnBattleScrollingLoopYellowStreakEffect(struct BattleAnimationGroup *group) {
    CreateBattleAnimationSprite(group, 0, 0,
                  (s16)(BATTLE_ANIMATION_FIELD(group, s32, x) + gBattleZoidScrollX / 0x100),
                  (s32)(s16)(BATTLE_ANIMATION_FIELD(group, s32, y) + gSpriteBackgroundScroll[0].y_fixed8 / 0x100),
                  (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_LOOP_ANIMATION), 0, 0);
    SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_ACCELERATE_SCROLL, 0);
    *(s16 *)0x03000050 = 0x1010;
    PlayBattleAnimationSound(0);
    DestroySpriteGroup(group);
}
