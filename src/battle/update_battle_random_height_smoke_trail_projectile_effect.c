#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedProjectileTrailSprite(void *) asm("func_080D3F6C");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");
void UpdateBattleRandomHeightSmokeTrailProjectileEffect(void *group) asm("func_080D4BD4");

void UpdateBattleRandomHeightSmokeTrailProjectileEffect(void *group) {
  s32 *state_slot = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
  s32 state = *state_slot;
  void *projectile;
  void *trail_projectile;
  u32 elapsed_frames;
  u32 trail_slot_end;
  u8 trail_slot_index;
  s32 sprite_offset;
  if (state == 0) {
    projectile = CreateBattleAnimationSprite(
        group, 0, 0, (s16)(*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(x)) - 0x10),
        (s32) * (s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.random_height.y)), 0x20, (s32)UpdateBattleTargetedProjectileTrailSprite, 1);
    *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0])) = projectile;
    *(void **)((char *)projectile + (s32)&((struct BattleDisplaySprite *)0)->user_data.projectile_trail.group) = group;
    *(s32 *)((char *)projectile + 0x2C) = state;
    *(s32 *)((char *)projectile + (s32)&((struct BattleDisplaySprite *)0)->user_data.projectile_trail.impact_x) = state;
    PlayBattleAnimationSound(0);
    *state_slot = *state_slot + 1;
    return;
  }
  {
    register void *projectile asm("r0") = *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
    s32 projectile_hidden = *(s32 *)projectile & 0x20000;
    trail_projectile = projectile;
    if (projectile_hidden) {
      trail_slot_index = 0x18;
      __asm__ volatile("" : "+r"(trail_slot_index));
      elapsed_frames = *(u32 *)((char *)trail_projectile + 0x2C);
      trail_slot_end = (elapsed_frames >> 2) + 0x19;
      if (((u32)trail_slot_index < trail_slot_end) && (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0)) {
        u32 trail_scan_end = trail_slot_end;
        s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
        do {
          trail_slot_index += 1;
        } while (((u32)trail_slot_index < trail_scan_end) &&
                 ((sprite_offset = trail_slot_index << 2, *(s32 *)((char *)sprite_slots + sprite_offset)) == 0));
      }
      if (trail_slot_index == ((*(volatile u32 *)((char *)trail_projectile + 0x2C) >> 2) + 0x19))
        DestroySpriteGroup(group);
    }
  }
}
