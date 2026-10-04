#include "m2c_prelude.h"
extern void UpdateBattleHorizontalProjectileSprite(void *) asm("func_080D2528");
#include "battle_animation.h"
#include "battle_display.h"

extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern void SpawnBattleInwardChargeParticle(u8 *, s32) asm("func_080DCDC4");
extern u8 *CreateBattleAnimationSprite(u8 *, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2450");
extern u8 *CreateBattleAngledProjectileSprite(u8 *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
extern void DestroySprite(u8 *) asm("func_08094554");
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");
extern s32 DivideSigned32(s32, s32) asm("func_080ECD98");
extern s32 ModuloUnsigned32(s32, s32) asm("func_080ECF78");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void DestroySpriteGroup(u8 *) asm("func_08095114");

extern s32 gRandomNumberCallback asm("D_03000010");
extern s32 gBattleZoidScrollX asm("D_02034034");
extern struct SpriteBackgroundScrollOffsets gSpriteBackgroundScroll[] asm("D_03000054");

void UpdateBattleChargedCyanOrbProjectileEffect(u8 *group_bytes) asm("func_080DF84C");

void UpdateBattleChargedCyanOrbProjectileEffect(u8 *group_bytes)
{
    u32 particle_or_sprite_slot_index;
    u32 *charge_or_trail_updates_slot;
    u8 **sprite_slots;
    s32 **horizontal_projectile_slot;
    u32 *effect_state_slot;

    switch (*(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state))) {
    case BATTLE_CHARGED_CYAN_ORB_PROJECTILE_CHARGE:
    {
        u32 charge_updates_or_parity;

        charge_updates_or_parity = *(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.progress.charge_updates));
        charge_or_trail_updates_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.progress.charge_updates));
        if (charge_updates_or_parity == 0) {
            PlayBattleAnimationSound(0);
        }
        charge_updates_or_parity = *charge_or_trail_updates_slot;
        if (charge_updates_or_parity <= BATTLE_CHARGED_CYAN_ORB_PROJECTILE_ORB_CREATE_UPDATE || *(u16 *)(*(u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) + BATTLE_SPRITE_OFFSET(animation_step)) <= BATTLE_CHARGED_CYAN_ORB_PROJECTILE_LAST_CHARGE_EMISSION_STEP) {
            charge_updates_or_parity &= 1;
            if (charge_updates_or_parity == 0) {
                SpawnBattleInwardChargeParticle(group_bytes, 1);
            }
        }
        if (*charge_or_trail_updates_slot == BATTLE_CHARGED_CYAN_ORB_PROJECTILE_ORB_CREATE_UPDATE) {
            *(u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(group_bytes, 0, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
                                                 *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), 0, 0);
        } else if (*charge_or_trail_updates_slot > BATTLE_CHARGED_CYAN_ORB_PROJECTILE_ORB_CREATE_UPDATE && *(u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            particle_or_sprite_slot_index = 1;
            effect_state_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
            group_bytes += BATTLE_ANIMATION_OFFSET(sprites[0]);
            sprite_slots = (u8 **)group_bytes;
            do {
                u8 *primary_sprite = sprite_slots[particle_or_sprite_slot_index];
                if (primary_sprite != 0) {
                    DestroySprite(primary_sprite);
                }
                particle_or_sprite_slot_index = (u8)(particle_or_sprite_slot_index + 1);
            } while (particle_or_sprite_slot_index < BATTLE_ANIMATION_GROUP_SPRITE_COUNT);
            (*effect_state_slot)++;
        }
        (*charge_or_trail_updates_slot)++;
        break;
    }
    case BATTLE_CHARGED_CYAN_ORB_PROJECTILE_CREATE_FAN_AND_PROJECTILE:
    {
        u32 *saved_scroll_x_slot;
        u32 *saved_scroll_y_slot;
        s32 *random_callback_slot;
        s32 random_or_origin_x;
        s32 particle_angle;
        s32 particle_speed_fixed8;
        u8 *effect_sprite;

        particle_or_sprite_slot_index = 0;
        effect_state_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        charge_or_trail_updates_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.progress.trail_updates));
        sprite_slots = (u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
        horizontal_projectile_slot = (s32 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[31]));
        saved_scroll_x_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.saved_scroll_x_fixed8));
        saved_scroll_y_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.saved_scroll_y_fixed8));
        random_callback_slot = &gRandomNumberCallback;
        do {
            random_or_origin_x = CallFunctionR0(*random_callback_slot);
            particle_angle = DivideSigned32(particle_or_sprite_slot_index << 5, 7);
            particle_angle += 108;
            particle_angle += (u32)(random_or_origin_x * 9) >> 15;
            particle_angle <<= 24;
            particle_angle = (u32)particle_angle >> 24;
            particle_speed_fixed8 = ((u32)(CallFunctionR0(*random_callback_slot) * 513) >> 15) + 0x200;
            sprite_slots[particle_or_sprite_slot_index] = CreateBattleAngledProjectileSprite(group_bytes, 2, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
                                        *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), particle_angle, particle_speed_fixed8, 0);
            particle_or_sprite_slot_index = (u8)(particle_or_sprite_slot_index + 1);
        } while (particle_or_sprite_slot_index < BATTLE_CHARGED_CYAN_ORB_PROJECTILE_FAN_PARTICLE_COUNT);
        particle_or_sprite_slot_index = 0;
        do {
            particle_speed_fixed8 = ((u32)(CallFunctionR0(gRandomNumberCallback) * 257) >> 15) + 0x300;
            *(u8 **)((u8 *)sprite_slots + ((particle_or_sprite_slot_index + BATTLE_CHARGED_CYAN_ORB_PROJECTILE_FORWARD_FIRST_SLOT) << 2)) =
                CreateBattleAngledProjectileSprite(group_bytes, 3, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)),
                              *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)), BATTLE_SPRITE_SEMITRANSPARENT,
                              (particle_or_sprite_slot_index << 2) - 10, particle_speed_fixed8, 0);
            particle_or_sprite_slot_index = (u8)(particle_or_sprite_slot_index + 1);
        } while (particle_or_sprite_slot_index < BATTLE_CHARGED_CYAN_ORB_PROJECTILE_FORWARD_PARTICLE_COUNT);
        effect_sprite = CreateBattleAnimationSprite(group_bytes, 4, 0, *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)), *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)),
                            (BATTLE_SPRITE_LOOP_ANIMATION | BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_SECONDARY_SORT_PRIORITY_1), (s32)UpdateBattleHorizontalProjectileSprite, 0);
        *horizontal_projectile_slot = (s32 *)effect_sprite;
        *(s32 *)(effect_sprite + BATTLE_SPRITE_OFFSET(user_data.horizontal_projectile.speed)) = BATTLE_CHARGED_CYAN_ORB_PROJECTILE_SPEED_PIXELS;
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_UNIT_SHAKE_AND_RECOIL_SCROLL, 0);
        *charge_or_trail_updates_slot = 0;
        *saved_scroll_x_slot = gBattleZoidScrollX;
        *saved_scroll_y_slot = gSpriteBackgroundScroll[0].y_fixed8;
        PlayBattleAnimationSound(1);
        (*effect_state_slot)++;
        break;
    }
    case BATTLE_CHARGED_CYAN_ORB_PROJECTILE_EMIT_TRAIL:
    {
        u32 masked;

        masked = *(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.progress.trail_updates));
        masked &= 3;
        asm volatile("" :: "r"(masked));
        asm volatile("" :: "r"(masked));
        asm volatile("" :: "r"(masked));
        charge_or_trail_updates_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.progress.trail_updates));
        if (masked == 0) {
            s32 random_or_origin_x;
            s32 dy;
            s32 y;
            s32 dx;
            s32 x;
            s32 y_jitter;
            u8 *effect_sprite;

            random_or_origin_x = CallFunctionR0(gRandomNumberCallback);
            {
                s32 ybase = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
                s32 *saved_scroll_y_view = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.saved_scroll_y_fixed8));
                struct SpriteBackgroundScrollOffsets *background_scroll = gSpriteBackgroundScroll;

                dy = *saved_scroll_y_view;
                dy -= background_scroll[0].y_fixed8;
                if (dy < 0) {
                    dy += 255;
                }
                y = dy >> 8;
                y = ybase + ({ s32 restored_y_offset = y; restored_y_offset; });
            }
            y_jitter = ModuloUnsigned32(*charge_or_trail_updates_slot, 6);
            y_jitter += (u32)random_or_origin_x >> 14;
            y_jitter += 0xFFFE;
            y += y_jitter;
            y <<= 16;
            y >>= 16;
            random_or_origin_x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
            {
                s32 *saved_scroll_x_view = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.charged_cyan_orb_projectile.saved_scroll_x_fixed8));
                s32 *zoid_scroll_x_view = &gBattleZoidScrollX;

                dx = *saved_scroll_x_view;
                {
                    register s32 clobbered_r3 asm("r3");
                    asm volatile("" : "=r"(clobbered_r3));
                    asm volatile("" :: "r"(clobbered_r3));
                }
                dx -= *zoid_scroll_x_view;
                if (dx < 0) {
                    dx += 255;
                }
                x = dx >> 8;
                x = random_or_origin_x + ({ s32 restored_x_offset = x; restored_x_offset; });
            }
            x <<= 16;
            x >>= 16;
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0, x, y, BATTLE_SPRITE_SEMITRANSPARENT, 128, 0x180, masked);
            {
                u32 trail_slot_offset = *charge_or_trail_updates_slot >> 2;
                u8 *sprite_slots_base;
                trail_slot_offset += BATTLE_CHARGED_CYAN_ORB_PROJECTILE_TRAIL_FIRST_SLOT;
                trail_slot_offset <<= 2;
                sprite_slots_base = (u8 *)group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
                *(u8 **)(sprite_slots_base + trail_slot_offset) = effect_sprite;
            }
        }
        (*charge_or_trail_updates_slot)++;
        if (*charge_or_trail_updates_slot == BATTLE_CHARGED_CYAN_ORB_PROJECTILE_TRAIL_UPDATES) {
            (*(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state)))++;
        }
        break;
    }
    case BATTLE_CHARGED_CYAN_ORB_PROJECTILE_WAIT_FOR_SPRITES:
        particle_or_sprite_slot_index = 0;
        if (*(u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            u8 **sprite_slots_view = (u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            for (;;) {
                particle_or_sprite_slot_index = (u8)(particle_or_sprite_slot_index + 1);
                if (particle_or_sprite_slot_index >= BATTLE_ANIMATION_GROUP_SPRITE_COUNT) {
                    break;
                }
                if (*(u8 **)((u8 *)sprite_slots_view + (particle_or_sprite_slot_index << 2)) != 0) {
                    break;
                }
            }
        }
        if (particle_or_sprite_slot_index == BATTLE_ANIMATION_GROUP_SPRITE_COUNT) {
            DestroySpriteGroup(group_bytes);
        }
        break;
    }
}
