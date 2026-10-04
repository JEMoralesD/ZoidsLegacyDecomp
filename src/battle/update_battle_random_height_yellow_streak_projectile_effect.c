#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

void UpdateBattleRandomHeightYellowStreakProjectileEffect(struct BattleAnimationGroup *group) asm("func_080E07D4");

void UpdateBattleRandomHeightYellowStreakProjectileEffect(struct BattleAnimationGroup *group) {
    void *projectile_sprite;
    if (BATTLE_ANIMATION_FIELD(group, s32, state) == BATTLE_YELLOW_STREAK_PROJECTILE_CREATE) {
        projectile_sprite = CreateBattleAnimationSprite(group, 0, 0, (s16)(BATTLE_ANIMATION_FIELD(group, s32, x) - 0x80), BATTLE_ANIMATION_FIELD(group, s16, effect.random_height.y), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_HOLD_LAST_FRAME), (s32)UpdateBattleHorizontalProjectileSprite, 1);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = projectile_sprite;
        BATTLE_SPRITE_FIELD(projectile_sprite, s32, user_data.horizontal_projectile.speed) = BATTLE_YELLOW_STREAK_PROJECTILE_SPEED_PIXELS;
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, s32, state)++;
    } else if (BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) == 0) {
        DestroySpriteGroup(group);
    }
}
