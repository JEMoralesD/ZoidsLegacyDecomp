#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedYellowOrbTrailSprite(void *) asm("func_080D98C0");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound() asm("func_080D2790");
void UpdateBattleRandomHeightYellowOrbEffect(struct BattleAnimationGroup *group) asm("func_080D9EB8");

void UpdateBattleRandomHeightYellowOrbEffect(struct BattleAnimationGroup *group) {
    s32 *effect_state = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(state));
    s32 phase = *effect_state;
    void *orb, *trail_orb;
    u32 elapsed_trail_frames, trail_slot_end;
    u8 live_trail_index;
    s32 sprite_offset;
    if (phase == BATTLE_PROJECTILE_TRAIL_CREATE) {
        orb = CreateBattleAnimationSprite(
                group, 0, 0, BATTLE_ANIMATION_FIELD(group, s16, x),
                (s32)(s16)(BATTLE_ANIMATION_FIELD(group, s32, y) + BATTLE_ANIMATION_FIELD(group, s32, effect.random_height_offset.y_offset)),
                BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedYellowOrbTrailSprite, 1);
        BATTLE_ANIMATION_FIELD(group, void *, sprites[0]) = orb;
        BATTLE_SPRITE_FIELD(orb, void *, user_data.projectile_trail.group) = group;
        BATTLE_SPRITE_FIELD(orb, s32, user_data.projectile_trail.elapsed_frames) = phase;
        BATTLE_SPRITE_FIELD(orb, s32, user_data.projectile_trail.impact_x) = phase;
        PlayBattleAnimationSound(0);
        *effect_state = *effect_state + 1;
        return;
    }
    {
        register void *orb_view asm("r0") = BATTLE_ANIMATION_FIELD(group, void *, sprites[0]);
        s32 orb_hidden = *(s32 *)orb_view & BATTLE_SPRITE_HIDDEN;
        trail_orb = orb_view;
        if (orb_hidden) {
            live_trail_index = 0x10;
            __asm__ volatile("" : "+r"(live_trail_index));
            elapsed_trail_frames = BATTLE_SPRITE_FIELD(trail_orb, u32, user_data.projectile_trail.elapsed_frames);
            trail_slot_end = (elapsed_trail_frames >> 1) + 0x11;
            if (((u32)live_trail_index < trail_slot_end) && (BATTLE_ANIMATION_FIELD(group, s32, sprites[16]) == 0)) {
                u32 trail_scan_end = trail_slot_end;
                s32 *sprite_slots = (s32 *)((char *)group + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    live_trail_index += 1;
                } while (((u32)live_trail_index < trail_scan_end) &&
                                  ((sprite_offset = live_trail_index << 2, *(s32 *)((char *)sprite_slots + sprite_offset)) == 0));
            }
            if (live_trail_index == ((*(volatile u32 *)((char *)trail_orb + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames)) >> 1) + 0x11))
                DestroySpriteGroup(group);
        }
    }
}
