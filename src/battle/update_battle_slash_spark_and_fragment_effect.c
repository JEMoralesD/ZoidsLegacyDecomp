#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleSlashSparkAndFragmentEffect(void *group) asm("func_080D586C");

void UpdateBattleSlashSparkAndFragmentEffect(void *group) {
    register char *owner asm("r6") = group;
    register s32 *effect_state asm("r4") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r0") = *effect_state;
    s32 index;

    switch (state) {
    case 0: {
        void *created = CreateBattleAnimationSprite(owner, 0, 0,
            *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
            0, 0, 1);
        *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        *effect_state = *effect_state + 1;
        break;
    }
    case 1: {
        if (*(u16 *)(*(char **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) + 0x14) == 5) {
            register u32 *rng asm("r5");
            register u32 *second_rng asm("r9");
            register s32 *children asm("r8");
            register s32 *state_slot asm("r10");
            register s32 *child_base asm("r2");

            index = 0;
            state_slot = effect_state;
            child_base = (s32 *)0xC;
            child_base = (s32 *)((s32)child_base + (s32)owner);
            children = child_base;
            rng = &gRandomNumberCallback;
            do {
                register s32 random asm("r0");
                register s32 spark_angle asm("r4");
                register s32 next asm("r2");
                register s32 child_off asm("r1");
                register s32 speed_fixed8 asm("r1");
                register s32 minimum_speed_fixed8 asm("r0");
                void *spawned;

                random = CallFunctionR0(*rng);
                spark_angle = random << 1;
                spark_angle += random;
                spark_angle <<= 4;
                spark_angle += random;
                spark_angle = (u32)spark_angle >> 15;
                spark_angle += 128;
                speed_fixed8 = (CallFunctionR0(*rng) * 0x201) >> 15;
                minimum_speed_fixed8 = 0x200;
                speed_fixed8 += minimum_speed_fixed8;
                spawned = CreateBattleAngledProjectileSprite(owner, 1, 0,
                    *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
                    0x500, spark_angle, speed_fixed8, 0);
                next = index + 1;
                child_off = next << 2;
                *(s32 *)((char *)children + child_off) = (s32)spawned;
                next <<= 24;
                index = (u32)next >> 24;
            } while ((u32)index <= 7);

            index = 0;
            second_rng = &gRandomNumberCallback;
            do {
                register s32 random asm("r4");
                register s32 fragment_angle asm("r5");
                register s32 angle_jitter asm("r0");
                register s32 minimum_speed_fixed8 asm("r0");
                register s32 child_off asm("r1");
                s32 speed_fixed8;
                void *spawned;

                {
                    register u32 *rng_view asm("r1") = second_rng;
                    random = CallFunctionR0(*rng_view);
                }
                fragment_angle = DivideSigned32(index << 6, 7);
                angle_jitter = (u32)(random * 9) >> 15;
                angle_jitter -= 36;
                fragment_angle += angle_jitter;
                {
                    register u32 *rng_view asm("r2") = second_rng;
                    speed_fixed8 = (CallFunctionR0(*rng_view) * 0x201) >> 15;
                }
                minimum_speed_fixed8 = 0x200;
                __asm__ volatile ("" : "+r" (minimum_speed_fixed8));
                speed_fixed8 += minimum_speed_fixed8;
                {
                    register s32 spawn_x asm("r3");
                    spawn_x = *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x));
                    spawned = CreateBattleAngledProjectileSprite(owner, 2, 0,
                        spawn_x, (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)),
                        0x20, fragment_angle, speed_fixed8, 0);
                }
                child_off = index + 9;
                child_off <<= 2;
                *(s32 *)((char *)children + child_off) = (s32)spawned;
                index = (u8)(index + 1);
            } while ((u32)index <= 7);

            SetBattleAnimationCameraMode(8, 0);
            PlayBattleAnimationSound(0);
            {
                register s32 *slot asm("r1") = state_slot;
                *slot = *slot + 1;
            }
        }
        break;
    }
    case 2: {
        index = 0;
        if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            s32 *children = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
            s32 off;
            do {
                index = (u8)(index + 1);
            } while ((u32)index <= 0x10 &&
                (off = index << 2,
                 *(s32 *)((char *)children + off)) == 0);
        }
        if (index == 0x11) {
            DestroySpriteGroup(owner);
        }
        break;
    }
    }
}
