#include "m2c_prelude.h"
#include "../../graphics/camera.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"

extern volatile u16 gBlendControlShadow asm("D_0300004E");
extern volatile u16 gBlendAlphaShadow asm("D_03000050");
extern void *gBattleUnitSprites[] asm("D_02032E8C");

extern struct PerspectiveCamera gPerspectiveCamera asm("D_030033C4");

M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
M2C_UNK StopSong(s32) asm("func_08092EA0");                         /* extern */
M2C_UNK ClearSpritePools() asm("func_08094330");                            /* extern */
void *CreateSpriteFromTable(M2C_UNK, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
void *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(void *) asm("func_08094554");                      /* extern */
M2C_UNK SetSpriteAnimation(void *, s32) asm("func_08094564");                 /* extern */
M2C_UNK QueueCopy(M2C_UNK, M2C_UNK, s32) asm("func_08095208");       /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadZoidIconGraphics(s32, s32, s32, s32) asm("func_0809A4CC");          /* extern */
M2C_UNK QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(s32, s32, s32, s32) asm("func_0809AA64");          /* extern */
M2C_UNK BiosCpuFastSet(s32 *, s32, s32) asm("func_080ECD28");             /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

void PlayArcadiaTimeSpaceUnitDestructionScene(void) asm("func_080A9344");

void PlayArcadiaTimeSpaceUnitDestructionScene(void) {
    s32 vram_clear_word;
    void *beam_sprite;
    void *explosion_sprite;
    s32 camera_x_velocity_fixed8;
    s32 beam_offset_y_fixed8;
    s32 *beam_world_x_address;
    s32 temp_r0_6;
    s32 temp_r1_6;
    s32 ramp_limit;
    s32 var_r0_6;
    s32 var_r0_7;
    register s32 zoid_or_beam_velocity_fixed8 asm("r9");
    register u32 flash_delay_updates asm("r5");
    register u32 white_hold_updates asm("r5");
    register u32 explosion_position_update asm("r7");
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r1;
    void *temp_r1_2;
    void *temp_r1_5;
    void *portrait_sprite;
    register s32 zero_r4 asm("r4");

    asm volatile("" : "=m"(vram_clear_word), "=m"(beam_sprite), "=m"(explosion_sprite),
                  "=m"(camera_x_velocity_fixed8), "=m"(beam_offset_y_fixed8), "=m"(beam_world_x_address));
    *(s32 *)0x02021690 = GAME_MODE_BATTLE;
    *(s32 *)0x02030558 = 0xFF10;
    {
        register s8 *battle_setup_address asm("r0") = (s8 *)0x0203055C;
        register struct PerspectiveCamera *config asm("r1");

        zero_r4 = 0;
        asm volatile("" : "+r"(battle_setup_address), "+r"(zero_r4));
        battle_setup_address[1] = zero_r4;
        config = &gPerspectiveCamera;
        asm volatile("" : "+r"(config));
        config->pose.position.world_x = zero_r4;
        config->pose.position.world_z = zero_r4;
        config->pose.position.depth_offset = 0x8000;
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
    {
    register s32 menu asm("r7") = 0x087AF9D4;
    register void *template_a asm("r9");
    register void *template_b_r1 asm("r1");
    register void *template_b asm("r10");
    register void **sprite_slots asm("r6");
    register s32 scene_y asm("r8");
    asm volatile("" : "+r"(menu));
    LoadSpriteGraphicsFromTable(menu, 0x13, 0, 0);
    template_a = (void *)0x0821024C;
    template_b_r1 = (void *)0x08210258;
    template_b = template_b_r1;
    asm volatile("" : "+r"(template_a), "+r"(template_b_r1), "+r"(template_b));
    temp_r0 = CreateSprite(template_a, template_b_r1, 0, 0, zero_r4,
        zero_r4, zero_r4, ({
            register s32 scene_y_seed asm("r2") = 0x2C8;
            asm volatile("" : "+r"(scene_y_seed));
            scene_y = scene_y_seed;
            asm volatile("" : "+r"(scene_y), "+r"(scene_y_seed));
            scene_y_seed;
        }), 0x080BAE19);
    sprite_slots = gBattleUnitSprites;
    asm volatile("" : "+r"(sprite_slots));
    sprite_slots[0] = temp_r0;
    M2C_FIELD(temp_r0, s32 *, 0x28) = 0xFFF80000;
    M2C_FIELD(temp_r0, s32 *, 0x2C) = zero_r4;
    M2C_FIELD(temp_r0, s32 *, 0x30) = zero_r4;
    {
        register u8 *state asm("r5") = (u8 *)0x02032EEC;

        asm volatile("" : "+r"(state));
        {
            register s32 zero asm("r3") = 0;
            asm volatile("" : "+r"(zero));
            state[0] = zero;
        }
        LoadZoidIconGraphics(0x74, 0, 0x40, 1);
        temp_r0_2 = CreateSprite(template_a, template_b, 0, 0, zero_r4,
            0x40, 1, ({
                register s32 scene_y_copy asm("r0") = scene_y;
                asm volatile("" : "+r"(scene_y_copy));
                scene_y_copy;
            }), 0x080BADD5);
        sprite_slots[1] = temp_r0_2;
        M2C_FIELD(temp_r0_2, s32 *, 0x28) = 0x10000;
        M2C_FIELD(temp_r0_2, s32 *, 0x2C) = zero_r4;
        M2C_FIELD(temp_r0_2, s32 *, 0x30) = zero_r4;
        {
            register s32 zero asm("r1") = 0;
            asm volatile("" : "+r"(zero));
            state[1] = zero;
        }
    }
    LoadSpriteGraphicsFromTable(menu, 7, 0x80, 2);
    }
    LoadSpriteGraphicsFromTable(0x087AC9F8, 0x16, 0x100, 4);
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    goto test_ready_2;
wait_ready_2:
    YieldTaskForUpdates(1);
test_ready_2:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_2;
    }
    gBlendControlShadow = 0x740;
    gBlendAlphaShadow = 0x810;
    if (M2C_FIELD(gBattleUnitSprites[1], s32 *, 0x28) != 0) {
        do {
            temp_r1 = gBattleUnitSprites[1];
            M2C_FIELD(temp_r1, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r1, s32 *, 0x28) + 0xFFFFF000);
            YieldTaskForUpdates(1);
        } while (M2C_FIELD(gBattleUnitSprites[1], s32 *, 0x28) != 0);
    }
    {
    void *asset_a = (void *)0x08359850;
    void *asset_b = (void *)0x0835985C;
    register s32 scene_x asm("r6");

    asm volatile("" : "+r"(asset_a), "+r"(asset_b));
    portrait_sprite = CreateSprite(asset_a, asset_b, 0, 8, 0x68,
        ({ scene_x = 0x3C2; scene_x; }), 0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(1, 3, 0, scene_x, 0xD, 0x02002880);
    }
    RunMenuScript(0x08018AF8);
    DestroySprite(portrait_sprite);
    RunMenuScript(0x08017BE6);
    zoid_or_beam_velocity_fixed8 = 0;
    {
        register s32 ramp_zero asm("r0") = 0;

        asm volatile("" : "+r"(ramp_zero));
        camera_x_velocity_fixed8 = ramp_zero;
    }
    PlaySong(0x68);
    ramp_limit = (s32)0xFFFFF000;
    asm volatile("" : : "r"(ramp_limit));
    do {
        {
        register s32 ramp_value asm("r1") = camera_x_velocity_fixed8;

        asm volatile("" : "+r"(ramp_value));
        if (ramp_value > ramp_limit) {
            register s32 ramp_delta asm("r2") = 0xFFFFFE00;

            asm volatile("" : "+r"(ramp_delta));
            ramp_value += ramp_delta;
            camera_x_velocity_fixed8 = ramp_value;
        }
        }
        {
            register s32 motion_delta asm("r3") = 0xFFFFFF00;

            asm volatile("" : "+r"(motion_delta));
            zoid_or_beam_velocity_fixed8 += motion_delta;
        }
        gPerspectiveCamera.pose.position.world_x += camera_x_velocity_fixed8;
        {
            register s32 depth_offset_value asm("r0") = gPerspectiveCamera.pose.position.depth_offset;
            register s32 depth_offset_delta asm("r2");

            asm volatile("" : "+r"(depth_offset_value));
            depth_offset_delta = 0x100;
            asm volatile("" : "+r"(depth_offset_delta));
            depth_offset_value += depth_offset_delta;
            gPerspectiveCamera.pose.position.depth_offset = depth_offset_value;
        }
        temp_r1_2 = gBattleUnitSprites[1];
        M2C_FIELD(temp_r1_2, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r1_2, s32 *, 0x28) + zoid_or_beam_velocity_fixed8);
        YieldTaskForUpdates(1);
    } while (zoid_or_beam_velocity_fixed8 > ramp_limit);
    temp_r0_3 = CreateSpriteFromTable(0x087AFA94, 7, 0, 0, 0, 0x80, 2, 0x7E0, 0x080BADD5);
    beam_sprite = temp_r0_3;
    {
        register void *child_fields asm("r2") = temp_r0_3 + 0x28;
        register void *source_fields asm("r1") = gBattleUnitSprites[1];

        asm volatile("" : "+r"(temp_r0_3), "+r"(child_fields),
                     "+r"(source_fields));
        M2C_FIELD(child_fields, s32 *, 0) =
            M2C_FIELD(source_fields, s32 *, 0x28) + 0x8000;
        M2C_FIELD(child_fields, s32 *, 4) =
            M2C_FIELD(source_fields, s32 *, 0x2C);
        M2C_FIELD(child_fields, s32 *, 8) =
            M2C_FIELD(source_fields, s32 *, 0x30);
    }
    beam_offset_y_fixed8 = -0x1000;
    BiosCpuFastSet((s32 *)0x05000220, 0x02002880, 8);
    {
    u32 outer = 0;
    register u16 *output_base asm("r8");
    register u32 next_outer asm("r10");
    register u32 max_channel asm("r6");
    goto test_loop_40;
loop_40:
    {
        {
            register s32 *beam_world_x_target_seed asm("r1") = beam_sprite + 0x28;

            asm volatile("" : "+r"(beam_world_x_target_seed));
            beam_world_x_address = beam_world_x_target_seed;
        }
        if (outer <= 0x10U) {
            register u32 inner asm("r5") = 0;
            register u16 *output_copy asm("ip");
            register u32 output_guard0 asm("r0");
            register u32 output_guard1 asm("r1");

            asm volatile("" : "=r"(output_guard0), "=r"(output_guard1));
            output_base = (u16 *)0x020028A0;
            asm volatile("" : "+r"(output_base)
                         : "r"(output_guard0), "r"(output_guard1));
            next_outer = outer + 1;
            max_channel = 0x1F;
            output_copy = output_base;
            asm volatile("" : "+r"(output_copy));
            do {
                register u32 offset asm("r2") = inner << 1;
                register u16 *input_base asm("r1") = (u16 *)0x02002880;
                register u32 input_address asm("r0");
                register u32 battle_phase_or_palette_color asm("r0");
                register u32 red asm("r3");
                register u32 green asm("r1");
                register u32 blue asm("r4");
                s32 blend;

                asm volatile("" : "+r"(offset), "+r"(input_base));
                input_address = offset + (u32)input_base;
                asm volatile("" : "+r"(input_address));
                battle_phase_or_palette_color = *(u16 *)input_address;
                red = max_channel;
                red &= battle_phase_or_palette_color;
                battle_phase_or_palette_color <<= 16;
                green = (battle_phase_or_palette_color >> 21) & max_channel;
                blue = (battle_phase_or_palette_color >> 26) & max_channel;
                blend = (max_channel - red) * outer;
                if (blend < 0) {
                    blend += 0xF;
                }
                blend >>= 4;
                asm volatile("add %0, %1, %0"
                             : "+r"(blend) : "r"(red));
                blend <<= 24;
                red = (u32)blend >> 24;
                blend = (max_channel - green) * outer;
                if (blend < 0) {
                    blend += 0xF;
                }
                blend >>= 4;
                asm volatile("add %0, %1, %0"
                             : "+r"(blend) : "r"(green));
                blend <<= 24;
                green = (u32)blend >> 24;
                blend = (max_channel - blue) * outer;
                if (blend < 0) {
                    blend += 0xF;
                }
                blend >>= 4;
                asm volatile("add %0, %1, %0"
                             : "+r"(blend) : "r"(blue));
                blend <<= 24;
                asm volatile("" : "+r"(offset), "+r"(output_copy));
                offset = (u32)((u8 *)output_copy + offset);
                green <<= 5;
                red |= green;
                blend = (u32)blend >> 14;
                red |= blend;
                *(u16 *)offset = red;
                {
                    register u32 next_inner asm("r0") = inner + 1;

                    asm volatile("" : "+r"(next_inner));
                    next_inner <<= 24;
                    inner = next_inner >> 24;
                }
            } while (inner <= 0xFU);
            {
                register u16 *call_source asm("r0") = output_base;
                register u32 call_destination asm("r1");

                asm volatile("" : "+r"(call_source));
                call_destination = 0x05000220;
                asm volatile("" : "+r"(call_destination));
                QueueCopy(call_source, call_destination, 0x20);
            }
            {
                register u32 restored_outer asm("r2") = next_outer;
                register u32 narrowed_outer asm("r0");

                asm volatile("" : "+r"(restored_outer));
                narrowed_outer = restored_outer << 24;
                outer = narrowed_outer >> 24;
            }
        }
        if ((M2C_FIELD(beam_sprite, u16 *, 0x12) == 0) && (M2C_FIELD(beam_sprite, s32 *, 0) & 4)) {
            void *dismiss = *(void *volatile *)&beam_sprite;

            SetSpriteAnimation(dismiss, 1);
            DestroySprite(gBattleUnitSprites[1]);
            {
                register s32 motion_reset asm("r3") = -0x1D00;

                asm volatile("" : "+r"(motion_reset));
                zoid_or_beam_velocity_fixed8 = motion_reset;
            }
            PlaySong(0x6B);
        }
        if (zoid_or_beam_velocity_fixed8 < (s32)0xFFFFF000) {
            register s32 motion_step asm("r0") = 0x100;

            asm volatile("" : "+r"(motion_step));
            zoid_or_beam_velocity_fixed8 += motion_step;
        }
        {
            register struct PerspectiveCamera *motion_config asm("r0") =
                &gPerspectiveCamera;
            register s32 motion_x asm("r2");
            register s32 motion_limit asm("r1");
            register struct PerspectiveCamera *motion_store asm("r3");
            register s32 next_x asm("r0");

            asm volatile("" : "+r"(motion_config));
            motion_x = motion_config->pose.position.world_x;
            asm volatile("" : "+r"(motion_x));
            motion_limit = (s32)0xFFF8E000;
            asm volatile("" : "+r"(motion_limit));
            motion_store = motion_config;
            asm volatile("" : "+r"(motion_store));
            if (motion_x > motion_limit) {
                register s32 ramp asm("r1") = camera_x_velocity_fixed8;

                asm volatile("" : "+r"(ramp));
                next_x = motion_x + ramp;
            } else {
                next_x = 0xFFF86000 - motion_x;
                if (next_x < 0) {
                    next_x += 7;
                }
                next_x >>= 3;
                next_x = motion_x + next_x;
            }
            motion_store->pose.position.world_x = next_x;
            {
            register void *flag_child asm("r2") = beam_sprite;

            asm volatile("" : "+r"(flag_child));
            if (M2C_FIELD(flag_child, u16 *, 0x12) == 0) {
                temp_r1_5 = gBattleUnitSprites[1];
                M2C_FIELD(temp_r1_5, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r1_5, s32 *, 0x28) + zoid_or_beam_velocity_fixed8);
            }
            motion_store->pose.position.depth_offset += 0x100;
            {
            register s32 *beam_world_x_target asm("r3") = beam_world_x_address;

            asm volatile("" : "+r"(beam_world_x_target));
            *beam_world_x_target += zoid_or_beam_velocity_fixed8;
            }
        if (M2C_FIELD(flag_child, u16 *, 0x12) != 0) {
            register s32 beam_offset_y_limit asm("r0") = (s32)0xFFFFE000;
            register s32 beam_offset_y_value asm("r1") = beam_offset_y_fixed8;

            asm volatile("" : "+r"(beam_offset_y_limit), "+r"(beam_offset_y_value));
            if (beam_offset_y_value > beam_offset_y_limit) {
                beam_offset_y_value -= 0x40;
                beam_offset_y_fixed8 = beam_offset_y_value;
            }
        }
        {
            register void *beam_sprite_for_offset asm("r1") =
                *(void *volatile *)&beam_sprite;
            register s32 rounded_beam_offset_y_fixed8 asm("r0") = beam_offset_y_fixed8;

            asm volatile("" : "+r"(beam_sprite_for_offset), "+r"(rounded_beam_offset_y_fixed8));
            var_r0_6 = rounded_beam_offset_y_fixed8;
            if (var_r0_6 < 0) {
                var_r0_6 += 0xFF;
            }
            M2C_FIELD(beam_sprite_for_offset, s16 *, 0xA) =
                (s16) (var_r0_6 >> 8);
            }
            }
        }
        YieldTaskForUpdates(1);
    }
test_loop_40:
    {
        register struct PerspectiveCamera *loop_config asm("r0") = &gPerspectiveCamera;
        register s32 depth_offset_for_limit asm("r1");

        asm volatile("" : "+r"(loop_config));
        depth_offset_for_limit = loop_config->pose.position.depth_offset;
        if (depth_offset_for_limit <= 0xFFFF) {
            goto loop_40;
        }
    }
    }
    SetSpriteAnimation(beam_sprite, 2);
    M2C_FIELD(beam_sprite, s32 *, 0) = (s32) (M2C_FIELD(beam_sprite, s32 *, 0) & ~0x30);
    explosion_sprite = CreateSpriteFromTable(0x087ACDD8, 0x16, 0, 0, 0x38, 0x100, 4, 0x85C0, 0);
    explosion_position_update = 0;
    PlaySong(0x72);
    do {
        temp_r1_6 = gPerspectiveCamera.pose.position.world_x;
        var_r0_7 = 0xFFF86000 - temp_r1_6;
        if (var_r0_7 < 0) {
            var_r0_7 += 7;
        }
        temp_r0_6 = temp_r1_6 + (var_r0_7 >> 3);
        gPerspectiveCamera.pose.position.world_x = temp_r0_6;
        {
            register void *position_child asm("r1");
            register s32 position_delta asm("r0");

            position_child = explosion_sprite;
            asm volatile("" : "+r"(position_child));
            position_delta = 0xFFF86000 - temp_r0_6;
            asm volatile("" : "+r"(position_delta));
            if (position_delta < 0) {
                position_delta += 0x1FF;
            }
            position_delta >>= 9;
            position_delta += 0x68;
            M2C_FIELD(position_child, s16 *, 4) = (s16)position_delta;
        }
        {
            register u32 next_position_frame asm("r0") = explosion_position_update + 1;

            asm volatile("" : "+r"(next_position_frame));
            next_position_frame <<= 24;
            explosion_position_update = next_position_frame >> 24;
        }
        YieldTaskForUpdates(1);
    } while ((u32) explosion_position_update <= 0x1FU);
    StartTask(5, 0x0809FF55);
    PlaySong(0x85);
    PlaySong(0x4A);
    flash_delay_updates = 0;
    do {
        register u32 next_delay_frame asm("r0");

        YieldTaskForUpdates(1);
        next_delay_frame = flash_delay_updates + 1;
        asm volatile("" : "+r"(next_delay_frame));
        next_delay_frame <<= 24;
        flash_delay_updates = next_delay_frame >> 24;
    } while ((u32) flash_delay_updates <= 0x3BU);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_WHITE, 0x78);
    goto test_ready_53;
wait_ready_53:
    YieldTaskForUpdates(1);
test_ready_53:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_53;
    }
    StopTask(5);
    ClearSpritePools();
    vram_clear_word = 0;
    BiosCpuFastSet(&vram_clear_word, 0x06000000, 0x01004000);
    *(s16 *)0x05000000 = 0x7FFF;
    white_hold_updates = 0;
    do {
        register u32 next_clear_frame asm("r0");

        YieldTaskForUpdates(1);
        next_clear_frame = white_hold_updates + 1;
        asm volatile("" : "+r"(next_clear_frame));
        next_clear_frame <<= 24;
        white_hold_updates = next_clear_frame >> 24;
    } while ((u32) white_hold_updates <= 0x77U);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x78);
    goto test_ready_58;
wait_ready_58:
    YieldTaskForUpdates(1);
test_ready_58:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_58;
    }
    StopSong(0x4A);
    *(s32 *)0x02021690 = -1;
    YieldTaskForUpdates(1);
}
