#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
s32 CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");


struct BattleBeamStripEffectView {
    u8 data00[8];
    s16 y;
    u16 data0A;
    void *sprites[BATTLE_ANIMATION_GROUP_SPRITE_COUNT];
    u32 segment_count;
};

void UpdateBattleBeamStripEffect(struct BattleBeamStripEffectView *group) asm("func_080D32D4");

void UpdateBattleBeamStripEffect(struct BattleBeamStripEffectView *group) {
    u32 index;
    u8 live_sprite_index;

    index = group->segment_count;
    if (index <= 7U) {
        group->sprites[group->segment_count] = CreateBattleAnimationSprite(group, 0, 0, (s16)(index << 5), group->y, 0x400, 0, 1);
        if (group->segment_count == 0) {
            PlayBattleAnimationSound(0);
        }
        group->segment_count += 1;
        return;
    }
    live_sprite_index = 0;
    if (group->sprites[0] == 0) {
        do {
            live_sprite_index += 1;
            if ((u32)live_sprite_index > 7U) {
                break;
            }
        } while (group->sprites[live_sprite_index] == 0);
    }
    if (live_sprite_index == 8) {
        DestroySpriteGroup(group);
    }
}
