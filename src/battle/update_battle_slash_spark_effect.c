#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleSlashSparkEffect(void *group) asm("func_080D5764");

void UpdateBattleSlashSparkEffect(void *group)
{
    register char *owner asm("r5") = group;
    register s32 *effect_state asm("r4") = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r0") = *effect_state;
    register s32 index asm("r6");

    switch (state) {
    case 0:
        goto create_slash;
    case 1:
        goto emit_sparks;
    case 2:
        goto wait_for_sprites;
    default:
        return;
    }

create_slash:
    *(void **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(owner, 0, 0,
        *(s16 *)(owner + BATTLE_ANIMATION_OFFSET(x)), (s32)*(s16 *)(owner + BATTLE_ANIMATION_OFFSET(y)), 0, 0, 1);
    *effect_state = *effect_state + 1;
    return;

emit_sparks:
    if (*(u16 *)(*(char **)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) + 0x14) == 5) {
        register s32 *state_slot asm("r9");
        register s32 *children asm("r8");
        register u32 *rng;

        index = 0;
        state_slot = effect_state;
        children = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
        rng = &gRandomNumberCallback;
        do {
            register s32 spark_angle asm("r4");
            register s32 random asm("r0");
            register s32 speed_fixed8 asm("r1");
            register s32 minimum_speed_fixed8 asm("r0");
            register s32 next asm("r2");
            register s32 child_off asm("r1");
            void *spawned;

            random = CallFunctionR0(*rng);
            spark_angle = random << 1;
            spark_angle += random;
            spark_angle <<= 4;
            spark_angle += random;
            spark_angle = (u32)spark_angle >> 15;
            spark_angle += 0x80;
            random = CallFunctionR0(*rng);
            speed_fixed8 = random << 9;
            speed_fixed8 += random;
            speed_fixed8 = (u32)speed_fixed8 >> 15;
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
        SetBattleAnimationCameraMode(8, 0);
        PlayBattleAnimationSound(0);
        {
            register s32 *slot asm("r1") = state_slot;

            *slot = *slot + 1;
        }
    }
    return;

wait_for_sprites:
    index = 0;
    if (*(s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
        s32 *children = (s32 *)(owner + BATTLE_ANIMATION_OFFSET(sprites[0]));
        s32 off;

        do {
            register s32 next asm("r0") = index + 1;

            next <<= 24;
            index = (u32)next >> 24;
        } while ((u32)index <= 8 &&
            (off = index << 2,
             *(s32 *)((char *)children + off)) == 0);
    }
    if (index == 9) {
        DestroySpriteGroup(owner);
    }
}
