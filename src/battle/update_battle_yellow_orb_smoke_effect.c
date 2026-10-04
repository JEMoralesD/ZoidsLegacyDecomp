#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleYellowOrbTrailSprite(void *) asm("func_080D980C");
extern void DestroySpriteGroup() asm("func_08095114");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound() asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleYellowOrbSmokeEffect(struct BattleAnimationGroup *group) asm("func_080D999C");

void UpdateBattleYellowOrbSmokeEffect(struct BattleAnimationGroup *group) {
    char *group_bytes = group;
    register s32 *effect_state asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register s32 state asm("r1") = *effect_state;
    register s32 sprite_index asm("r8");

    if (state == BATTLE_PROJECTILE_TRAIL_CREATE) {
        register s32 *sprite_slots asm("r10");
        register u32 *random_callback_slot asm("r9");
        s32 *saved_effect_state;
        void *orb_sprite;

        orb_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
            (s32)*(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleYellowOrbTrailSprite, state);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = orb_sprite;
        sprite_index = 0;
        saved_effect_state = effect_state;
        sprite_slots = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        random_callback_slot = &gRandomNumberCallback;
        do {
            register s32 x asm("r6");
            register s32 y asm("r4");
            register s32 angle asm("r5");
            s32 speed_fixed8;
            void *smoke_sprite;
            s32 next_sprite_index;
            s32 sprite_offset;

            {
                register u32 *random_callback_view_4 asm("r4") = random_callback_slot;
                register u32 random asm("r0");
                random = CallFunctionR0(*random_callback_view_4);
                x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) - 2;
                x += (random * 5) >> 15;
                x = (s16)x;
                random = CallFunctionR0(*random_callback_view_4);
                y = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) - 2;
                y += (random * 5) >> 15;
                y = (s16)y;
            }
            {
                register u32 *random_callback_view_1 asm("r1") = random_callback_slot;
                angle = (CallFunctionR0(*random_callback_view_1) * 0x11) >> 15;
            }
            {
                register u32 *random_callback_view_2 asm("r2") = random_callback_slot;
                speed_fixed8 = ((CallFunctionR0(*random_callback_view_2) * 0x41) >> 15) + 0x40;
            }
            smoke_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0, x, y,
                BATTLE_SPRITE_SEMITRANSPARENT, angle, speed_fixed8, 0);
            next_sprite_index = sprite_index + 1;
            sprite_offset = next_sprite_index << 2;
            *(s32 *)((char *)sprite_slots + sprite_offset) = (s32)smoke_sprite;
            {
                register s32 next_index_bits asm("r2") = next_sprite_index;
                next_index_bits <<= 24;
                next_index_bits = (u32)next_index_bits >> 24;
                sprite_index = next_index_bits;
            }
        } while ((u32)sprite_index <= 3);
        orb_sprite = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        BATTLE_SPRITE_FIELD(orb_sprite, void *, user_data.projectile_trail.group) = group_bytes;
        BATTLE_SPRITE_FIELD(orb_sprite, s32, user_data.projectile_trail.elapsed_frames) = 0;
        PlayBattleAnimationSound(0);
        *saved_effect_state = *saved_effect_state + 1;
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
            if (sprite_index == ((*(volatile u32 *)((char *)orb + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames)) >> 1) + 6)) {
                DestroySpriteGroup(group_bytes);
            }
        }
    }
}
