#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void DestroySprite(void *) asm("func_08094554");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern s32 IsBattleAnimationSpriteAtFacingPosition(void *, s32, s32) asm("func_080D2754");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleProjectileImpactDebrisEffect(void *group) asm("func_080D35B4");

void UpdateBattleProjectileImpactDebrisEffect(void *group) {
    register char *owner asm("r6") = group;
    s32 *state_slot = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r0") = *state_slot;
    u8 index;

    switch (state) {
    case BATTLE_PROJECTILE_IMPACT_LAUNCH: {
        void *created = CreateBattleAnimationSprite(owner, 0, 0,
            (s16)(*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x)) - 0x100),
            *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y)), 0x410, (s32)UpdateBattleHorizontalProjectileSprite, 1);
        *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = created;
        BATTLE_SPRITE_FIELD(created, s32, user_data.horizontal_projectile.speed) = 0x10;
        PlayBattleAnimationSound(0);
        *state_slot = *state_slot + 1;
        break;
    }
    case BATTLE_PROJECTILE_IMPACT_WAIT: {
        register s32 *x_ptr asm("r5");
        register s32 *y_ptr asm("r4");
        register s32 *saved_x asm("r9");
        register s32 *saved_y asm("r8");
        register s32 *children asm("sl");
        s32 reached;
        s32 * volatile saved_state;

        reached = IsBattleAnimationSpriteAtFacingPosition(*(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])),
            *(x_ptr = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.x))) - 0x20,
            *(y_ptr = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(effect.impact.y)))) << 24;
        saved_x = x_ptr;
        saved_y = y_ptr;
        if (reached == 0) {
            break;
        }
        DestroySprite(*(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])));
        {
            register s32 x_offset asm("r2");
            register s32 x_value asm("r3");
            register s32 y_offset asm("r1");
            register s32 y_value asm("r0");

            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(x_offset), "=r"(x_value)
                : "r"(x_ptr));
            asm volatile(
                "mov %0, #0\n\t"
                "ldrsh %1, [%2, %0]"
                : "=r"(y_offset), "=r"(y_value)
                : "r"(y_ptr));
            *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(owner, 1, 0,
                x_value, y_value, 0, 0, 0);
        }

        index = 1;
        saved_state = state_slot;
        {
            register s32 *children_init asm("r2") = (s32 *)0xC;
            asm volatile("" : "+r"(children_init));
            children_init = (s32 *)((char *)children_init + (s32)owner);
            children = children_init;
        }
        do {
            register u32 *rng asm("r7") = &gRandomNumberCallback;
            register s32 random asm("r0");
            register s32 angle asm("r4");
            register s32 speed_fixed8 asm("r1");
            register s32 minimum_speed_fixed8 asm("r0");
            register s32 child_offset asm("r1");
            register char *x_view asm("r2");
            register s32 x_offset asm("r7");
            register s32 x_value asm("r3");
            register char *y_view asm("r2");
            register s32 y_offset asm("r7");
            register s32 y_value asm("r0");
            void *spawned;

            random = CallFunctionR0(*rng);
            angle = (u32)(random * 0x41) >> 15;
            angle -= 0x20;
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
            spawned = CreateBattleAngledProjectileSprite(owner, 2, 0, x_value, y_value,
                0x500, angle, speed_fixed8, 0);
            child_offset = index << 2;
            asm volatile("add %0, %1" : "+r"(child_offset)
                         : "r"(children));
            *(void **)child_offset = spawned;
            index = (u8)(index + 1);
        } while (index <= 4);

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
            } while (index <= 4 &&
                *(s32 *)(children + (index << 2)) == 0);
        }
        if (index == 5) {
            DestroySpriteGroup(owner);
        }
        break;
    }
}
