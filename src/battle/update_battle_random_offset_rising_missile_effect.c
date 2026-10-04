#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedRisingMissileTrailSprite(void *) asm("func_080D6D84");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");
void UpdateBattleRandomOffsetRisingMissileEffect(void *group) asm("func_080D7B34");

void UpdateBattleRandomOffsetRisingMissileEffect(void *group) {
  s32 *effect_state = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
  s32 phase = *effect_state;
  void *projectile, *trail_projectile;
  u32 elapsed_trail_frames, trail_slot_end;
  u8 live_trail_index;
  s32 sprite_offset;
  if (phase == BATTLE_PROJECTILE_TRAIL_CREATE) {
    projectile = CreateBattleAnimationSprite(
        group, 0, 0,
        (s16)(BATTLE_ANIMATION_FIELD(group, s32, x) + (BATTLE_ANIMATION_FIELD(group, s32, effect.random_diagonal_launch.launch_x_offset_half) * 2)),
        (s32) * (s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedRisingMissileTrailSprite, 1);
    BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = projectile;
    BATTLE_SPRITE_FIELD(projectile, void *, user_data.missile_trail.group) = group;
    BATTLE_SPRITE_FIELD(projectile, s32, user_data.missile_trail.elapsed_frames) = phase;
    BATTLE_SPRITE_FIELD(projectile, s32, user_data.missile_trail.impact_x) = phase;
    PlayBattleAnimationSound(0);
    *effect_state = *effect_state + 1;
    return;
  }
  {
    register void *projectile_view asm("r0") = BATTLE_ANIMATION_FIELD(group, void *, sprites[0]);
    s32 projectile_hidden = *(s32 *)projectile_view & BATTLE_SPRITE_HIDDEN;
    trail_projectile = projectile_view;
    if (projectile_hidden) {
      live_trail_index = 0x18;
      __asm__ volatile("" : "+r"(live_trail_index));
      elapsed_trail_frames = BATTLE_SPRITE_FIELD(trail_projectile, u32, user_data.missile_trail.elapsed_frames);
      trail_slot_end = (elapsed_trail_frames >> 1) + 0x19;
      if (((u32)live_trail_index < trail_slot_end) && (BATTLE_ANIMATION_FIELD(group, s32, sprites[24]) == 0)) {
        u32 trail_scan_end = trail_slot_end;
        s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
        do {
          live_trail_index += 1;
        } while (((u32)live_trail_index < trail_scan_end) &&
                 ((sprite_offset = live_trail_index << 2, *(s32 *)((char *)sprite_slots + sprite_offset)) == 0));
      }
      if (live_trail_index == ((*(volatile u32 *)((char *)trail_projectile + 0x2C) >> 1) + 0x19))
        DestroySpriteGroup(group);
    }
  }
}
