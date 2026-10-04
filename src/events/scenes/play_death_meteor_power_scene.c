#include "m2c_prelude.h"
#include "../../graphics/camera.h"
#include "../../graphics/screen_effects.h"

M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
s32 *CreateSpriteFromTable(M2C_UNK, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
s32 *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(s32 *) asm("func_08094554");                       /* extern */
M2C_UNK QueueCopy(M2C_UNK, M2C_UNK, s32) asm("func_08095208");       /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadZoidIconGraphics(s32, s32, s32, s32) asm("func_0809A4CC");          /* extern */
M2C_UNK QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, s32, s32, s32) asm("func_0809AA64");      /* extern */
M2C_UNK BiosCpuFastSet(M2C_UNK, s32, s32) asm("func_080ECD28");           /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

void PlayDeathMeteorPowerScene(void) asm("func_080A6ED0");

void PlayDeathMeteorPowerScene(void) {
    s32 *scene_sprite_slots[2];
    s32 *death_meteor_sprite;
    s32 *glowing_sprite;
    register u8 *glowing_position_address asm("r0");
    register s32 *portrait_sprite asm("r9");
    register u32 palette_update_or_blue asm("r5");
    register s32 zero_r4 asm("r4");

    asm volatile("" : : "m"(scene_sprite_slots[1]));
    {
        register s32 *mode asm("r1") = (s32 *)0x02021690;
        asm volatile("" : "+r"(mode));
        *mode = 9;
    }
    {
        register s32 *battle_phase_or_palette_color asm("r1") = (s32 *)0x02030558;
        register s32 value asm("r0") = 0xFF10;
        asm volatile("" : "+r"(battle_phase_or_palette_color), "+r"(value));
        *battle_phase_or_palette_color = value;
    }
    {
        register s8 *battle_setup_address asm("r1") = (s8 *)0x0203055C;
        register s32 value asm("r0");
        register struct PerspectiveCamera *config asm("r1");
        asm volatile("" : "+r"(battle_setup_address));
        zero_r4 = 0;
        asm volatile("" : "+r"(zero_r4));
        value = 0x14;
        battle_setup_address[1] = value;
        config = (struct PerspectiveCamera *)0x030033C4;
        asm volatile("" : "+r"(config));
        config->pose.position.world_x = zero_r4;
        config->pose.position.world_z = zero_r4;
        config->pose.position.depth_offset = 0x10000;
        palette_update_or_blue = 0;
        config->pose.orientation.angles.pitch = 0x20;
        asm volatile(
            "strh r4, [r1, #16]\n\t"
            "strh r4, [r1, #14]"
            : : "r"(zero_r4), "r"(config) : "memory");
        config->projection.screen_center_x = 0x78;
        config->projection.screen_center_y = 0x58;
        config->projection.focal_length = 0x80;
        config->far_clip_depth = 0x20000;
    }
    YieldTaskForUpdates(1);
    LoadZoidIconGraphics(0x91, 0, 0, 0);
    death_meteor_sprite = CreateSprite(0x0821024C, 0x08210258, 0, 0,
        zero_r4, zero_r4, zero_r4, 0x82C8, 0x080BAE19);
    *(s32 **)0x02032E8C = death_meteor_sprite;
    M2C_FIELD(death_meteor_sprite, s32 *, 0x28) = zero_r4;
    M2C_FIELD(death_meteor_sprite, s32 *, 0x2C) = zero_r4;
    M2C_FIELD(death_meteor_sprite, s32 *, 0x30) = zero_r4;
    *(s8 *)0x02032EEC = palette_update_or_blue;
    LoadSpriteGraphicsFromTable(0x087AF9D4, 6, 0x40, 1);
    glowing_sprite = CreateSpriteFromTable(0x087AFA94, 6, 0, 0,
        zero_r4, 0x40, 1, 0x7E0, 0x080A6DB1);
    scene_sprite_slots[0] = glowing_sprite;
    glowing_position_address = (u8 *)glowing_sprite;
    glowing_position_address += 0x28;
    *(s32 *)glowing_position_address = 0x6000;
    *(s32 *)(glowing_position_address + 4) = zero_r4;
    *(s32 *)(glowing_position_address + 8) = zero_r4;
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    goto test_ready_1;
wait_ready_1:
    YieldTaskForUpdates(1);
test_ready_1:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_1;
    }
    portrait_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x34, 2, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018272);
    QueuePilotPortraitGraphics(0x34, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x0801829D);
    {
        register s32 *load_flags asm("r1") = portrait_sprite;
        register u32 flags asm("r0");
        register s32 *store_flags asm("r1");
        asm volatile("" : "+r"(load_flags));
        flags = *load_flags;
        flags |= 0x20000;
        store_flags = portrait_sprite;
        asm volatile("" : "+r"(store_flags));
        *store_flags = flags;
    }
    RunMenuScript(0x08017BE6);
    {
        register s32 *glowing_world_x_address asm("r5") = (s32 *)((u8 *)scene_sprite_slots[0] + 0x28);
        asm volatile("" : "+r"(glowing_world_x_address));
        if (*glowing_world_x_address != 0) {
            register s32 *current asm("r4");
            do {
                current = glowing_world_x_address;
                asm volatile("" : "+r"(current));
                *glowing_world_x_address += 0xFFFFFF00;
                YieldTaskForUpdates(1);
            } while (*current != 0);
        }
    }
    DestroySprite(scene_sprite_slots[0]);
    {
        register s32 target asm("r0") = 0x05000200;
        register s32 palette asm("r4");
        asm volatile("" : "+r"(target));
        palette = 0x02002880;
        asm volatile("" : "+r"(palette));
        BiosCpuFastSet(target, palette, 8);
    }
    palette_update_or_blue = 0;
    PlaySong(0x54);
    {
        register u32 max_channel asm("r6") = 0x1F;
        register u16 *output_base asm("sl") = (u16 *)0x020028A0;
        register u32 next_outer asm("r8");
        u32 inner;
        register s32 white_blend_strength asm("r4");

        asm volatile("" : "+r"(max_channel), "+r"(output_base));
outer_palette:
        inner = 0;
        next_outer = palette_update_or_blue + 1;
        white_blend_strength = max_channel - palette_update_or_blue;
inner_palette:
        {
            register u32 offset asm("r2") = inner << 1;
            register u16 *input_base asm("r1") = (u16 *)0x02002880;
            register u32 input_address asm("r0");
            register u32 battle_phase_or_palette_color asm("r0");
            register u32 red asm("r3");
            register u32 green asm("r1");
            register s32 blend asm("r0");

            asm volatile("" : "+r"(offset), "+r"(input_base));
            input_address = offset + (u32)input_base;
            asm volatile("" : "+r"(input_address));
            battle_phase_or_palette_color = *(u16 *)input_address;
            red = max_channel;
            asm volatile("" : "+r"(red));
            red &= battle_phase_or_palette_color;
            battle_phase_or_palette_color <<= 16;
            green = (battle_phase_or_palette_color >> 21) & max_channel;
            palette_update_or_blue = (battle_phase_or_palette_color >> 26) & max_channel;

            blend = (max_channel - red) * white_blend_strength;
            if (blend < 0) {
                blend += 0x1F;
            }
            blend >>= 5;
            blend = red + blend;
            blend <<= 24;
            red = (u32)blend >> 24;

            blend = (max_channel - green) * white_blend_strength;
            if (blend < 0) {
                blend += 0x1F;
            }
            blend >>= 5;
            blend = green + blend;
            blend <<= 24;
            green = (u32)blend >> 24;

            blend = (max_channel - palette_update_or_blue) * white_blend_strength;
            if (blend < 0) {
                blend += 0x1F;
            }
            blend >>= 5;
            blend = palette_update_or_blue + blend;
            blend <<= 24;

            asm volatile("" : "+r"(offset), "+r"(output_base));
            offset = (u32)((u8 *)output_base + offset);
            green <<= 5;
            red |= green;
            blend = (u32)blend >> 14;
            red |= blend;
            *(u16 *)offset = red;
        }
        {
            register u32 next_inner asm("r0") = inner + 1;
            asm volatile("" : "+r"(next_inner));
            next_inner <<= 24;
            inner = next_inner >> 24;
        }
        if (inner <= 0xF) {
            goto inner_palette;
        }
        QueueCopy(0x020028A0, 0x05000200, 0x20);
        {
            register u32 restored_outer asm("r1") = next_outer;
            register u32 narrowed_outer asm("r0");
            asm volatile("" : "+r"(restored_outer));
            narrowed_outer = restored_outer << 24;
            palette_update_or_blue = narrowed_outer >> 24;
        }
        YieldTaskForUpdates(1);
        if (palette_update_or_blue <= 0x1F) {
            goto outer_palette;
        }
    }
    StartTask(4, 0x080A6E15);
    {
        register s32 *load_flags asm("r1") = portrait_sprite;
        register u32 flags asm("r0");
        register s32 *store_flags asm("r1");
        asm volatile("" : "+r"(load_flags));
        flags = *load_flags;
        flags &= 0xFFFDFFFF;
        store_flags = portrait_sprite;
        asm volatile("" : "+r"(store_flags));
        *store_flags = flags;
    }
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(1, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x080182F9);
    QueuePilotPortraitGraphics(0x34, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018315);
    DestroySprite(portrait_sprite);
    RunMenuScript(0x08017BE6);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    goto test_ready_2;
wait_ready_2:
    YieldTaskForUpdates(1);
test_ready_2:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_2;
    }
    StopTask(4);
    *(s32 *)0x02021690 = -1;
    YieldTaskForUpdates(1);
}
