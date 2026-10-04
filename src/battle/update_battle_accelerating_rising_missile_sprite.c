#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleAcceleratingRisingMissileSprite(struct BattleDisplaySprite *input_sprite) asm("func_080D6BDC");

void UpdateBattleAcceleratingRisingMissileSprite(struct BattleDisplaySprite *input_sprite) {
    volatile s32 reserve0;
    volatile s32 reserve1;
    volatile s32 reserve2;
    volatile s32 reserve3;
    volatile s32 reserve4;
    register struct BattleDisplaySprite *sprite asm("r6") = input_sprite;
    register s32 check_screen_bounds asm("r3") = 0;
    char *group_bytes = BATTLE_SPRITE_FIELD(sprite, void *, user_data.accelerating_rising_missile.group);
    u32 elapsed_frames = BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames);

    asm volatile("" : "=m"(reserve0), "=m"(reserve1), "=m"(reserve2),
                       "=m"(reserve3), "=m"(reserve4));

    if (elapsed_frames <= 0xF) {
        register u32 elapsed_frames_view asm("r2");

        if (elapsed_frames == 0) {
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) = BATTLE_SPRITE_FIELD(sprite, s16, x) << 8;
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.y_fixed8) = BATTLE_SPRITE_FIELD(sprite, s16, y) << 8;
        }
        if ((BATTLE_SPRITE_FIELD(sprite, u32, flags) & BATTLE_SPRITE_FLIP_X) == 0) {
            elapsed_frames_view = BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames);
            asm volatile("" : "+r"(elapsed_frames_view));
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) -= elapsed_frames_view << 7;
        } else {
            elapsed_frames_view = BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames);
            asm volatile("" : "+r"(elapsed_frames_view));
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) += elapsed_frames_view << 7;
        }
        {
            register s32 y_delta asm("r0") = elapsed_frames_view << 6;
            register s32 y_fixed8 asm("r1") = BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.y_fixed8);

            y_fixed8 -= y_delta;
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.y_fixed8) = y_fixed8;
            BATTLE_SPRITE_FIELD(sprite, s16, x) = (u32)BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) >> 8;
            BATTLE_SPRITE_FIELD(sprite, s16, y) = (u32)y_fixed8 >> 8;
        }

        {
            u32 emission_phase = 3;
            emission_phase &= elapsed_frames_view;
            if (emission_phase == 0) {
                register u32 *rng asm("r5") = &gRandomNumberCallback;
                register s32 x asm("r4");
                register s32 y asm("r2");
                register u32 random asm("r0");
                register volatile s32 *outgoing asm("sp");
                void *smoke_sprite;

                random = CallFunctionR0(*rng);
                x = (u16)BATTLE_SPRITE_FIELD(sprite, s16, x) - 2;
                x += (random * 5) >> 15;
                x = (s16)x;
                random = CallFunctionR0(*rng);
                y = (u16)BATTLE_SPRITE_FIELD(sprite, s16, y) - 2;
                y += (random * 5) >> 15;
                y = (s16)y;

                outgoing[0] = y;
                outgoing[1] = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
                outgoing[2] = 0x13;
                outgoing[3] = 0x100;
                outgoing[4] = BATTLE_ANIMATION_SPRITE_KEEP_SCREEN_X;
                smoke_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0, x);
                {
                    register u32 offset asm("r1") = BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames);
                    register char *children asm("r2");
                    offset >>= 2;
                    offset += 1;
                    offset <<= 2;
                    children = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    children += offset;
                    *(void **)children = smoke_sprite;
                }
                goto check_boundary;
            }
        }
    } else {
        register u32 flags asm("r1") = BATTLE_SPRITE_FIELD(sprite, u32, flags);
        register u32 direction asm("r0") = flags;
        register u32 sprite_flags asm("r2");

        direction &= BATTLE_SPRITE_FLIP_X;
        sprite_flags = flags;
        asm volatile("" : "+r"(direction), "+r"(sprite_flags));

        if (direction == 0) {
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) -= 0x1000;
        } else {
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) += 0x1000;
        }
        {
            register s32 y_fixed8 asm("r1") = BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.y_fixed8);
            register s32 y_delta asm("r0") = -0x800;

            y_fixed8 += y_delta;
            BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.y_fixed8) = y_fixed8;
            BATTLE_SPRITE_FIELD(sprite, s16, x) = (u32)BATTLE_SPRITE_FIELD(sprite, s32, user_data.accelerating_rising_missile.x_fixed8) >> 8;
            BATTLE_SPRITE_FIELD(sprite, s16, y) = (u32)y_fixed8 >> 8;
        }

        if ((BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames) & 1) == 0) {
            register s32 x asm("r3");
            register s32 previous_x asm("r1");
            register volatile s32 *outgoing asm("sp");
            void *smoke_sprite;

            previous_x = BATTLE_SPRITE_FIELD(sprite, s16, x);
            if ((sprite_flags & BATTLE_SPRITE_FLIP_X) == 0) {
                register s32 next_x asm("r0") = previous_x;
                next_x += 0x20;
                x = (s16)next_x;
            } else {
                register s32 next_x asm("r0") = previous_x;
                next_x -= 0x20;
                x = (s16)next_x;
            }
            outgoing[0] = (s16)((u16)BATTLE_SPRITE_FIELD(sprite, s16, y) + 0x10);
            outgoing[1] = (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1);
            outgoing[2] = 0x13;
            outgoing[3] = 0x80;
            outgoing[4] = BATTLE_ANIMATION_SPRITE_KEEP_SCREEN_X;
            smoke_sprite = CreateBattleAngledProjectileSprite(group_bytes, 1, 0, x);
            {
                register u32 offset asm("r1") = BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames);
                register char *children asm("r2");
                offset -= 0x10;
                offset >>= 1;
                offset += 5;
                offset <<= 2;
                children = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                children += offset;
                *(void **)children = smoke_sprite;
            }
            check_screen_bounds = 1;
        }
    }

    if (check_screen_bounds == 0) {
        goto advance;
    }
check_boundary:
    /* Native exit requires both an offscreen x and a negative y. */
    {
        register u32 flags asm("r1") = BATTLE_SPRITE_FIELD(sprite, u32, flags);
        register u32 direction asm("r0") = flags;
        register u32 sprite_flags asm("r2");

        direction &= BATTLE_SPRITE_FLIP_X;
        sprite_flags = flags;
        asm volatile("" : "+r"(direction), "+r"(sprite_flags));

        if (direction == 0) {
            if (BATTLE_SPRITE_FIELD(sprite, s16, x) < 0) {
                goto check_y;
            }
            goto advance;
        } else if (BATTLE_SPRITE_FIELD(sprite, s16, x) <= 0xEF) {
            goto advance;
        }
check_y:
        if (BATTLE_SPRITE_FIELD(sprite, s16, y) < 0) {
            BATTLE_SPRITE_FIELD(sprite, u32, flags) = sprite_flags | BATTLE_SPRITE_HIDDEN;
            BATTLE_SPRITE_FIELD(sprite, s32, update_callback) = 0;
            return;
        }
    }
advance:
    BATTLE_SPRITE_FIELD(sprite, u32, user_data.accelerating_rising_missile.elapsed_frames) += 1;
}
