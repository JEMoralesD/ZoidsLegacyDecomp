#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedProjectileTrailSprite(void *) asm("func_080D3F6C");

void DestroySpriteGroup(void *) asm("func_08095114");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void *CreateBattleAnimationSprite() asm("func_080D2450");
void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleSingleSmokeTrailImpactEffect(char *group) asm("func_080D4568");

void UpdateBattleSingleSmokeTrailImpactEffect(char *group)
{
    s32 next_impact_index;
    char *sprite_slots;
    s32 *state_slot;
    s32 *impact_x;
    s32 *impact_y;
    volatile s32 outgoing_reserve;
    s32 y_jitter;
    register s32 impact_sprite_offset asm("r8");
    register s32 y_spread asm("r4");
    register s32 base_y asm("r6");
    register s32 impact_y_value asm("r0");
    register u16 impact_y_bits asm("r0");
    register u16 impact_x_bits asm("r8");
    u32 state;
    u32 state_value;
    register s32 smoke_index asm("r4");
    u8 data_index;
    u8 data_index2;
    register u8 impact_index asm("r5");
    register s32 zero asm("sl");
    register s32 x_shift asm("r6");
    register s32 impact_slot_offset asm("r2");
    register s32 y_shift asm("r9");
    register s32 y_signed asm("r5");
    register s32 impact_index_word asm("r5");
    register s32 x_base asm("r0");
    register s32 x_offset asm("r1");
    register s32 nonzero_impact_index asm("r0");
    register void *spawned asm("r0");
    register s32 impact_slots_offset asm("r2");
    register s32 child_index asm("r1");
    register s32 impact_sprite_slot asm("r1");
    register s32 saved_impact_index asm("r1");
    register s32 impact_index_bits asm("r0");
    register s32 *state_reload asm("r2");
    char *impact_sprite;
    char *projectile;

    asm volatile("" : "=m"(outgoing_reserve));
    state = BATTLE_ANIMATION_FIELD(group, u32, state);
    switch (state) {
    case 0:
        projectile = CreateBattleAnimationSprite(group, 0, 0,
            (s16)(BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x) - 0x100),
            (s32)BATTLE_ANIMATION_FIELD(group, s16, effect.impact.y), 0x20, (s32)UpdateBattleTargetedProjectileTrailSprite, 1);
        BATTLE_ANIMATION_FIELD(group, char *, sprites[0]) = projectile;
        BATTLE_SPRITE_FIELD(projectile, char *, user_data.projectile_trail.group) = group;
        BATTLE_SPRITE_FIELD(projectile, s32, user_data.projectile_trail.elapsed_frames) = 0;
        BATTLE_SPRITE_FIELD(projectile, s32, user_data.projectile_trail.impact_x) = BATTLE_ANIMATION_FIELD(group, s32, effect.impact.x);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group, u32, state) += 1;
        return;

    case 1:
        if (!(M2C_FIELD(BATTLE_ANIMATION_FIELD(group, char *, sprites[0]), s32 *, 0) & 0x20000)) {
            return;
        }
        impact_index = 0;
        state_slot = (s32 *)(group + BATTLE_ANIMATION_OFFSET(state));
        impact_x = (s32 *)(group + BATTLE_ANIMATION_OFFSET(effect.impact.x));
        impact_y = (s32 *)(group + BATTLE_ANIMATION_OFFSET(effect.impact.y));
        sprite_slots = group + BATTLE_ANIMATION_OFFSET(sprites[0]);
        asm volatile("mov %0, %1" : "=r"(zero) : "r"(impact_index));
        asm volatile("" : "=r"(impact_index_word) : "0"(impact_index));
        do {
            x_base = *impact_x;
            x_offset = impact_index_word << 3;
            x_base += x_offset;
            x_base <<= 16;
            x_base = (u32)x_base >> 16;
            impact_x_bits = x_base;
            nonzero_impact_index = -impact_index_word;
            nonzero_impact_index |= impact_index_word;
            y_spread = (nonzero_impact_index >> 31) & 8;
            base_y = *impact_y;
            y_jitter = ((u32)(CallFunctionR0(gRandomNumberCallback) * 9) >> 15) - 4;
            if (1 & impact_index_word) {
                impact_y_value = -y_spread;
                asm volatile("" : "+r"(impact_y_value));
                impact_y_value = base_y + impact_y_value;
            } else {
                impact_y_value = base_y + y_spread;
            }
            impact_y_value += y_jitter;
            asm volatile("lsl %0, %0, #16\n\tlsr %0, %0, #16"
                         : "+r"(impact_y_value));
            impact_y_bits = impact_y_value;
            smoke_index = 0;
            x_shift = impact_x_bits << 16;
            impact_slot_offset = impact_index_word << 2;
            impact_sprite_offset = impact_slot_offset;
            impact_index_word += 1;
            next_impact_index = impact_index_word;
            impact_y_value = impact_y_bits << 16;
            y_signed = impact_y_value >> 16;
            y_shift = impact_y_value;
spawn_impact_smoke:
            {
                register volatile s32 *outgoing asm("sp");
                s32 speed_fixed8;

                speed_fixed8 = ((u32)(CallFunctionR0(gRandomNumberCallback) * 0x41) >> 15)
                    + 0xC0;
                outgoing[0] = y_signed;
                outgoing[1] = BATTLE_SPRITE_SEMITRANSPARENT;
                outgoing[2] = (smoke_index * 0xC) - 0xC;
                outgoing[3] = speed_fixed8;
                outgoing[4] = zero;
                {
                    register char *call0 asm("r0") = group;
                    register s32 call1 asm("r1") = 2;
                    register s32 call2 asm("r2") = 0;
                    register s32 call3 asm("r3");
                    register s32 value asm("r6") = x_shift;

                    asm volatile("" : "+r"(value));
                    call3 = value >> 16;
                    asm volatile("" : "+r"(call0), "+r"(call1),
                                       "+r"(call2), "+r"(call3));
                    spawned = CreateBattleAngledProjectileSprite(call0, call1, call2, call3);
                }
            }
            impact_slots_offset = impact_sprite_offset;
            child_index = impact_slots_offset + smoke_index;
            child_index += 1;
            child_index <<= 2;
            asm volatile("add %0, %1, %0"
                         : "+r"(child_index) : "r"(sprite_slots));
            *(void **)child_index = spawned;
            {
                register s32 next_inner asm("r0") = smoke_index + 1;

                next_inner <<= 24;
                smoke_index = (u32)next_inner >> 24;
            }
            if ((u32)smoke_index <= 2U) {
                goto spawn_impact_smoke;
            }
            spawned = CreateBattleAnimationSprite(group, 3, 0, x_shift >> 16,
                y_shift >> 16, zero, zero, zero);
            impact_sprite_slot = impact_sprite_offset;
            impact_sprite_slot += 4;
            impact_sprite_slot <<= 2;
            asm volatile("add %0, %1, %0"
                         : "+r"(impact_sprite_slot) : "r"(sprite_slots));
            *(void **)impact_sprite_slot = spawned;
            saved_impact_index = next_impact_index;
            impact_index_bits = saved_impact_index << 24;
            impact_index = (u32)impact_index_bits >> 24;
        } while (impact_index == 0);
        SetBattleAnimationCameraMode(6, 0);
        PlayBattleAnimationSound(1);
        state_reload = state_slot;
        *state_reload += 1;
        return;

    case 2:
        {
            register char *trail_projectile asm("r0") = BATTLE_ANIMATION_FIELD(group, char *, sprites[0]);
            s32 projectile_hidden = M2C_FIELD(trail_projectile, s32 *, 0) & 0x20000;
            register char *child asm("r3") = trail_projectile;

            if (projectile_hidden) {
                register s32 scan asm("r5");
                u32 elapsed_frames;
                u32 bound;
                s32 off;

                scan = 0x18;
                asm volatile("" : "+r"(scan));
                elapsed_frames = M2C_FIELD(child, u32 *, 0x2C);
                bound = (elapsed_frames >> 2) + 0x19;
                if ((u32)scan < bound && BATTLE_ANIMATION_FIELD(group, s32, sprites[24]) == 0) {
                    u32 scan_bound = bound;
                    s32 *children = (s32 *)(group + BATTLE_ANIMATION_OFFSET(sprites[0]));

                    do {
                        register s32 next asm("r0");

                        next = scan + 1;
                        next <<= 24;
                        scan = (u32)next >> 24;
                    } while ((u32)scan < scan_bound &&
                        (off = scan << 2,
                         *(s32 *)((char *)children + off)) == 0);
                }
                if (scan == ((*(volatile u32 *)(child + 0x2C) >> 2) + 0x19)) {
                    scan = 1;
                    if (BATTLE_ANIMATION_FIELD(group, s32, sprites[1]) == 0) {
                        s32 *children = (s32 *)(group + BATTLE_ANIMATION_OFFSET(sprites[0]));

                        do {
                            register s32 next asm("r0");

                            next = scan + 1;
                            next <<= 24;
                            scan = (u32)next >> 24;
                        } while ((u32)scan <= 4 &&
                            (off = scan << 2,
                             *(s32 *)((char *)children + off)) == 0);
                    }
                    if (scan == 5) {
                        DestroySpriteGroup(group);
                    }
                }
            }
        }
        return;
    }
}
