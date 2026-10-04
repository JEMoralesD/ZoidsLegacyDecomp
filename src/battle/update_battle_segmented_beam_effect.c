#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern s32 CreateBattleAnimationSprite(struct BattleAnimationGroup *, s32, s32, s16, s32, s32, u32, u32) asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern void DestroySpriteGroup(struct BattleAnimationGroup *) asm("func_08095114");

void UpdateBattleSegmentedBeamEffect(struct BattleAnimationGroup *group) asm("func_080D307C");

void UpdateBattleSegmentedBeamEffect(struct BattleAnimationGroup *group) {
    u32 segment_index;
    u32 stored_segment_index;
    u8 live_sprite_index;
    s32 segment_sprite;

    segment_index = BATTLE_ANIMATION_FIELD(group, u32, state);
    if (segment_index <= 7) {
        segment_sprite = CreateBattleAnimationSprite(group, 0, 0, (s16)(group->x - (segment_index << 5) - 8), BATTLE_ANIMATION_FIELD(group, s16, y), 0x500, 0, 0);
        stored_segment_index = BATTLE_ANIMATION_FIELD(group, u32, state);
        group->sprites[stored_segment_index] = segment_sprite;
        if (stored_segment_index == 0) {
            group->sprites[8] = CreateBattleAnimationSprite(group, 0, 1, (s16)group->x, BATTLE_ANIMATION_FIELD(group, s16, y), 0x500, stored_segment_index, stored_segment_index);
            group->sprites[9] = CreateBattleAnimationSprite(group, 1, 0, (s16)group->x, BATTLE_ANIMATION_FIELD(group, s16, y), 0x400, stored_segment_index, stored_segment_index);
            PlayBattleAnimationSound(0);
        }
        BATTLE_ANIMATION_FIELD(group, u32, state)++;
        return;
    }
    live_sprite_index = 0;
    if (group->sprites[0] == 0) {
        do {
            live_sprite_index++;
            if (live_sprite_index > 9) {
                break;
            }
        } while (group->sprites[live_sprite_index] == 0);
    }
    if (live_sprite_index == 10) {
        DestroySpriteGroup(group);
    }
}

void InitializeBattleBeamImpactTrailEffect(struct BattleAnimationGroup *group) asm("func_080D3140");

void InitializeBattleBeamImpactTrailEffect(struct BattleAnimationGroup *group) {
    BATTLE_ANIMATION_FIELD(group, u32, state) = group->effect.beam_trail.started = group->effect.beam_trail.sprite_count = 0;
}
