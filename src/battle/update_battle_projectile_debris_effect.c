#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleProjectileDebrisEffect(void *group) asm("func_080D2CA4");

void UpdateBattleProjectileDebrisEffect(void *group) {
    register char *owner asm("r6") = group;
    register s32 *state_slot asm("sl") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    u8 index;
    register u32 state asm("r0") = *state_slot;

    switch (state) {
    case BATTLE_PROJECTILE_IMPACT_LAUNCH: {
        register char *x_ptr asm("r0") = owner + BATTLE_ANIMATION_OFFSET(effect.impact.x);
        register s32 x_value asm("r3");
        register s32 y_offset asm("r2");
        void *created;

        x_value = *(s32 *)x_ptr;
        asm volatile("" : "+r"(x_ptr));
        x_value = (s16)(x_value - 0x100);
        x_ptr += 4;
        asm volatile(
            "mov %1, #0\n\t"
            "ldrsh %0, [%0, %1]"
            : "+r"(x_ptr), "=r"(y_offset));
        created = CreateBattleAnimationSprite(owner, 0, 0, x_value, (s32)x_ptr,
            0, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        BATTLE_SPRITE_FIELD(created, s32, user_data.horizontal_projectile.speed) = 0x10;
        PlayBattleAnimationSound(0);
        {
            register s32 *slot asm("r4") = state_slot;
            *slot = *slot + 1;
        }
        break;
    }
    case BATTLE_PROJECTILE_IMPACT_WAIT: {
        register char *x_ptr asm("r9");
        register char *y_ptr asm("r5");
        register s32 *children asm("r9");
        register u32 *rng asm("r5");
        register void *guard asm("r7");
        s32 * volatile saved_state;
        volatile s32 angle_slot;

        guard = *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
        if (guard != 0) {
            break;
        }
        {
            register s32 x_offset asm("r1");
            register s32 x_value asm("r3");
            register char *x_view asm("r0") = (char *)BATTLE_ANIMATION_OFFSET(effect.impact.x);
            register s32 y_offset asm("r2");
            register s32 y_value asm("r0");

            asm volatile("add %0, %0, %1"
                         : "+r"(x_view) : "r"(owner));
            x_ptr = x_view;
            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(x_offset), "=r"(x_value)
                : "r"(x_view));
            y_ptr = owner + BATTLE_ANIMATION_OFFSET(effect.impact.y);
            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(y_offset), "=r"(y_value)
                : "r"(y_ptr));
            {
                register volatile s32 *outgoing asm("sp");
                register char *call0 asm("r0");
                register s32 call1 asm("r1");
                register s32 call2 asm("r2");

                outgoing[0] = y_value;
                outgoing[1] = (s32)guard;
                outgoing[2] = (s32)guard;
                outgoing[3] = (s32)guard;
                call0 = owner;
                call1 = 1;
                asm volatile("mov %0, #0" : "=r"(call2));
                *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(
                    call0, call1, call2, x_value);
            }
        }
        {
            register u32 *rng_view asm("r4") = &gRandomNumberCallback;
            register s32 angle asm("r4");
            register s32 x_value asm("r3");
            register s32 x_offset asm("r1");
            register char *x_view asm("r0");
            register s32 y_value asm("r0");
            register s32 y_offset asm("r2");
            void *spawned;

            angle = (u32)(CallFunctionR0(*rng_view) * 0x41) >> 15;
            angle -= 0x20;
            asm volatile(
                "mov %0, %3\n\t"
                "mov %1, #0\n\t"
                "ldrsh %2, [%0, %1]"
                : "=r"(x_view), "=r"(x_offset), "=r"(x_value)
                : "r"(x_ptr));
            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(y_offset), "=r"(y_value)
                : "r"(y_ptr));
            {
                register volatile s32 *outgoing asm("sp");
                register char *call0 asm("r0");
                register s32 call1 asm("r1");
                register s32 call2 asm("r2");

                outgoing[0] = y_value;
                outgoing[1] = 0x100;
                outgoing[2] = angle;
                outgoing[3] = 0x800;
                outgoing[4] = (s32)guard;
                call0 = owner;
                call1 = 2;
                asm volatile("mov %0, #0" : "=r"(call2));
                spawned = CreateBattleAngledProjectileSprite(call0, call1, call2, x_value);
            }
            *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[1])) = spawned;
            BATTLE_SPRITE_FIELD(spawned, s8, rotation) = angle;
        }

        index = 2;
        {
            register s32 *saved_state_view asm("r4") = state_slot;
            saved_state = saved_state_view;
        }
        {
            register char *saved_x asm("r8") = x_ptr;
            register char *saved_y asm("sl") = y_ptr;

            {
                register s32 *children_init asm("r0") = (s32 *)0xC;
                asm volatile("" : "+r"(children_init));
                children_init = (s32 *)((char *)children_init + (s32)owner);
                children = children_init;
            }
            rng = &gRandomNumberCallback;
            do {
                register s32 random asm("r0");
                register s32 angle asm("r4");
                register s32 speed_fixed8 asm("r1");
                register s32 minimum_speed_fixed8 asm("r2");
                register char *x_view asm("r4");
                register s32 x_offset asm("r0");
                register s32 x_value asm("r3");
                register char *y_view asm("r2");
                register s32 y_offset asm("r4");
                register s32 y_value asm("r0");
                register s32 child_offset asm("r1");
                void *spawned;

                random = CallFunctionR0(*rng);
                angle = random * 3;
                angle <<= 4;
                angle += random;
                angle = (u32)angle >> 15;
                angle += 0x80;
                angle_slot = angle;
                speed_fixed8 = (u32)(CallFunctionR0(*rng) * 0x101) >> 15;
                minimum_speed_fixed8 = 0x100;
                asm volatile("" : "+r"(minimum_speed_fixed8));
                speed_fixed8 += minimum_speed_fixed8;
                asm volatile(
                    "mov %0, %3\n\t"
                    "mov %1, #0\n\t"
                    "ldrsh %2, [%0, %1]"
                    : "=r"(x_view), "=r"(x_offset), "=r"(x_value)
                    : "r"(saved_x));
                asm volatile(
                    "mov %0, %3\n\t"
                    "mov %1, #0\n\t"
                    "ldrsh %2, [%0, %1]"
                    : "=r"(y_view), "=r"(y_offset), "=r"(y_value)
                    : "r"(saved_y));
                spawned = CreateBattleAngledProjectileSprite(owner, 3, 0, x_value, y_value,
                    0x500, angle_slot, speed_fixed8, 0);
                child_offset = index << 2;
                asm volatile("add %0, %1" : "+r"(child_offset)
                             : "r"(children));
                *(void **)child_offset = spawned;
                index = (u8)(index + 1);
            } while (index <= 5);
        }
        SetBattleAnimationCameraMode(6, 0);
        PlayBattleAnimationSound(1);
        {
            register s32 *slot asm("r1") = saved_state;
            *slot = *slot + 1;
        }
        break;
    }
    case BATTLE_PROJECTILE_IMPACT_FINISH:
        index = 0;
        if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *children asm("r1") = owner + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                index = (u8)(index + 1);
            } while (index <= 5 &&
                *(s32 *)(children + (index << 2)) == 0);
        }
        if (index == 6) {
            DestroySpriteGroup(owner);
        }
        break;
    }
}
