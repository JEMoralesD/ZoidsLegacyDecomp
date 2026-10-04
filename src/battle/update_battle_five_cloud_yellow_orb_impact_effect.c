#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void UpdateBattleTargetedYellowOrbTrailSprite(void *) asm("func_080D98C0");
extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern u32 CallFunctionR0(u32) asm("func_080ECD5C");
extern u32 gRandomNumberCallback asm("D_03000010");

void UpdateBattleFiveCloudYellowOrbImpactEffect(struct BattleAnimationGroup *group) asm("func_080D9B34");

void UpdateBattleFiveCloudYellowOrbImpactEffect(struct BattleAnimationGroup *group) {
    register char *group_bytes asm("r5") = group;
    register s32 *saved_effect_state asm("r6") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
    register u32 state asm("r0") = *saved_effect_state;

    switch (state) {
    case BATTLE_ORB_CLOUD_IMPACT_LAUNCH: {
        register s32 *impact_x_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x));
        register s32 x asm("r3") = *impact_x_slot - 0x100;
        register s32 y asm("r0");
        void *orb_sprite;

        x = (s16)x;
        asm volatile("" : "+r"(x));
        {
            register s16 *impact_y_view asm("r0") = (s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y));
            register s32 impact_y_index asm("r1") = 0;
            y = impact_y_view[impact_y_index];
        }
        orb_sprite = CreateBattleAnimationSprite(group_bytes, 0, 0, x, y,
            BATTLE_SPRITE_LOOP_ANIMATION, (s32)UpdateBattleTargetedYellowOrbTrailSprite, 1);
        *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = orb_sprite;
        BATTLE_SPRITE_FIELD(orb_sprite, void *, user_data.projectile_trail.group) = group_bytes;
        BATTLE_SPRITE_FIELD(orb_sprite, s32, user_data.projectile_trail.elapsed_frames) = 0;
        BATTLE_SPRITE_FIELD(orb_sprite, s32, user_data.projectile_trail.impact_x) = *impact_x_slot;
        PlayBattleAnimationSound(0);
        *saved_effect_state = *saved_effect_state + 1;
        break;
    }
    case BATTLE_ORB_CLOUD_IMPACT_CREATE_CLOUDS:
        if ((**(u32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) & BATTLE_SPRITE_HIDDEN) == 0) {
            break;
        }
        {
            register s32 *impact_elapsed_init asm("r1") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.elapsed_frames));
            register s32 *impact_elapsed_slot asm("r6");
            register u32 elapsed_impact_frames asm("r2") = *impact_elapsed_init;
            register u32 emission_phase asm("r0") = elapsed_impact_frames & 3;

            impact_elapsed_slot = impact_elapsed_init;

            if (emission_phase == 0) {
                register s32 impact_x_base asm("r4") = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.x));
                s32 x;
                register s32 impact_y_base asm("r4");
                register s32 y asm("r0");
                void *cloud_sprite;

                if (elapsed_impact_frames != 0) {
                    register u32 random asm("r0") =
                        CallFunctionR0(gRandomNumberCallback);
                    register s32 jittered_coordinate asm("r2") = impact_x_base - 0x10;
                    jittered_coordinate += (u32)(random * 0x21) >> 15;
                    x = (s16)jittered_coordinate;
                } else {
                    register s32 impact_x_bits asm("r0");
                    asm volatile(
                        "lsl %1, %2, #16\n\t"
                        "asr %0, %1, #16"
                        : "=&r"(x), "=&r"(impact_x_bits) : "r"(impact_x_base));
                }
                impact_y_base = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.impact.y));
                if (*impact_elapsed_slot != 0) {
                    register u32 random asm("r0") =
                        CallFunctionR0(gRandomNumberCallback);
                    register s32 jittered_coordinate asm("r2") = impact_y_base - 0x10;
                    jittered_coordinate += (u32)(random * 0x21) >> 15;
                    asm volatile(
                        "lsl %1, %1, #16\n\t"
                        "asr %0, %1, #16"
                        : "=r"(y), "+r"(jittered_coordinate));
                } else {
                    asm volatile(
                        "lsl %0, %1, #16\n\t"
                        "asr %0, %0, #16"
                        : "=&r"(y) : "r"(impact_y_base));
                }
                cloud_sprite = CreateBattleAnimationSprite(group_bytes, 2, 0, x, y,
                    BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
                {
                    register u32 slot_index asm("r1") =
                        ((u32)*impact_elapsed_slot >> 2) + 1;
                    register char *sprite_slots asm("r2");
                    slot_index <<= 2;
                    sprite_slots = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                    asm volatile("add %0, %0, %1"
                        : "+r"(sprite_slots) : "r"(slot_index));
                    *(void **)sprite_slots = cloud_sprite;
                }
            }
            if (*impact_elapsed_slot == 0) {
                PlayBattleAnimationSound(1);
            }
            {
                register s32 next_elapsed_frames asm("r0") = *impact_elapsed_slot + 1;
                *impact_elapsed_slot = next_elapsed_frames;
                if (next_elapsed_frames == 0x14) {
                    SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE, 0);
                    {
                        register s32 *next_phase_slot asm("r1") =
                            (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
                        *next_phase_slot = *next_phase_slot + 1;
                    }
                }
            }
        }
        break;
    case BATTLE_ORB_CLOUD_IMPACT_WAIT_FOR_SPRITES: {
        register u32 sprite_index asm("r2") = 0x10;
        register char *orb_bytes asm("r1") = *(char **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        register u32 trail_slot_count asm("r0") =
            *(u32 *)(orb_bytes + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames)) >> 1;
        register u32 trail_slot_end asm("r3") = trail_slot_count;
        asm volatile("" : "+r"(trail_slot_count));
        trail_slot_end += 0x11;

        if (sprite_index < trail_slot_end && *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[16])) == 0) {
            register u32 trail_scan_end asm("r4") = trail_slot_end;
            register char *sprite_slots asm("r3") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register u32 next_sprite_index_bits asm("r0") = sprite_index + 1;
                next_sprite_index_bits <<= 24;
                sprite_index = next_sprite_index_bits >> 24;
            } while (sprite_index < trail_scan_end &&
                *(s32 *)(sprite_slots + (sprite_index << 2)) == 0);
        }
        if (sprite_index == ((*(u32 *)(orb_bytes + BATTLE_SPRITE_OFFSET(user_data.projectile_trail.elapsed_frames)) >> 1) + 0x11)) {
            sprite_index = 1;
            if (*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0) {
                register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                do {
                    register u32 next_sprite_index_bits asm("r0") = sprite_index + 1;
                    next_sprite_index_bits <<= 24;
                    sprite_index = next_sprite_index_bits >> 24;
                } while (sprite_index <= 5 &&
                    *(s32 *)(sprite_slots + (sprite_index << 2)) == 0);
            }
            if (sprite_index == 6) {
                DestroySpriteGroup(group_bytes);
            }
        }
        break;
    }
    }
}
