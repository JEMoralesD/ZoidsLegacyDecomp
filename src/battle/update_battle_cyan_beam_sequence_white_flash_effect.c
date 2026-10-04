#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"

void DestroySpriteGroup(void *) asm("func_08095114");
void QueueCopy(s32, s32, s32) asm("func_08095208");
void *CreateBattleAnimationSprite(void *, s32, s32, s32, s32, s32, void *, void *) asm("func_080D2450");
void *CreateBattleAngledProjectileSprite(void *, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_080D2660");
void PlayBattleAnimationSound(s32) asm("func_080D2790");
void BiosCpuFastSet(void *, void *, u32) asm("func_080ECD28");
void BiosCpuSet(void *, void *, u32) asm("func_080ECD2C");

void UpdateBattleCyanBeamSequenceWhiteFlashEffect(struct BattleAnimationGroup *group) asm("func_080DE5D8");

void UpdateBattleCyanBeamSequenceWhiteFlashEffect(struct BattleAnimationGroup *group)
{
    u8 *group_bytes = group;
    u16 local_colors[2];
    u32 phase = *(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));

    switch (phase) {
    case BATTLE_CYAN_BEAM_SEQUENCE_CREATE_RISING_ORB:
        {
            s32 x = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
            s32 y = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAngledProjectileSprite(
                group_bytes, BATTLE_CYAN_BEAM_SEQUENCE_ORB_RESOURCE, 0, x, y, BATTLE_SPRITE_SEMITRANSPARENT, 0xC0, 0x49, 0);
        }
        PlayBattleAnimationSound(0);
        goto increment_state;

    case BATTLE_CYAN_BEAM_SEQUENCE_CREATE_FLASH_AND_HORIZONTAL_BEAM:
        {
            register void *zero asm("r5") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]));
            register u32 x_offset asm("r2");
            register s32 x asm("r3");
            register s32 y asm("r0");
            if (zero != 0) {
                return;
            }
            x_offset = 4;
            x = *(s16 *)(group_bytes + x_offset);
            y = (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) - 0x18);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) = CreateBattleAnimationSprite(
                group_bytes, BATTLE_CYAN_BEAM_SEQUENCE_FLASH_RESOURCE, 0, x, y, BATTLE_SPRITE_SEMITRANSPARENT, zero, zero);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(
                group_bytes, BATTLE_CYAN_BEAM_SEQUENCE_HORIZONTAL_BEAM_RESOURCE, 0, (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x)) + 0x80),
                (s16)(*(s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y)) - 0x18), BATTLE_SPRITE_SEMITRANSPARENT, zero, zero);
            PlayBattleAnimationSound(1);
            __asm__("" : "+r"(group_bytes));
            goto increment_state;
        }

    case BATTLE_CYAN_BEAM_SEQUENCE_WAIT_FOR_HORIZONTAL_BEAM:
        {
            register void *zero asm("r4") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1]));
            register u32 *sequence_updates_slot asm("r5");
            if (zero != 0) {
                return;
            }
            sequence_updates_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sequence.sequence_updates));
            ++*sequence_updates_slot;
            if (*sequence_updates_slot != BATTLE_CYAN_BEAM_SEQUENCE_DELAY_UPDATES) {
                return;
            }
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = CreateBattleAnimationSprite(
                group_bytes, BATTLE_CYAN_BEAM_SEQUENCE_DIAGONAL_BEAM_RESOURCE, 0, 0x70, -0x20, BATTLE_SPRITE_SEMITRANSPARENT, zero, zero);
            *sequence_updates_slot = (u32)zero;
            PlayBattleAnimationSound(1);
            __asm__("" : "+r"(group_bytes));
            goto increment_state;
        }

    case BATTLE_CYAN_BEAM_SEQUENCE_WAIT_FOR_DIAGONAL_BEAM:
        {
            register void *zero asm("r2") = *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1]));
            register u32 *sequence_updates_slot asm("r1");
            register s32 x asm("r3");
            register s32 y asm("r0");
            if (zero != 0) {
                return;
            }
            sequence_updates_slot = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sequence.sequence_updates));
            ++*sequence_updates_slot;
            if (*sequence_updates_slot != BATTLE_CYAN_BEAM_SEQUENCE_DELAY_UPDATES) {
                return;
            }
            x = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(x));
            y = *(s16 *)(group_bytes + BATTLE_ANIMATION_OFFSET(y));
            *(volatile u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) = (u32)CreateBattleAnimationSprite(
                group_bytes, BATTLE_CYAN_BEAM_SEQUENCE_VERTICAL_BEAM_RESOURCE, 0, x, y, BATTLE_SPRITE_SEMITRANSPARENT, zero, zero);
            PlayBattleAnimationSound(1);
            goto increment_state;
        }

    case BATTLE_CYAN_BEAM_SEQUENCE_WAIT_FOR_VERTICAL_BEAM_STEP:
        if (*(u16 *)(*(u8 **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) + BATTLE_SPRITE_OFFSET(animation_step)) != BATTLE_CYAN_BEAM_SEQUENCE_FLASH_ANIMATION_STEP) {
            return;
        }
        {
            register void *background_palette_memory asm("sl");
            register void *sprite_palette_memory asm("r9");
            register void *background_palette_buffer asm("r5");
            register void *sprite_palette_buffer asm("r4");
            register u32 white_color asm("r6");
            register u32 background_fill_control asm("r8");
            register u16 *packed_color_or_white_fill asm("r0");
            register u32 background_palette_address asm("r2");
            register void *sprite_palette_address asm("r6");
            register u32 white_color_init asm("r1");
            register u32 background_fill_control_init asm("r2");

            background_palette_address = 0x05000000;
            background_palette_memory = (void *)background_palette_address;
            background_palette_buffer = (void *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors);
            BiosCpuFastSet(background_palette_memory, background_palette_buffer, 0x20);
            sprite_palette_address = (void *)0x05000200;
            __asm__("" : "+r"(sprite_palette_address));
            sprite_palette_memory = sprite_palette_address;
            sprite_palette_buffer = (void *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors);
            BiosCpuFastSet(sprite_palette_memory, sprite_palette_buffer, 0x18);
            packed_color_or_white_fill = &local_colors[0];
            white_color_init = 0x7FFF;
            __asm__("" : "+r"(white_color_init));
            white_color = white_color_init;
            *packed_color_or_white_fill = white_color;
            background_palette_buffer += 0x80;
            background_fill_control_init = 0x01000040;
            background_fill_control = background_fill_control_init;
            BiosCpuSet(packed_color_or_white_fill, background_palette_buffer, background_fill_control_init);
            packed_color_or_white_fill = (u16 *)((u8 *)local_colors + 2);
            *packed_color_or_white_fill = white_color;
            sprite_palette_buffer += 0x60;
            /* Native fills 64 sprite colors. Restoration updates only the 48 saved colors. */
            BiosCpuSet(packed_color_or_white_fill, sprite_palette_buffer, background_fill_control);
            QueueCopy(background_palette_buffer, background_palette_memory, 0x80);
            QueueCopy(sprite_palette_buffer, sprite_palette_memory, 0x60);
        }
        {
            register u32 x_offset asm("r6") = 4;
            register s32 x asm("r3") = *(s16 *)(group_bytes + x_offset);
            register u32 y_offset asm("r1") = 8;
            register s32 y asm("r0") = *(s16 *)(group_bytes + y_offset);
            *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[2])) = CreateBattleAnimationSprite(
                group_bytes, BATTLE_CYAN_BEAM_SEQUENCE_RING_RESOURCE, 0, x, y, BATTLE_SPRITE_SEMITRANSPARENT, 0, 0);
            *(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sequence.sequence_updates)) = 0;
        }
        PlayBattleAnimationSound(2);
        goto increment_state;

    case BATTLE_CYAN_BEAM_SEQUENCE_RESTORE_PALETTES:
        {
            register u32 palette_color_index asm("r9");
            register u32 *sequence_updates_slot asm("sl");
            u32 color_channel_mask;
            register u16 *palette_input_or_output asm("ip");

            {
                register u32 zero asm("r2") = 0;
                __asm__("" : "+r"(zero));
                palette_color_index = zero;
            }
            {
                register u32 restore_updates_offset_or_address asm("r6") = BATTLE_ANIMATION_OFFSET(effect.cyan_beam_sequence.sequence_updates);
                __asm__("" : "+r"(restore_updates_offset_or_address));
                restore_updates_offset_or_address += (u32)group_bytes;
                sequence_updates_slot = (u32 *)restore_updates_offset_or_address;
            }
            {
                register u32 channel_mask_init asm("r0") = 0x1F;
                __asm__("" : "+r"(channel_mask_init));
                color_channel_mask = channel_mask_init;
            }
            {
                register u16 *palette_input_or_output_init asm("r1") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors);
                __asm__("" : "+r"(palette_input_or_output_init));
                palette_input_or_output = palette_input_or_output_init;
            }
restore_background_palette:
            {
                register u32 offset asm("r3");
                register u32 packed_color_or_white_fill asm("r0");
                register u32 red asm("r4");
                register u32 green asm("r6");
                register u32 blue asm("r5");
                register u32 restore_blend_progress asm("r2");
                register u32 color_blend_work asm("r0");
                register u32 green_blend_work asm("r1");
                register u32 palette_color_index_view asm("r2");

                palette_color_index_view = palette_color_index;
                offset = palette_color_index_view << 1;
                packed_color_or_white_fill = *(u16 *)(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors) + offset);
                red = color_channel_mask;
                red &= packed_color_or_white_fill;
                packed_color_or_white_fill <<= 16;
                green = packed_color_or_white_fill >> 21;
                green &= color_channel_mask;
                blue = packed_color_or_white_fill >> 26;
                blue &= color_channel_mask;
                color_blend_work = color_channel_mask;
                color_blend_work -= red;
                restore_blend_progress = *sequence_updates_slot;
                restore_blend_progress = color_channel_mask - restore_blend_progress;
                color_blend_work *= restore_blend_progress;
                color_blend_work >>= 5;
                color_blend_work = red + color_blend_work;
                color_blend_work <<= 24;
                red = color_blend_work >> 24;
                color_blend_work = color_channel_mask;
                color_blend_work -= green;
                __asm__ volatile("" : "+r"(color_blend_work));
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
                register u32 zero asm("r0") = 0;
                register u16 *palette_input_or_output_init asm("r1") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_sprite_colors);
                register u32 sprite_channel_mask asm("r2") = 0x1F;
                palette_color_index = zero;
                palette_input_or_output = palette_input_or_output_init;
                color_channel_mask = sprite_channel_mask;
            }
restore_sprite_palette:
            {
                register u32 offset asm("r3");
                register u32 packed_color_or_white_fill asm("r0");
                register u32 red asm("r4");
                register u32 green asm("r6");
                register u32 blue asm("r5");
                register u32 restore_blend_progress asm("r2");
                register u32 color_blend_work asm("r0");
                register u32 green_blend_work asm("r1");
                register u32 palette_color_index_view asm("r6");
                register u16 *original_sprite_palette_view asm("r1");

                palette_color_index_view = palette_color_index;
                offset = palette_color_index_view << 1;
                original_sprite_palette_view = palette_input_or_output;
                packed_color_or_white_fill = *(u16 *)(offset + (u32)original_sprite_palette_view);
                red = color_channel_mask;
                red &= packed_color_or_white_fill;
                packed_color_or_white_fill <<= 16;
                green = packed_color_or_white_fill >> 21;
                green &= color_channel_mask;
                blue = packed_color_or_white_fill >> 26;
                blue &= color_channel_mask;
                color_blend_work = color_channel_mask;
                color_blend_work -= red;
                restore_blend_progress = *sequence_updates_slot;
                restore_blend_progress = color_channel_mask - restore_blend_progress;
                color_blend_work *= restore_blend_progress;
                color_blend_work >>= 5;
                color_blend_work = red + color_blend_work;
                color_blend_work <<= 24;
                red = color_blend_work >> 24;
                color_blend_work = color_channel_mask;
                color_blend_work -= green;
                __asm__ volatile("" : "+r"(color_blend_work));
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

            {
                register s32 background_palette_destination asm("r1") = 0x05000000;
                QueueCopy(BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors), background_palette_destination, 0x80);
            }
            QueueCopy((void *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_sprite_colors), (void *)0x05000200, 0x60);
            {
                register u32 *restore_updates_slot asm("r6") = sequence_updates_slot;
                register u32 next_restore_update asm("r0") = *restore_updates_slot + 1;
                *restore_updates_slot = next_restore_update;
                if (next_restore_update != BATTLE_WHITE_FLASH_RESTORE_UPDATES) {
                    return;
                }
            }
        }

increment_state:
        ++*(u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        return;

    case BATTLE_CYAN_BEAM_SEQUENCE_WAIT_FOR_SPRITES:
        if (*(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0
            && *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[1])) == 0
            && *(void **)(group_bytes + BATTLE_ANIMATION_OFFSET(sprites[2])) == 0) {
            DestroySpriteGroup(group_bytes);
        }
        return;
    }
    return;
}
