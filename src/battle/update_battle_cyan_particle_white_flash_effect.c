#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void QueueCopy(s32, s32, s32) asm("func_08095208");
extern s32 IsScreenTransitionComplete(void) asm("func_0809669C");
extern void CreateBattleAngledProjectileSprite(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
extern s32 CallFunctionR0(s32) asm("func_080ECD5C");

void UpdateBattleCyanParticleWhiteFlashEffect(struct BattleAnimationGroup *group) asm("func_080DE9AC");

void UpdateBattleCyanParticleWhiteFlashEffect(struct BattleAnimationGroup *group) {
    u8 *group_bytes;
    register s32 *random_callback_slot asm("r6");
    register s32 particle_x asm("r5");
    register s32 particle_y asm("r4");
    register u32 *restore_update_slot asm("r8");
    register u32 palette_color_index asm("ip");
    u32 color_channel_mask;
    register u16 *blended_background_palette asm("sl");
    register u16 *palette_input_or_output asm("r9");

    group_bytes = group;
    random_callback_slot = (s32 *)0x03000010;
    {
        register s32 random asm("r0");
        register u32 particle_random_work asm("r1");

        random = CallFunctionR0(*random_callback_slot);
        particle_x = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
        particle_random_work = random << 6;
        particle_random_work += random;
        particle_random_work >>= 15;
        particle_x += particle_random_work;
        particle_x -= 0x20;
        particle_x <<= 16;
        particle_x >>= 16;

        random = CallFunctionR0(*random_callback_slot);
        particle_y = *(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
        particle_random_work = random << 6;
        particle_random_work += random;
        particle_random_work >>= 15;
        particle_y += particle_random_work;
        particle_y -= 0x20;
        particle_y <<= 16;
        particle_y >>= 16;

        random = CallFunctionR0(*random_callback_slot);
        particle_random_work = random << 8;
        particle_random_work += random;
        particle_random_work >>= 15;
        particle_random_work += 0x200;
        CreateBattleAngledProjectileSprite(group_bytes, 0, 0, particle_x, particle_y, 0, 0, particle_random_work, 0);
    }
    {
        register u32 *transition_or_restore_update_slot asm("r4");

        transition_or_restore_update_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        if (*transition_or_restore_update_slot == BATTLE_CYAN_PARTICLE_WHITE_FLASH_WAIT_FOR_TRANSITION) {
            if ((IsScreenTransitionComplete() << 24) != 0) {
                *transition_or_restore_update_slot = 1;
            }
            if (*transition_or_restore_update_slot == BATTLE_CYAN_PARTICLE_WHITE_FLASH_WAIT_FOR_TRANSITION) {
                goto done;
            }
        }
    }
    {
        register u32 *restore_update_slot_init asm("r0");
        register u32 restore_update asm("r1");

        restore_update_slot_init = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        restore_update = *restore_update_slot_init;
        restore_update_slot = restore_update_slot_init;
        if (restore_update > BATTLE_CYAN_PARTICLE_WHITE_FLASH_RESTORE_LAST) {
            goto destroy;
        }
    }

    palette_color_index = 0;
    {
        register u16 *blended_background_palette_init asm("r2") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors);
        blended_background_palette = blended_background_palette_init;
    }
    color_channel_mask = 0x1F;
    palette_input_or_output = blended_background_palette;
restore_background_palette:
    {
        register u32 offset asm("r3");
        register u32 color asm("r0");
        register u32 red asm("r4");
        register u32 green asm("r6");
        register u32 blue asm("r5");
        register u32 restore_blend_progress asm("r2");
        register u32 color_blend_work asm("r0");
        register u32 green_blend_work asm("r1");
        register u32 palette_color_index_view asm("r0");

        palette_color_index_view = palette_color_index;
        offset = palette_color_index_view << 1;
        color = *(u16 *)(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors) + offset);
        red = color_channel_mask;
        red &= color;
        color <<= 16;
        green = color >> 21;
        green &= color_channel_mask;
        blue = color >> 26;
        blue &= color_channel_mask;

        color_blend_work = color_channel_mask;
        color_blend_work -= red;
        restore_blend_progress = *restore_update_slot;
        restore_blend_progress = color_channel_mask - restore_blend_progress;
        color_blend_work *= restore_blend_progress;
        color_blend_work >>= 5;
        color_blend_work = red + color_blend_work;
        color_blend_work <<= 24;
        red = color_blend_work >> 24;

        color_blend_work = color_channel_mask;
        color_blend_work -= green;
        asm volatile("" : "+r"(color_blend_work));
        green_blend_work = color_blend_work;
        green_blend_work *= restore_blend_progress;
        green_blend_work >>= 5;
        green_blend_work = green + green_blend_work;
        green_blend_work <<= 24;

        color_blend_work = color_channel_mask;
        color_blend_work -= blue;
        color_blend_work *= restore_blend_progress;
        color_blend_work >>= 5;
        color_blend_work = blue + color_blend_work;
        color_blend_work <<= 24;

        offset += (u32)palette_input_or_output;
        green_blend_work >>= 19;
        red |= green_blend_work;
        color_blend_work >>= 14;
        red |= color_blend_work;
        *(u16 *)offset = red;
    }
    palette_color_index = (u8)(palette_color_index + 1);
    if (palette_color_index < BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT) {
        goto restore_background_palette;
    }

    {
        register u32 zero asm("r2") = 0;
        register u16 *palette_input_or_output_init asm("r0") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors);

        palette_color_index = zero;
        palette_input_or_output = palette_input_or_output_init;
    }
    color_channel_mask = 0x1F;
restore_sprite_palette:
    {
        register u32 offset asm("r3");
        register u32 color asm("r0");
        register u32 red asm("r4");
        register u32 green asm("r6");
        register u32 blue asm("r5");
        register u32 restore_blend_progress asm("r2");
        register u32 color_blend_work asm("r0");
        register u32 green_blend_work asm("r1");
        register u32 palette_color_index_view asm("r1");
        register u16 *base_view asm("r2");

        palette_color_index_view = palette_color_index;
        offset = palette_color_index_view << 1;
        base_view = palette_input_or_output;
        color = *(u16 *)(offset + (u32)base_view);
        red = color_channel_mask;
        red &= color;
        color <<= 16;
        green = color >> 21;
        green &= color_channel_mask;
        blue = color >> 26;
        blue &= color_channel_mask;

        color_blend_work = color_channel_mask;
        color_blend_work -= red;
        restore_blend_progress = *restore_update_slot;
        restore_blend_progress = color_channel_mask - restore_blend_progress;
        color_blend_work *= restore_blend_progress;
        color_blend_work >>= 5;
        color_blend_work = red + color_blend_work;
        color_blend_work <<= 24;
        red = color_blend_work >> 24;

        color_blend_work = color_channel_mask;
        color_blend_work -= green;
        asm volatile("" : "+r"(color_blend_work));
        green_blend_work = color_blend_work;
        green_blend_work *= restore_blend_progress;
        green_blend_work >>= 5;
        green_blend_work = green + green_blend_work;
        green_blend_work <<= 24;

        color_blend_work = color_channel_mask;
        color_blend_work -= blue;
        color_blend_work *= restore_blend_progress;
        color_blend_work >>= 5;
        color_blend_work = blue + color_blend_work;
        color_blend_work <<= 24;

        {
            register u32 blended_sprite_palette_address asm("r2") = BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_sprite_colors);
            offset += blended_sprite_palette_address;
        }
        green_blend_work >>= 19;
        red |= green_blend_work;
        color_blend_work >>= 14;
        red |= color_blend_work;
        *(u16 *)offset = red;
    }
    palette_color_index = (u8)(palette_color_index + 1);
    if (palette_color_index < BATTLE_WHITE_FLASH_SPRITE_COLOR_COUNT) {
        goto restore_sprite_palette;
    }

    QueueCopy((s32)blended_background_palette, 0x05000000, 0x80);
    QueueCopy(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_sprite_colors), 0x05000200, 0x60);
    {
        register u32 *restore_update_slot_view asm("r1") = restore_update_slot;
        *restore_update_slot_view = *restore_update_slot_view + 1;
    }
    goto done;

destroy:
    DestroySpriteGroup(group_bytes);
done:
    return;
}
