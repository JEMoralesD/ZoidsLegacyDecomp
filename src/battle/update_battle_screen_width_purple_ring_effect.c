#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
s32 CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, s32, s32) asm("func_080D2450");
M2C_UNK PlayBattleAnimationSound(s32) asm("func_080D2790");

struct BattleScreenWidthPurpleRingView {
    u32 flags;
    s32 x;
    s16 y;
    u16 y_high;
    void *sprites[BATTLE_ANIMATION_GROUP_SPRITE_COUNT];
    u32 ring_count;
};

void UpdateBattleScreenWidthPurpleRingEffect(struct BattleScreenWidthPurpleRingView *group) asm("func_080DA368");

void UpdateBattleScreenWidthPurpleRingEffect(struct BattleScreenWidthPurpleRingView *group) {
    u32 ring_index;
    u8 live_sprite_index;

    ring_index = group->ring_count;
    if (ring_index < BATTLE_PURPLE_RING_COUNT) {
        group->sprites[group->ring_count] = CreateBattleAnimationSprite(
            group, 0, 0,
            (s16)(ring_index * BATTLE_PURPLE_RING_SPACING_PIXELS),
            group->y, BATTLE_SPRITE_SEMITRANSPARENT, 0,
            BATTLE_ANIMATION_SPRITE_REVERSE_FACING
        );
        if (group->ring_count == 0) {
            PlayBattleAnimationSound(0);
        }
        group->ring_count += 1;
        return;
    }
    live_sprite_index = 0;
    if (group->sprites[0] == 0) {
        do {
            live_sprite_index += 1;
            if ((u32)live_sprite_index >= BATTLE_PURPLE_RING_COUNT) {
                break;
            }
        } while (group->sprites[live_sprite_index] == 0);
    }
    if (live_sprite_index == BATTLE_PURPLE_RING_COUNT) {
        DestroySpriteGroup(group);
    }
}
