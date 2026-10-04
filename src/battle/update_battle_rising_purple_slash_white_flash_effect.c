#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

M2C_UNK DestroySprite(void *) asm("func_08094554");
M2C_UNK DestroySpriteGroup(void *) asm("func_08095114");
M2C_UNK QueueCopy(M2C_UNK, s32, s32) asm("func_08095208");
M2C_UNK SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
M2C_UNK StartBattleBackgroundShake() asm("func_080D218C");
void *CreateBattleAnimationSprite(void *, s32, s32, s16, s32, s32, u32, u32) asm("func_080D2450");
M2C_UNK BiosCpuFastSet(s32, M2C_UNK, s32) asm("func_080ECD28");
u16 DivideSigned32(s32, s32) asm("func_080ECD98");

void UpdateBattleRisingPurpleSlashWhiteFlashEffect(struct BattleAnimationGroup *group) asm("func_080DC584");

void UpdateBattleRisingPurpleSlashWhiteFlashEffect(struct BattleAnimationGroup *group) {
    volatile s32 y_step;
    volatile s32 packed_x_step;
    register s32 *saved_effect_update_slot asm("r9");
    s32 x_step_numerator;
    register s32 half_update_divisor asm("r1");
    register s32 signed_x_step asm("r6");
    u32 x_step_bits;
    register u32 blend_update asm("r2");
    u32 palette_flash_update;
    register u32 effect_update asm("r6");
    void *phase;

    register void *group_or_palette_output asm("r8") = group;
    register u32 *effect_update_slot asm("r5") = (u32 *)((u8 *)group_or_palette_output + BATTLE_ANIMATION_OFFSET(state));

    effect_update = *effect_update_slot;
    if (effect_update > BATTLE_SLASH_WHITE_FLASH_BRIGHTEN_LAST) {
        goto restore_palette_phase;
    }
    if (effect_update == BATTLE_SLASH_WHITE_FLASH_CREATE) {
        void *effect_sprite;
        register void *base_r2 asm("r2");
        register void *base_r3 asm("r3");

        effect_sprite = CreateBattleAnimationSprite(group_or_palette_output, 0, 0, BATTLE_ANIMATION_FIELD(group, s16, x), (s32) (s16) (BATTLE_ANIMATION_FIELD(group, s32, y) + 0x80), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_LOOP_ANIMATION), effect_update, effect_update);
        base_r2 = group_or_palette_output;
        BATTLE_ANIMATION_FIELD(base_r2, void *, sprites[0]) = effect_sprite;
        BATTLE_ANIMATION_FIELD(group_or_palette_output, s32, sprite_flags) = BATTLE_ANIMATION_HARDWARE_SPRITE_PRIORITY_3;
        effect_sprite = CreateBattleAnimationSprite(group_or_palette_output, 0, 0, (s16) (BATTLE_ANIMATION_FIELD(base_r2, s32, x) - 0x20), (s32) (s16) (BATTLE_ANIMATION_FIELD(base_r2, s32, y) + 0x80), (BATTLE_SPRITE_SEMITRANSPARENT | BATTLE_SPRITE_LOOP_ANIMATION), effect_update, effect_update);
        base_r3 = group_or_palette_output;
        BATTLE_ANIMATION_FIELD(base_r3, void *, sprites[1]) = effect_sprite;
        BiosCpuFastSet(0x05000000, BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors), BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT / 2);
        BiosCpuFastSet(0x05000200, BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors), BATTLE_WHITE_FLASH_SPRITE_COLOR_COUNT / 2);
        SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_QUICK_SCROLL_SURGE, 0);
    }
    blend_update = *effect_update_slot;
    if (blend_update <= 3U) {
        register void *base_r4 asm("r4") = group_or_palette_output;
        u32 group_mirrored = BATTLE_ANIMATION_FIELD(base_r4, u32, flags) & BATTLE_ANIMATION_GROUP_MIRRORED;
        signed_x_step = 0xFFF0;
        if (!group_mirrored) {
            signed_x_step = 0x10;
        }
        y_step = 0xFFE0;
        asm volatile("mov r9, r5" : "=r"(saved_effect_update_slot));
        packed_x_step = signed_x_step << 0x10;
    } else {
        register void *flags_base asm("r1") = group_or_palette_output;
        if (!(BATTLE_ANIMATION_FIELD(flags_base, s32, flags) & BATTLE_ANIMATION_GROUP_MIRRORED)) {
            half_update_divisor = (s32) (blend_update << 0x17) >> 0x18;
            x_step_numerator = 0x10;
        } else {
            half_update_divisor = (s32) (blend_update << 0x17) >> 0x18;
            x_step_numerator = -0x10;
        }
        {
            register u32 result_r6 asm("r6");
            DivideSigned32(x_step_numerator, half_update_divisor);
            asm volatile("lsl r0, r0, #16\n\tlsr r6, r0, #16"
                         : "=r"(result_r6) : : "r0");
            x_step_bits = result_r6;
        }
        {
            register u32 *late_state_ptr asm("r5") = (u32 *)((u8 *)group_or_palette_output + BATTLE_ANIMATION_OFFSET(state));
            register u32 late_state asm("r4") = *late_state_ptr;
            palette_flash_update = late_state;
        }
        half_update_divisor = (s32)(palette_flash_update << 0x17) >> 0x18;
        y_step = (s32)DivideSigned32(-0x20, half_update_divisor);
        *(s16 *)0x03000050 = (0x14 - palette_flash_update) | 0x1000;
        {
        register u32 palette_color_index asm("r7");
        register u32 channel_mask asm("ip");
        register u16 *palette_input_or_output asm("sl");

        palette_color_index = 0;
        asm volatile("mov r9, r5" : "=r"(saved_effect_update_slot));
        packed_x_step = x_step_bits << 0x10;
        {
            register u32 channel_mask_seed asm("r4") = 0x1F;
            asm volatile("" : "+r"(channel_mask_seed));
            channel_mask = channel_mask_seed;
        }
        {
            register u16 *base_seed asm("r6") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors);
            palette_input_or_output = base_seed;
        }
brighten_background_palette:
        {
            register u32 offset asm("r3");
            register u32 packed_color asm("r0");
            register u32 red asm("r4");
            register u32 green asm("r6");
            register u32 blue asm("r5");
            register u32 blend_progress asm("r2");
            register u32 channel_blend_work asm("r0");
            register u32 green_blend_work asm("r1");

            offset = palette_color_index << 1;
            packed_color = *(u16 *)(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors) + offset);
            red = channel_mask;
            red &= packed_color;
            packed_color <<= 16;
            green = packed_color >> 21;
            green &= channel_mask;
            blue = packed_color >> 26;
            blue &= channel_mask;
            channel_blend_work = channel_mask;
            channel_blend_work -= red;
            blend_progress = *saved_effect_update_slot - 3;
            channel_blend_work *= blend_progress;
            channel_blend_work >>= 4;
            channel_blend_work = red + channel_blend_work;
            channel_blend_work <<= 24;
            red = channel_blend_work >> 24;
            channel_blend_work = channel_mask;
            channel_blend_work -= green;
            asm volatile("" : "+r"(channel_blend_work));
            green_blend_work = channel_blend_work;
            green_blend_work *= blend_progress;
            green_blend_work >>= 4;
            green_blend_work = green + green_blend_work;
            green_blend_work <<= 24;
            channel_blend_work = channel_mask;
            channel_blend_work -= blue;
            channel_blend_work *= blend_progress;
            channel_blend_work >>= 4;
            channel_blend_work = blue + channel_blend_work;
            channel_blend_work <<= 24;
            offset += (u32)palette_input_or_output;
            green_blend_work >>= 19;
            red |= green_blend_work;
            channel_blend_work >>= 14;
            red |= channel_blend_work;
            *(u16 *)offset = red;
        }
        palette_color_index = (u8)(palette_color_index + 1);
        if (palette_color_index < BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT) {
            goto brighten_background_palette;
        }

        palette_color_index = 0;
        {
            register u16 *base_seed asm("r0") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors);
            register u32 channel_mask_seed asm("r1") = 0x1F;
            palette_input_or_output = base_seed;
            channel_mask = channel_mask_seed;
        }
brighten_sprite_palette:
        {
            register u32 offset asm("r3");
            register u32 packed_color asm("r0");
            register u32 red asm("r4");
            register u32 green asm("r6");
            register u32 blue asm("r5");
            register u32 blend_progress asm("r2");
            register u32 channel_blend_work asm("r0");
            register u32 green_blend_work asm("r1");

            offset = palette_color_index << 1;
            packed_color = *(u16 *)(offset + (u32)palette_input_or_output);
            red = channel_mask;
            red &= packed_color;
            packed_color <<= 16;
            green = packed_color >> 21;
            green &= channel_mask;
            blue = packed_color >> 26;
            blue &= channel_mask;
            channel_blend_work = channel_mask;
            channel_blend_work -= red;
            blend_progress = *saved_effect_update_slot - 3;
            channel_blend_work *= blend_progress;
            channel_blend_work >>= 4;
            channel_blend_work = red + channel_blend_work;
            channel_blend_work <<= 24;
            red = channel_blend_work >> 24;
            channel_blend_work = channel_mask;
            channel_blend_work -= green;
            asm volatile("" : "+r"(channel_blend_work));
            green_blend_work = channel_blend_work;
            green_blend_work *= blend_progress;
            green_blend_work >>= 4;
            green_blend_work = green + green_blend_work;
            green_blend_work <<= 24;
            channel_blend_work = channel_mask;
            channel_blend_work -= blue;
            channel_blend_work *= blend_progress;
            channel_blend_work >>= 4;
            channel_blend_work = blue + channel_blend_work;
            channel_blend_work <<= 24;
            {
                register u32 blended_sprite_palette_address asm("r2") = BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_sprite_colors);
                offset += blended_sprite_palette_address;
            }
            green_blend_work >>= 19;
            red |= green_blend_work;
            channel_blend_work >>= 14;
            red |= channel_blend_work;
            *(u16 *)offset = red;
        }
        palette_color_index = (u8)(palette_color_index + 1);
        if (palette_color_index < BATTLE_WHITE_FLASH_SPRITE_COLOR_COUNT) {
            goto brighten_sprite_palette;
        }
        }
        {
            register u32 background_palette_address asm("r1") = 0x05000000;
            asm volatile("" : "+r"(background_palette_address));
            QueueCopy(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors), background_palette_address, BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT * sizeof(u16));
        }
        QueueCopy(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_sprite_colors), 0x05000200, BATTLE_WHITE_FLASH_SPRITE_COLOR_COUNT * sizeof(u16));
    }
    {
        register void *first_sprite_group asm("r3") = group_or_palette_output;
        register void *first_slash_sprite asm("r1") = BATTLE_ANIMATION_FIELD(first_sprite_group, void *, sprites[0]);
        register u32 previous_first_slash_x asm("r0") = BATTLE_SPRITE_FIELD(first_slash_sprite, u16, x);
        register s32 packed_x_step_view asm("r4") = packed_x_step;
        register s32 x_step_pixels asm("r3") = packed_x_step_view >> 0x10;
        BATTLE_SPRITE_FIELD(first_slash_sprite, u16, x) = (u16)(x_step_pixels + previous_first_slash_x);
        {
            register void *base asm("r6") = group_or_palette_output;
            register void *slash_sprite asm("r2") = BATTLE_ANIMATION_FIELD(base, void *, sprites[0]);
            register u32 previous_coordinate asm("r0") = BATTLE_SPRITE_FIELD(slash_sprite, u16, y);
            register s32 y_step_view asm("r4") = y_step;
            register s32 y_step_pixels asm("r1") = (s16)y_step_view;
            BATTLE_SPRITE_FIELD(slash_sprite, u16, y) = (u16)(y_step_pixels + previous_coordinate);
            slash_sprite = *(void * volatile *)((u8 *)base + BATTLE_ANIMATION_OFFSET(sprites[1]));
            previous_coordinate = BATTLE_SPRITE_FIELD(slash_sprite, u16, x);
            x_step_pixels += previous_coordinate;
            BATTLE_SPRITE_FIELD(slash_sprite, u16, x) = (u16)x_step_pixels;
            slash_sprite = *(void * volatile *)((u8 *)base + BATTLE_ANIMATION_OFFSET(sprites[1]));
            previous_coordinate = BATTLE_SPRITE_FIELD(slash_sprite, u16, y);
            y_step_pixels += previous_coordinate;
            BATTLE_SPRITE_FIELD(slash_sprite, u16, y) = (u16)y_step_pixels;
        }
    }
    {
        register s32 *phase asm("r6") = saved_effect_update_slot;
        *phase += 1;
    }
    goto return_effect;

restore_palette_phase:
    {
    register u32 restore_update asm("r6");
    asm volatile("" : "=r"(restore_update));
    if (restore_update > BATTLE_SLASH_WHITE_FLASH_RESTORE_LAST) {
        goto destroy_effect;
    }
    if (restore_update == BATTLE_SLASH_WHITE_FLASH_RESTORE_FIRST) {
        register void *base_r1 asm("r1") = group_or_palette_output;
        register void *base_r2 asm("r2");
        DestroySprite(BATTLE_ANIMATION_FIELD(base_r1, void *, sprites[0]));
        base_r2 = group_or_palette_output;
        DestroySprite(BATTLE_ANIMATION_FIELD(base_r2, void *, sprites[1]));
        StartBattleBackgroundShake();
    }
    {
    u32 palette_color_index;
    register u32 *phase asm("r9");
    register u32 channel_mask asm("ip");
    register u16 *palette_input_or_output asm("sl");

    palette_color_index = 0;
    asm volatile("mov r9, r5" : "=r"(phase));
    {
        register u32 base_seed asm("r3") = BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors);
        asm volatile("" : "+r"(base_seed));
        base_seed -= 0x80;
        palette_input_or_output = (u16 *)base_seed;
    }
    {
        register u32 channel_mask_seed asm("r4") = 0x1F;
        asm volatile("" : "+r"(channel_mask_seed));
        channel_mask = channel_mask_seed;
    }
    {
        register u16 *base_seed asm("r6") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors);
        asm volatile("" : "+r"(base_seed));
        group_or_palette_output = base_seed;
    }
restore_background_palette:
    {
        register u32 offset asm("r3");
        register u32 packed_color asm("r0");
        register u32 red asm("r4");
        register u32 green asm("r6");
        register u32 blue asm("r5");
        register u32 blend_progress asm("r2");
        register u32 channel_blend_work asm("r0");
        register u32 green_blend_work asm("r1");
        register u32 channel_distance_to_white asm("r1");
        register u32 original_palette_address asm("r1");
        register u32 channel_mask_view asm("r2");

        offset = palette_color_index << 1;
        original_palette_address = (u32)palette_input_or_output;
        asm volatile("" : "+r"(original_palette_address));
        packed_color = offset + original_palette_address;
        packed_color = *(u16 *)packed_color;
        red = channel_mask;
        red &= packed_color;
        packed_color <<= 16;
        green = packed_color >> 21;
        channel_mask_view = channel_mask;
        asm volatile("" : "+r"(channel_mask_view));
        green &= channel_mask_view;
        blue = packed_color >> 26;
        blue &= channel_mask_view;
        channel_distance_to_white = channel_mask_view;
        channel_distance_to_white -= red;
        asm volatile("" : "+r"(channel_distance_to_white));
        blend_progress = (u32)phase;
        channel_blend_work = *(u32 *)blend_progress;
        blend_progress = BATTLE_SLASH_WHITE_FLASH_RESTORE_LAST - channel_blend_work;
        channel_blend_work = channel_distance_to_white;
        channel_blend_work *= blend_progress;
        channel_blend_work >>= 5;
        channel_blend_work = red + channel_blend_work;
        channel_blend_work <<= 24;
        red = channel_blend_work >> 24;
        channel_blend_work = channel_mask;
        channel_blend_work -= green;
        asm volatile("" : "+r"(channel_blend_work));
        green_blend_work = channel_blend_work;
        green_blend_work *= blend_progress;
        green_blend_work >>= 5;
        green_blend_work = green + green_blend_work;
        green_blend_work <<= 24;
        channel_blend_work = channel_mask;
        channel_blend_work -= blue;
        channel_blend_work *= blend_progress;
        channel_blend_work >>= 5;
        channel_blend_work = blue + channel_blend_work;
        channel_blend_work <<= 24;
        offset += (u32)group_or_palette_output;
        green_blend_work >>= 19;
        red |= green_blend_work;
        channel_blend_work >>= 14;
        red |= channel_blend_work;
        *(u16 *)offset = red;
    }
    palette_color_index = (u8)(palette_color_index + 1);
    if (palette_color_index < BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT) {
        goto restore_background_palette;
    }

    palette_color_index = 0;
    {
        register u16 *base_seed asm("r0") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors);
        register u32 channel_mask_seed asm("r1") = 0x1F;
        register u32 sprite_output_offset asm("r2") = 0x60;
        group_or_palette_output = base_seed;
        channel_mask = channel_mask_seed;
        sprite_output_offset += (u32)group_or_palette_output;
        palette_input_or_output = (u16 *)sprite_output_offset;
    }
restore_sprite_palette:
    {
        register u32 offset asm("r3");
        register u32 packed_color asm("r0");
        register u32 red asm("r4");
        register u32 green asm("r6");
        register u32 blue asm("r5");
        register u32 blend_progress asm("r2");
        register u32 channel_blend_work asm("r0");
        register u32 green_blend_work asm("r1");
        register u32 channel_distance_to_white asm("r1");

        offset = palette_color_index << 1;
        {
            register u32 original_palette_address asm("r4") = (u32)group_or_palette_output;
            register u32 address asm("r0");
            asm volatile("" : "+r"(original_palette_address));
            address = offset + original_palette_address;
            asm volatile("" : "+r"(address));
            packed_color = *(u16 *)address;
        }
        red = channel_mask;
        red &= packed_color;
        packed_color <<= 16;
        green = packed_color >> 21;
        green &= channel_mask;
        blue = packed_color >> 26;
        blue &= channel_mask;
        channel_distance_to_white = channel_mask;
        channel_distance_to_white -= red;
        asm volatile("" : "+r"(channel_distance_to_white));
        blend_progress = (u32)phase;
        channel_blend_work = *(u32 *)blend_progress;
        blend_progress = BATTLE_SLASH_WHITE_FLASH_RESTORE_LAST - channel_blend_work;
        channel_blend_work = channel_distance_to_white;
        channel_blend_work *= blend_progress;
        channel_blend_work >>= 5;
        channel_blend_work = red + channel_blend_work;
        channel_blend_work <<= 24;
        red = channel_blend_work >> 24;
        channel_blend_work = channel_mask;
        channel_blend_work -= green;
        asm volatile("" : "+r"(channel_blend_work));
        green_blend_work = channel_blend_work;
        green_blend_work *= blend_progress;
        green_blend_work >>= 5;
        green_blend_work = green + green_blend_work;
        green_blend_work <<= 24;
        channel_blend_work = channel_mask;
        channel_blend_work -= blue;
        channel_blend_work *= blend_progress;
        channel_blend_work >>= 5;
        channel_blend_work = blue + channel_blend_work;
        channel_blend_work <<= 24;
        offset += (u32)palette_input_or_output;
        green_blend_work >>= 19;
        red |= green_blend_work;
        channel_blend_work >>= 14;
        red |= channel_blend_work;
        *(u16 *)offset = red;
    }
    palette_color_index = (u8)(palette_color_index + 1);
    if (palette_color_index < BATTLE_WHITE_FLASH_SPRITE_COLOR_COUNT) {
        goto restore_sprite_palette;
    }
    {
        register u32 background_palette_address asm("r1") = 0x05000000;
        asm volatile("" : "+r"(background_palette_address));
        QueueCopy(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors), background_palette_address, BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT * sizeof(u16));
    }
    QueueCopy(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_sprite_colors), 0x05000200, BATTLE_WHITE_FLASH_SPRITE_COLOR_COUNT * sizeof(u16));
    {
        register u32 *phase_update asm("r1") = phase;
        *phase_update += 1;
    }
    }
    goto return_effect;
    }
destroy_effect:
    DestroySpriteGroup(group_or_palette_output);
return_effect:
    asm volatile(".Lsub_080DC584_return:");
}
