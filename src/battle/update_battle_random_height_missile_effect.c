#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedMissileTrailSprite(void *) asm("func_080D63D0");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");
void UpdateBattleRandomHeightMissileEffect(void *group) asm("func_080D6B2C");

void UpdateBattleRandomHeightMissileEffect(void *group) {
  s32 *effect_state = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
  s32 phase = *effect_state;
  void *missile, *trail_missile;
  u32 elapsed_frames, trail_slot_end;
  u8 live_trail_index;
  s32 sprite_offset;
  if (phase == 0) {
    missile = CreateBattleAnimationSprite(
        group, 0, 0, *(s16 *)((char *)group + BATTLE_ANIMATION_OFFSET(x)),
        (s32)(s16)(*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(y)) + *(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(effect.random_height_missile.y_offset))),
        0x20, (s32)UpdateBattleTargetedMissileTrailSprite, 1);
    *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0])) = missile;
    BATTLE_SPRITE_FIELD(missile, void *, user_data.missile_trail.group) = group;
    BATTLE_SPRITE_FIELD(missile, s32, user_data.missile_trail.elapsed_frames) = phase;
    BATTLE_SPRITE_FIELD(missile, s32, user_data.missile_trail.impact_x) = phase;
    BATTLE_SPRITE_FIELD(missile, s32, user_data.missile_trail.trail_resource_slot_base) = phase;
    PlayBattleAnimationSound(0);
    *effect_state = *effect_state + 1;
    return;
  }
  {
    register void *missile_view asm("r0") = *(void **)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
    s32 missile_hidden = *(s32 *)missile_view & BATTLE_SPRITE_HIDDEN;
    trail_missile = missile_view;
    if (missile_hidden) {
      live_trail_index = 0x18;
      __asm__ volatile("" : "+r"(live_trail_index));
      elapsed_frames = BATTLE_SPRITE_FIELD(trail_missile, u32, user_data.missile_trail.elapsed_frames);
      trail_slot_end = (elapsed_frames >> 1) + 0x19;
      if (((u32)live_trail_index < trail_slot_end) && (*(s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0)) {
        u32 trail_scan_end = trail_slot_end;
        s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
        do {
          live_trail_index += 1;
        } while (((u32)live_trail_index < trail_scan_end) &&
                 ((sprite_offset = live_trail_index << 2, *(s32 *)((char *)sprite_slots + sprite_offset)) == 0));
      }
      if (live_trail_index == ((*(volatile u32 *)((char *)trail_missile + (s32)&((struct BattleDisplaySprite *)0)->user_data.missile_trail.elapsed_frames) >> 1) + 0x19))
        DestroySpriteGroup(group);
    }
  }
}
