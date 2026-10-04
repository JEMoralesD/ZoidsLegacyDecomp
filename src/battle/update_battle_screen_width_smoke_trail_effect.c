#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");


struct BattleScreenWidthSmokeTrailView {
    u8 data00[8];
    s16 y;
    u8 data0A[2];
    struct BattleDisplaySprite *children[9];
    u8 data30[0x5C];
    u32 state;
};

struct BattleDisplaySprite *CreateBattleAnimationSprite(struct BattleScreenWidthSmokeTrailView *, s32, s32, s32,
    s32, s32, s32, s32) asm("func_080D2450");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
void DestroySpriteGroup(struct BattleScreenWidthSmokeTrailView *) asm("func_8095114");

void UpdateBattleScreenWidthSmokeTrailEffect(struct BattleScreenWidthSmokeTrailView *owner) asm("func_080D54D4");

void UpdateBattleScreenWidthSmokeTrailEffect(struct BattleScreenWidthSmokeTrailView *owner)
{
    register struct BattleScreenWidthSmokeTrailView *saved_owner asm("r4") = owner;
    register u32 *state asm("r5") =
        (u32 *)((u8 *)saved_owner + 0x8C);

    if (*state <= 7) {
        if (*state == 0) {
            struct BattleDisplaySprite *child;

            child = CreateBattleAnimationSprite(saved_owner, 1, 0, 0, saved_owner->y,
                0x110, (s32)UpdateBattleHorizontalProjectileSprite, 1);
            saved_owner->children[0] = child;
            child->user_data.horizontal_projectile.speed = 0x20;
            PlayBattleAnimationSound(0);
        }

        {
            u32 current = *state;
            struct BattleDisplaySprite *child;
            register u32 next asm("r2");
            register u32 offset asm("r3");
            register struct BattleDisplaySprite **slot asm("r1");

            child = CreateBattleAnimationSprite(saved_owner, 0, 0,
                (s16)(current << 5), saved_owner->y, BATTLE_SPRITE_SEMITRANSPARENT, 0, 1);
            next = *state;
            next++;
            asm volatile("" : "+r"(next));
            offset = next << 2;
            asm volatile("" : "+r"(offset));
            slot = &saved_owner->children[0];
            asm volatile("" : "+r"(slot));
            slot = (struct BattleDisplaySprite **)((u8 *)slot + offset);
            *slot = child;
            *state = next;
        }
    } else {
        u8 live_sprite_index = 0;

        if (saved_owner->children[0] == 0) {
            register struct BattleDisplaySprite **slots asm("r2") =
                &saved_owner->children[0];

scan_next:
            live_sprite_index++;
            if (live_sprite_index > 8) {
                goto scan_done;
            }
            {
                register u32 slot_address asm("r0") = live_sprite_index << 2;
                asm volatile("add %0, %1, %0"
                             : "+r"(slot_address)
                             : "r"(slots));
                if (*(struct BattleDisplaySprite **)slot_address == 0) {
                    goto scan_next;
                }
            }
        }
scan_done:
        if (live_sprite_index == 9) {
            DestroySpriteGroup(saved_owner);
        }
    }
}
