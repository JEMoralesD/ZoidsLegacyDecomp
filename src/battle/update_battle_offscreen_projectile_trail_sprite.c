#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"


void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32) asm("func_080D2660");
u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleOffscreenProjectileTrailSprite(struct BattleDisplaySprite *input_sprite) asm("func_080D3E9C");

void UpdateBattleOffscreenProjectileTrailSprite(struct BattleDisplaySprite *input_sprite)
{
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    register struct BattleDisplaySprite *sprite asm("r6") = input_sprite;
    void *group = sprite->user_data.projectile_trail.group;
    register s32 previous_x asm("r2") = *(s16 *)&sprite->x;
    register u32 facing_test asm("r0") = sprite->flags;
    register u32 facing_mask asm("r1") = 0x80;
    register s32 next_x asm("r0");

    asm volatile("" : "=m"(reserve0), "=m"(reserve1),
                       "=m"(reserve2), "=m"(reserve3),
                       "=m"(reserve4));
    facing_mask <<= 8;
    facing_test &= facing_mask;
    if (facing_test == 0) {
        next_x = previous_x;
        asm volatile("" : "+r"(next_x));
        next_x -= BATTLE_PROJECTILE_TRAIL_STEP_PIXELS;
    } else {
        next_x = previous_x;
        asm volatile("" : "+r"(next_x));
        next_x += BATTLE_PROJECTILE_TRAIL_STEP_PIXELS;
    }
    sprite->x = next_x;
    asm volatile("" : "+r"(facing_mask));

    /* Each trail slot receives two emissions before the next slot is used. */
    if ((sprite->user_data.projectile_trail.elapsed_frames & (BATTLE_PROJECTILE_TRAIL_EMIT_INTERVAL - 1)) == 0) {
        register u32 *rng asm("r5") = &gRandomNumberCallback;
        register s32 x asm("r4");
        register s32 y asm("r2");
        register u32 random asm("r0");
        register volatile s32 *outgoing asm("sp");
        void *created;

        random = CallFunctionR0(*rng);
        x = sprite->x - 2;
        x += (random * 5) >> 15;
        x = (s16)x;
        random = CallFunctionR0(*rng);
        y = sprite->y - 2;
        y += (random * 5) >> 15;
        y = (s16)y;

        outgoing[0] = y;
        outgoing[1] = BATTLE_SPRITE_SEMITRANSPARENT;
        outgoing[2] = 0x7C;
        outgoing[3] = 0x40;
        outgoing[4] = 2;
        created = CreateBattleAngledProjectileSprite(group, 1, 0, x);
        {
            register u32 offset asm("r1") = sprite->user_data.projectile_trail.elapsed_frames;
            register u8 *children asm("r2");

            /* Native elapsed frame 32 writes slot 32, which overlaps group state. */
            offset >>= 2;
            offset += BATTLE_PROJECTILE_TRAIL_FIRST_SPRITE;
            offset <<= 2;
            children = (u8 *)group + 0xC;
            children += offset;
            *(void **)children = created;
        }

        {
            register u32 test asm("r0") = sprite->flags;
            register u32 mask asm("r1") = 0x80;

            mask <<= 8;
            test &= mask;
        if (test == 0) {
            if (*(s16 *)&sprite->x < 0) {
                goto stop_trail;
            }
        } else if (*(s16 *)&sprite->x > BATTLE_PROJECTILE_TRAIL_SCREEN_LAST_X) {
stop_trail:
            sprite->flags |= BATTLE_SPRITE_HIDDEN;
            sprite->update_callback = 0;
            return;
        }
        }
    }
    sprite->user_data.projectile_trail.elapsed_frames += 1;
}
