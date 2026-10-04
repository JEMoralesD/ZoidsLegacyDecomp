#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleOffscreenProjectileTrailSprite(void *) asm("func_080D3E9C");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound() asm("func_080D2790");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleSmokeTrailProjectileEffect(void *group) asm("func_080D4068");

void UpdateBattleSmokeTrailProjectileEffect(void *group) {
    register char *owner asm("r5") = group;
    register s32 *effect_state asm("r6") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register s32 state asm("r4") = *effect_state;

    if (state == 0) {
        void *created;
        s32 *state_slot;
        u8 index;

        created = CreateBattleAnimationSprite(owner, 0, 0, *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)),
            (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)), 0x20, (s32)UpdateBattleOffscreenProjectileTrailSprite, state);
        *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        *(void **)((char *)created + (s32)&((struct BattleDisplaySprite *)0)->user_data.projectile_trail.group) = owner;
        *(s32 *)((char *)created + 0x2C) = state;

        index = 0;
        state_slot = effect_state;
        effect_state -= 0x20;
        do {
            u32 random;
            s32 speed_fixed8;
            void *spawned;
            s32 next;
            s32 child_off;

            random = CallFunctionR0(gRandomNumberCallback);
            speed_fixed8 = ((random * 0x41) >> 0xF) + 0xC0;
            spawned = CreateBattleAngledProjectileSprite(owner, 2, 0,
                *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
                BATTLE_SPRITE_SEMITRANSPARENT, (index << 2) - 0xA, speed_fixed8, 0);
            next = index + 1;
            child_off = next << 2;
            *(s32 *)((char *)effect_state + child_off) = (s32)spawned;
            index = (u8)next;
        } while (index <= 5);
        SetBattleAnimationCameraMode(5, 0);
        PlayBattleAnimationSound(0);
        *state_slot = *state_slot + 1;
        return;
    }
    {
        register void *projectile asm("r0") = *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
        s32 projectile_hidden = *(s32 *)projectile & 0x20000;
        register void *child asm("r3") = projectile;

        if (projectile_hidden) {
            register s32 trail_slot_index asm("r4");
            u32 elapsed_frames;
            u32 bound;
            s32 off;

            trail_slot_index = 0x18;
            __asm__ volatile ("" : "+r" (trail_slot_index));
            elapsed_frames = *(u32 *)((char *)child + 0x2C);
            bound = (elapsed_frames >> 2) + 0x19;
            if ((u32)trail_slot_index < bound && *(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[24])) == 0) {
                u32 scan_bound = bound;
                s32 *children = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    register s32 next_trail_slot asm("r0");
                    next_trail_slot = trail_slot_index + 1;
                    next_trail_slot <<= 24;
                    trail_slot_index = (u32)next_trail_slot >> 24;
                } while ((u32)trail_slot_index < scan_bound &&
                    (off = trail_slot_index << 2, *(s32 *)((char *)children + off)) == 0);
            }
            if (trail_slot_index == ((*(volatile u32 *)((char *)child + 0x2C) >> 2) + 0x19)) {
                trail_slot_index = 1;
                if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                    s32 *children = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
                    do {
                        register s32 next_trail_slot asm("r0");
                        next_trail_slot = trail_slot_index + 1;
                        next_trail_slot <<= 24;
                        trail_slot_index = (u32)next_trail_slot >> 24;
                    } while ((u32)trail_slot_index <= 6 &&
                        (off = trail_slot_index << 2, *(s32 *)((char *)children + off)) == 0);
                }
                if (trail_slot_index == 7) {
                    DestroySpriteGroup(owner);
                }
            }
        }
    }
}
