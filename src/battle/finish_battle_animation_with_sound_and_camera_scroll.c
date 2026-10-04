#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySpriteGroup() asm("func_08095114");
extern int CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");
extern void SetBattleAnimationCameraMode() asm("func_080D12A0");

void FinishBattleAnimationWithSoundAndCameraScroll(s32 group_address) asm("func_080D5690");

void FinishBattleAnimationWithSoundAndCameraScroll(s32 group_address) {
    SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_ACCELERATE_SCROLL, 0);
    PlayBattleAnimationSound(0);
    DestroySpriteGroup(group_address);
}

void InitializeBattleFacingSpriteEffect(s32 group_address) asm("func_080D56B0");

void InitializeBattleFacingSpriteEffect(s32 group_address) {
    *(s32 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(state)) = 0;
}

void UpdateBattleFacingSlashEffect(void *group_address) asm("func_080D56B8");

void UpdateBattleFacingSlashEffect(void *group_address) {
    s32 state;
    state = *(s32 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(state));
    if (state == 0) {
        *(s32 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_address, 0, 0,
            *(s16 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(x)), *(s16 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(y)),
            state, state, 1);
        PlayBattleAnimationSound(0);
        *(s32 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(state)) = *(s32 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(state)) + 1;
        return;
    }
    if (*(s32 *)((char *)group_address + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
        DestroySpriteGroup(group_address);
    }
}
