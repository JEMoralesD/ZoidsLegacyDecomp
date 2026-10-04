#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
s32 CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

struct BattleScreenWidthBeamView {
    u32 flags;
    s32 x;
    s16 y;
    u16 data0A;
    void *sprites[32];
    u32 segment_count;
};

void UpdateBattleScreenWidthBeamEffect(struct BattleScreenWidthBeamView *group) asm("func_080D61CC");

void UpdateBattleScreenWidthBeamEffect(struct BattleScreenWidthBeamView *group) {
    u32 segment_index;
    u8 live_sprite_index;

    segment_index = group->segment_count;
    if (segment_index <= 7U) {
        group->sprites[group->segment_count] = CreateBattleAnimationSprite(
            group,
            0,
            1,
            (s16)(segment_index << 5),
            group->y,
            BATTLE_SPRITE_SEMITRANSPARENT,
            0,
            1
        );
        PlayBattleAnimationSound(0);
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
