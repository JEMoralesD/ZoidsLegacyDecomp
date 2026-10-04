#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleRisingGreenOrbTrailSprite(void *) asm("func_080D8B70");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound() asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleRisingGreenOrbSmokeEffect(void *group) asm("func_080D8DEC");

void UpdateBattleRisingGreenOrbSmokeEffect(void *group) {
    char *group_bytes = group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 phase asm("r1") = *effect_state;
    register s32 sprite_index asm("r8");

    if (phase == BATTLE_PROJECTILE_TRAIL_CREATE) {
        register s32 *sprite_slots asm("r10");
        register u32 *rng asm("r9");
        s32 *state_slot;
        void *orb;

        orb = CreateBattleAnimationSprite(group_bytes, 0, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleRisingGreenOrbTrailSprite, phase);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = orb;
        sprite_index = 0;
        state_slot = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        rng = &gRandomNumberCallback;
        do {
            register s32 x asm("r6");
            register s32 y asm("r4");
            register s32 angle asm("r5");
            s32 speed_fixed8;
            void *smoke_sprite;
            s32 next;
            s32 sprite_offset;

            {
                register u32 *rng4 asm("r4") = rng;
                register u32 random asm("r0");
                random = CallFunctionR0(*rng4);
                x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - 2;
                x += (random * 5) >> 15;
                x = (s16)x;
                random = CallFunctionR0(*rng4);
                y = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) - 2;
                y += (random * 5) >> 15;
                y = (s16)y;
            }
            {
                register u32 *rng1 asm("r1") = rng;
                angle = (CallFunctionR0(*rng1) * 0x11) >> 15;
            }
            {
                register u32 *rng2 asm("r2") = rng;
                speed_fixed8 = ((CallFunctionR0(*rng2) * 0x41) >> 15) + 0x40;
            }
            smoke_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0, x, y,
                BATTLE_SPRITE_SEMITRANSPARENT, angle, speed_fixed8, 0);
            next = sprite_index + 1;
            sprite_offset = next << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)smoke_sprite;
            {
                register s32 next_index_bits asm("r2") = next;
                next_index_bits <<= 24;
                next_index_bits = (u32)next_index_bits >> 24;
                sprite_index = next_index_bits;
            }
        } while ((u32)sprite_index <= 3);
        orb = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        BATTLE_SPRITE_FIELD(orb, void *, user_data.projectile_trail.group) = group_bytes;
        BATTLE_SPRITE_FIELD(orb, s32, user_data.projectile_trail.elapsed_frames) = 0;
        PlayBattleAnimationSound(0);
        *state_slot = *state_slot + 1;
        return;
    }
    {
        register void *orb_view asm("r0") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        s32 orb_hidden = *(s32 *)orb_view & BATTLE_SPRITE_HIDDEN;
        register void *orb asm("r3") = orb_view;

        if (orb_hidden) {
            u32 trail_slot_end;

            sprite_index = 1;
            trail_slot_end = (BATTLE_SPRITE_FIELD(orb, u32, user_data.projectile_trail.elapsed_frames) >> 1) + 6;
            if ((u32)sprite_index < trail_slot_end && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                u32 trail_scan_end = trail_slot_end;
                s32 *sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
                do {
                    register s32 next_index_bits asm("r0");
                    next_index_bits = sprite_index;
                    next_index_bits += 1;
                    next_index_bits <<= 24;
                    next_index_bits = (u32)next_index_bits >> 24;
                    sprite_index = next_index_bits;
                } while ((u32)sprite_index < trail_scan_end &&
                    *(s32 *)((char *)sprite_slots + (sprite_index << 2)) == 0);
            }
            if (sprite_index == ((*(volatile u32 *)((char *)orb + 0x2C) >> 1) + 6)) {
                DestroySpriteGroup(group_bytes);
            }
        }
    }
}
