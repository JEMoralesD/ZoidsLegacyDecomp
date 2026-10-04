#include "m2c_prelude.h"
#include "battle_animation.h"
#include "battle_display.h"
struct BattleLightningExplosionOffset { u16 x; u16 y; };
#define gBattleLightningExplosionOffsets ((struct BattleLightningExplosionOffset *)0x087A2CF4)

extern void DestroySpriteGroup(void *) asm("func_08095114");
extern void QueueCopy(s32, s32, s32) asm("func_08095208");
extern void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
extern void *CreateBattleAnimationSprite() asm("func_080D2450");
extern void *CreateBattleAngledProjectileSprite() asm("func_080D2660");
extern void PlayBattleAnimationSound(s32) asm("func_080D2790");
extern void BiosCpuFastSet(s32, s32, s32) asm("func_080ECD28");
extern void BiosCpuSet(s16 *, s32, s32) asm("func_080ECD2C");

void UpdateBattleLightningExplosionWhiteFlashEffect(struct BattleAnimationGroup *group) asm("func_080DF320");

void UpdateBattleLightningExplosionWhiteFlashEffect(struct BattleAnimationGroup *group) {
    struct BattleLightningExplosionWhiteFlashStack {
        s16 white_color;
        u16 alignment_padding;
        s32 palette_byte_offset;
        s32 red_distance_to_white;
    } stack;
    register char *group_bytes asm("r8") = group;
    register s32 *effect_state_slot asm("r7");
    u32 phase;

    {
        register s32 *palette_restore_updates_slot asm("r4") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_explosion_white_flash.palette_restore_updates));
        u32 palette_restore_update = *palette_restore_updates_slot;

        if (palette_restore_update != 0) {
            if (palette_restore_update <= 0x1FU) {
                register s32 palette_color_index asm("r6") = 0;
                register s32 *saved_palette_restore_updates_slot asm("r9") = palette_restore_updates_slot;
                register u16 *blended_background_palette_init asm("r12") = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(blended_background_colors);
                register u32 color_channel_max = 0x1F;
                register u16 *blended_background_palette asm("r10") = blended_background_palette_init;

                do {
                    register s32 palette_byte_offset asm("r1") = palette_color_index * 2;
                    register u16 *original_background_palette asm("r2");
                    register u16 *original_color_address asm("r0");
                    register u32 packed_color asm("r1");
                    register u32 red asm("r2");
                    register u32 green asm("r3");
                    register s32 red_distance_or_restore_progress asm("r4");
                    register s32 restore_update_or_red_distance asm("r5");
                    register u32 color_blend_work asm("r0");
                    register u16 *blended_color_slot asm("r5");

                    stack.palette_byte_offset = palette_byte_offset;
                    original_background_palette = (u16 *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors);
                    asm volatile(
                        "add %0, %2, %3\n\t"
                        "ldrh %1, [%0]"
                        : "=r"(original_color_address), "=r"(packed_color)
                        : "r"(palette_byte_offset), "r"(original_background_palette));
                    red = color_channel_max;
                    red &= packed_color;
                    packed_color <<= 16;
                    green = packed_color >> 21;
                    green &= color_channel_max;
                    packed_color >>= 26;
                    packed_color &= color_channel_max;
                    red_distance_or_restore_progress = color_channel_max - red;
                    stack.red_distance_to_white = red_distance_or_restore_progress;
                    asm volatile(
                        "mov %0, %1\n\t"
                        "ldr %0, [%0]"
                        : "=r"(restore_update_or_red_distance)
                        : "r"(saved_palette_restore_updates_slot));
                    red_distance_or_restore_progress = color_channel_max - restore_update_or_red_distance;
                    restore_update_or_red_distance = stack.red_distance_to_white;
                    asm volatile("" : "+r"(restore_update_or_red_distance));
                    color_blend_work = restore_update_or_red_distance;
                    color_blend_work *= red_distance_or_restore_progress;
                    color_blend_work >>= 5;
                    red += color_blend_work;
                    red = (u8)red;
                    color_blend_work = color_channel_max - green;
                    color_blend_work *= red_distance_or_restore_progress;
                    color_blend_work >>= 5;
                    green += color_blend_work;
                    green <<= 24;
                    color_blend_work = color_channel_max - packed_color;
                    color_blend_work *= red_distance_or_restore_progress;
                    color_blend_work >>= 5;
                    packed_color += color_blend_work;
                    packed_color <<= 24;
                    blended_color_slot = (u16 *)((char *)blended_background_palette + stack.palette_byte_offset);
                    green >>= 19;
                    red |= green;
                    packed_color >>= 14;
                    red |= packed_color;
                    *blended_color_slot = red;
                    color_blend_work = palette_color_index + 1;
                    palette_color_index = (u8)color_blend_work;
                } while ((u32)palette_color_index < BATTLE_WHITE_FLASH_BACKGROUND_COLOR_COUNT);
                QueueCopy((s32)blended_background_palette_init, 0x05000000, 0x80);
                {
                    register s32 *palette_restore_updates_view asm("r1") = saved_palette_restore_updates_slot;
                    register s32 next_index_bits asm("r0") = *palette_restore_updates_view;
                    next_index_bits += 1;
                    *palette_restore_updates_view = next_index_bits;
                }
            } else {
                *palette_restore_updates_slot = 0;
            }
        }
    }
    {
        register s32 *state_seed asm("r0") = (s32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(state));
        phase = *state_seed;
        asm volatile("" : "+r"(state_seed));
        effect_state_slot = state_seed;
    }
    switch (phase) {                            /* irregular */
    case BATTLE_LIGHTNING_EXPLOSION_CREATE_FIRST_LIGHTNING:
        BATTLE_ANIMATION_FIELD(group_bytes, void *, effect.lightning_explosion_white_flash.lightning_sprite) = CreateBattleAnimationSprite(group_bytes, 0, 0, 0xA8, 0x48, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_LOOP_ANIMATION), 0, 0);
        PlayBattleAnimationSound(0);
        asm volatile("" : : "r"(effect_state_slot));
        goto advance;
    case BATTLE_LIGHTNING_EXPLOSION_CREATE_SECOND_AND_THIRD_LIGHTNING: {
        register void **lightning_sprite_slot asm("r6") = (void **)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_explosion_white_flash.lightning_sprite));
        if (*(u16 *)((char *)*lightning_sprite_slot + BATTLE_SPRITE_OFFSET(animation_step)) != BATTLE_LIGHTNING_EXPLOSION_FIRST_ANIMATION_STEP) {
            return;
        }
        *lightning_sprite_slot = CreateBattleAnimationSprite(group_bytes, 0, 0, 0xA0, 0x60, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_LOOP_ANIMATION), 0, 0);
        CreateBattleAnimationSprite(group_bytes, 0, 0, 0xC7, 0x57, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_LOOP_ANIMATION), 0, 0);
        PlayBattleAnimationSound(0);
        asm volatile("" : : "r"(group_bytes));
        goto advance;
    }
    case BATTLE_LIGHTNING_EXPLOSION_CREATE_FOURTH_LIGHTNING: {
        register void **lightning_sprite_slot asm("r4") = (void **)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_explosion_white_flash.lightning_sprite));
        if (*(u16 *)((char *)*lightning_sprite_slot + BATTLE_SPRITE_OFFSET(animation_step)) != BATTLE_LIGHTNING_EXPLOSION_SECOND_ANIMATION_STEP) {
            return;
        }
        *lightning_sprite_slot = CreateBattleAnimationSprite(group_bytes, 0, 0, 0x9F, 0x2F, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_LOOP_ANIMATION), 0, 0);
        PlayBattleAnimationSound(0);
        asm volatile("" : : "r"(effect_state_slot));
        goto advance;
    }
    case BATTLE_LIGHTNING_EXPLOSION_CREATE_FIFTH_LIGHTNING:
        if (BATTLE_SPRITE_FIELD(BATTLE_ANIMATION_FIELD(group_bytes, void *, effect.lightning_explosion_white_flash.lightning_sprite), u16, animation_step) != 0xF) {
            return;
        }
        CreateBattleAnimationSprite(group_bytes, 0, 0, 0x88, 0x18, (BATTLE_SPRITE_BACKGROUND_RELATIVE | BATTLE_SPRITE_LOOP_ANIMATION), 0, 0);
        PlayBattleAnimationSound(0);
        BATTLE_ANIMATION_FIELD(group_bytes, u32, effect.lightning_explosion_white_flash.explosion_updates) = 0U;
        goto advance;
    case BATTLE_LIGHTNING_EXPLOSION_EMIT_EXPLOSIONS: {
        register u32 *explosion_updates_init asm("r0") = (u32 *)(group_bytes + BATTLE_ANIMATION_OFFSET(effect.lightning_explosion_white_flash.explosion_updates));
        register u32 explosion_update asm("r1");
        register u32 emission_remainder asm("r2");
        register u32 *explosion_updates_slot asm("r9");

        asm volatile(
            "ldr %1, [%0]\n\t"
            "mov %2, #3\n\t"
            "and %2, %2, %1\n\t"
            "mov %3, %0"
            : "+r"(explosion_updates_init), "=r"(explosion_update), "=r"(emission_remainder),
              "=r"(explosion_updates_slot));
        if (emission_remainder == 0) {
            register u32 explosion_index_bits asm("r0") = explosion_update >> 2;
            register u32 explosion_index asm("r6") = (u8)explosion_index_bits;
            register s32 x asm("r3");
            register const u16 *explosion_offsets_or_y_carrier asm("r1");
            register s32 explosion_offset_bytes asm("r4");
            register s32 y asm("r0");
            void *effect_sprite;

            {
                register char *group_view asm("r4") = group_bytes;
                x = *(s32 *)(group_view + BATTLE_ANIMATION_OFFSET(x));
            }
            /* Index 5 reads adjacent data after the five valid position pairs. */
            explosion_offsets_or_y_carrier = (const u16 *)gBattleLightningExplosionOffsets;
            explosion_offset_bytes = explosion_index * 4;
            {
                register u32 x_offset_bits asm("r0");
                asm volatile(
                    "add %0, %2, %3\n\t"
                    "ldrh %0, [%0]\n\t"
                    "add %1, %1, %0\n\t"
                    "lsl %1, %1, #16\n\t"
                    "asr %1, %1, #16"
                    : "=r"(x_offset_bits), "+r"(x)
                    : "r"(explosion_offset_bytes), "r"(explosion_offsets_or_y_carrier));
            }
            {
                register char *group_view asm("r5") = group_bytes;
                y = *(s32 *)(group_view + BATTLE_ANIMATION_OFFSET(y));
            }
            explosion_offsets_or_y_carrier = (const u16 *)((const char *)explosion_offsets_or_y_carrier + 2);
            asm volatile(
                "add %0, %1, %0\n\t"
                "ldrh %0, [%0]"
                : "+r"(explosion_offsets_or_y_carrier)
                : "r"(explosion_offset_bytes));
            y = (s16)(y + (u32)explosion_offsets_or_y_carrier);
            effect_sprite = CreateBattleAnimationSprite(group_bytes, 1, 0, x, y,
                emission_remainder, emission_remainder, emission_remainder);
            {
                register char *sprite_slot_destination asm("r1") = group_bytes;
                sprite_slot_destination += 0xC;
                sprite_slot_destination += explosion_offset_bytes;
                *(void **)sprite_slot_destination = effect_sprite;
            }
            if (explosion_index == 0) {
                register s32 background_palette_address asm("r5") = 0x05000000;
                register char *background_palette_buffer asm("r4") = (char *)BATTLE_WHITE_FLASH_PALETTE_ADDRESS(original_background_colors);

                BiosCpuFastSet(background_palette_address, (s32)background_palette_buffer, 0x20);
                {
                    register s16 *white_color_slot asm("r1") = &stack.white_color;
                    register s32 white asm("r2") = 0x7FFF;
                    register s32 white_view asm("r0");
                    asm volatile(
                        "mov %0, %1\n\t"
                        "strh %0, [%2]"
                        : "=r"(white_view)
                        : "r"(white), "r"(white_color_slot));
                }
                background_palette_buffer += 0x80;
                BiosCpuSet(&stack.white_color, (s32)background_palette_buffer, 0x01000040);
                QueueCopy((s32)background_palette_buffer, background_palette_address, 0x80);
                BATTLE_ANIMATION_FIELD(group_bytes, u32, effect.lightning_explosion_white_flash.palette_restore_updates) = 1U;
                SetBattleAnimationCameraMode(BATTLE_ANIMATION_CAMERA_IMPACT_SHAKE_AND_RECOIL_SCROLL, 0);
                PlayBattleAnimationSound(1);
            }
        }
        {
            register u32 *explosion_updates_view asm("r4") = explosion_updates_slot;
            register u32 value asm("r0") = *explosion_updates_view;
            if (value == BATTLE_LIGHTNING_EXPLOSION_LAST_EMISSION_UPDATE) {
                goto advance;
            }
            value += 1;
            {
                register u32 *explosion_updates_store asm("r5") = explosion_updates_slot;
                *explosion_updates_store = value;
            }
            return;
        }
    }
    case BATTLE_LIGHTNING_EXPLOSION_EMIT_SMOKE: {
        register s32 sprite_slot_index asm("r6") = 0;
        register char *sprite_slots asm("r4") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
        do {
            register s32 sprite_slot_offset asm("r0") = sprite_slot_index * 4;
            register void *explosion_sprite asm("r0") = *(void **)(sprite_slots + sprite_slot_offset);
            register s32 x_offset asm("r1");
            register s32 x asm("r3");
            register s32 y_offset asm("r2");
            register s32 y asm("r0");
            void *effect_sprite;

            asm volatile(
                "mov %0, #4\n\t"
                "ldrsh %1, [%4, %0]\n\t"
                "mov %2, #6\n\t"
                "ldrsh %3, [%4, %2]"
                : "=r"(x_offset), "=r"(x), "=r"(y_offset), "=r"(y)
                : "r"(explosion_sprite));
            effect_sprite = CreateBattleAngledProjectileSprite(group_bytes, 2, 0, x, y,
                BATTLE_SPRITE_SEMITRANSPARENT, 0, 0x80, 2);
            {
                register s32 sprite_slot_destination asm("r1");
                asm volatile(
                    "add %0, %1, #5\n\t"
                    "lsl %0, %0, #2\n\t"
                    "add %0, %2, %0\n\t"
                    "str %3, [%0]"
                    : "=r"(sprite_slot_destination)
                    : "r"(sprite_slot_index), "r"(sprite_slots), "r"(effect_sprite));
            }
            {
                register s32 next_index_bits asm("r0") = sprite_slot_index + 1;
                sprite_slot_index = (u8)next_index_bits;
            }
        } while ((u32)sprite_slot_index < BATTLE_LIGHTNING_EXPLOSION_SMOKE_COUNT);
        goto advance;
    }
advance:
        *effect_state_slot = *effect_state_slot + 1;
        return;
    case BATTLE_LIGHTNING_EXPLOSION_WAIT_FOR_SPRITES_AND_PALETTE: {
        register s32 sprite_slot_index asm("r6") = 0;
        register char *group_view asm("r4") = group_bytes;
        if (*(s32 *)(group_view + BATTLE_ANIMATION_OFFSET(sprites[0])) == 0) {
            register char *sprite_slots asm("r1") = group_bytes + BATTLE_ANIMATION_OFFSET(sprites[0]);
            do {
                register s32 next_index_bits asm("r0") = sprite_slot_index + 1;
                sprite_slot_index = (u8)next_index_bits;
            } while ((u32)sprite_slot_index < BATTLE_LIGHTNING_EXPLOSION_TRACKED_SPRITE_COUNT &&
                ({
                    register s32 sprite_slot_value asm("r0") = sprite_slot_index * 4;
                    asm volatile(
                        "add %0, %1, %0\n\t"
                        "ldr %0, [%0]"
                        : "+r"(sprite_slot_value)
                        : "r"(sprite_slots));
                    sprite_slot_value;
                }) == 0);
        }
        if ((sprite_slot_index == BATTLE_LIGHTNING_EXPLOSION_TRACKED_SPRITE_COUNT) && (BATTLE_ANIMATION_FIELD(group_bytes, u32, effect.lightning_explosion_white_flash.palette_restore_updates) == 0)) {
            DestroySpriteGroup(group_bytes);
        }
        return;
    }
    }
}
