#include "m2c_prelude.h"
#include "../../graphics/camera.h"
#include "../../graphics/screen_effects.h"

struct DeathMeteorJunoCopySceneLocals {
    void *beam_sprite;
    void *glowing_sprite;
    s32 *portrait_sprite;
    s32 camera_x_velocity_fixed8;
    s32 beam_offset_y_fixed8;
    s32 *beam_world_x_address;
};

extern volatile u16 gBlendControlShadow asm("D_0300004E");
extern volatile u16 gBlendAlphaShadow asm("D_03000050");
extern s32 *gBattleUnitSprites[] asm("D_02032E8C");
extern struct PerspectiveCamera gPerspectiveCamera asm("D_030033C4");

M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
void *CreateSpriteFromTable(M2C_UNK, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
s32 *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite(s32 *) asm("func_08094554");                       /* extern */
M2C_UNK SetSpriteAnimation(void *, s32) asm("func_08094564");                 /* extern */
M2C_UNK QueueCopy(M2C_UNK, M2C_UNK, s32) asm("func_08095208");       /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadZoidIconGraphics(s32, s32, s32, s32) asm("func_0809A4CC");          /* extern */
M2C_UNK QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(s32, s32, s32, s32) asm("func_0809AA64");          /* extern */
M2C_UNK BiosCpuFastSet(M2C_UNK, M2C_UNK, s32) asm("func_080ECD28");       /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

void PlayDeathMeteorJunoCopyScene(void) asm("func_080A71E0");

void PlayDeathMeteorJunoCopyScene(void) {
    struct DeathMeteorJunoCopySceneLocals frame;
    s32 *temp_r0;
    s32 *temp_r0_2;
    s32 *temp_r1;
    s32 *temp_r1_2;
    s32 *temp_r1_3;
    s32 *temp_r1_5;
    s32 *temp_r2_3;
    s32 temp_r1_4;
    s32 temp_r1_6;
    s32 temp_r1_7;
    s32 temp_r2_2;
    s32 temp_r2_4;
    s32 temp_r3;
    s32 temp_r3_3;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 temp_r6;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r0_5;
    s32 var_r0_6;
    s32 var_r0_7;
    s32 var_r0_8;
    register s32 zoid_or_beam_velocity_fixed8 asm("r8");
    register s32 glowing_x_velocity_fixed8 asm("r8");
    u16 temp_r0_4;
    u16 temp_r0_7;
    u32 temp_r0_5;
    u32 temp_r0_8;
    register u32 beam_palette_update asm("r6");
    register u32 glowing_palette_update_or_strength asm("r6");
    register u32 dialogue_delay_updates asm("r6");
    u8 var_r7;
    u8 var_r7_2;
    void *created_beam;
    void *created_glowing_sprite;
    void *temp_r2;
    void *temp_r3_2;

    {
        register s32 zero_r4 asm("r4");
        register s32 zero_sl asm("sl");
        register void *tiles asm("r8");
        register void *layout asm("r9");
        register s32 **objects asm("r6");
        register u8 *states asm("r5");
        register s32 *object asm("r0");

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
            config->pose.position.depth_offset = 0x8000;
            zero_sl = 0;
            asm volatile("" : "+r"(zero_sl));
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
        tiles = (void *)0x0821024C;
        layout = (void *)0x08210258;
        asm volatile("" : "+r"(tiles), "+r"(layout));
        object = CreateSprite(tiles, layout, 0, 0,
            zero_r4, zero_r4, zero_r4, 0x82C8, 0x080BAE19);
        objects = (s32 **)0x02032E8C;
        asm volatile("" : "+r"(objects));
        objects[0] = object;
        object[10] = 0xFFF80000;
        object[11] = zero_r4;
        object[12] = zero_r4;
        states = (u8 *)0x02032EEC;
        asm volatile("" : "+r"(states));
        states[0] = zero_sl;
        LoadZoidIconGraphics(0x87, 6, 0x40, 1);
        object = CreateSprite(tiles, layout, 0, 0,
            zero_r4, 0x40, 1, 0x2C8, 0x080BADD5);
        objects[1] = object;
        object[10] = 0x10000;
        object[11] = zero_r4;
        object[12] = zero_r4;
        states[1] = zero_sl;
    }
    LoadSpriteGraphicsFromTable(0x087AF9D4, 6, 0x80, 2);
    LoadSpriteGraphicsFromTable(0x087AF9D4, 7, 0xC0, 3);
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    goto test_ready_1;
wait_ready_1:
    YieldTaskForUpdates(1);
test_ready_1:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_1;
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
    frame.portrait_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x36, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018477);
    {
        register s32 *flags asm("r3") = frame.portrait_sprite;
        asm volatile("" : "+r"(flags));
        *flags |= 0x20000;
    }
    RunMenuScript(0x08017BE6);
    {
        register s32 motion_zero asm("r0") = 0;
        asm volatile("" : "+r"(motion_zero));
        zoid_or_beam_velocity_fixed8 = motion_zero;
    }
    {
        register s32 ramp_zero asm("r1") = 0;
        asm volatile("" : "+r"(ramp_zero));
        frame.camera_x_velocity_fixed8 = ramp_zero;
    }
    PlaySong(0x68);
    {
    register s32 ramp_limit asm("r5") = -0x1000;
    register struct PerspectiveCamera *config asm("r4") = &gPerspectiveCamera;
    register s32 **ramp_objects asm("r6");

    asm volatile("" : "+r"(ramp_limit), "+r"(config));
    do {
        register s32 ramp_value asm("r2") = frame.camera_x_velocity_fixed8;

        asm volatile("" : "+r"(ramp_value));
        if (ramp_value > ramp_limit) {
            register s32 ramp_delta asm("r3") = -0x200;

            asm volatile("" : "+r"(ramp_delta));
            ramp_value += ramp_delta;
            frame.camera_x_velocity_fixed8 = ramp_value;
        }
        {
            register s32 motion_delta asm("r0") = -0x100;
            asm volatile("" : "+r"(motion_delta));
            zoid_or_beam_velocity_fixed8 += motion_delta;
        }
        config->pose.position.world_x += frame.camera_x_velocity_fixed8;
        {
            register s32 depth_offset_value asm("r0") = config->pose.position.depth_offset;
            register s32 depth_offset_delta asm("r2") = 0x100;

            asm volatile("" : "+r"(depth_offset_value), "+r"(depth_offset_delta));
            depth_offset_value += depth_offset_delta;
            config->pose.position.depth_offset = depth_offset_value;
        }
        ramp_objects = gBattleUnitSprites;
        asm volatile("" : "+r"(ramp_objects));
        temp_r1_2 = ramp_objects[1];
        M2C_FIELD(temp_r1_2, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r1_2, s32 *, 0x28) + zoid_or_beam_velocity_fixed8);
        YieldTaskForUpdates(1);
    } while (zoid_or_beam_velocity_fixed8 > ramp_limit);
    created_beam = CreateSpriteFromTable(0x087AFA94, 7, 0, 0, 0, 0xC0, 3, 0x7E0, 0x080BADD5);
    frame.beam_sprite = created_beam;
    {
        register void *child_fields asm("r2") = created_beam + 0x28;
        register void *source_fields asm("r1") = ramp_objects[1];

        asm volatile("" : "+r"(created_beam), "+r"(child_fields),
                     "+r"(source_fields));
        M2C_FIELD(child_fields, s32 *, 0) =
            M2C_FIELD(source_fields, s32 *, 0x28) + 0x8000;
        M2C_FIELD(child_fields, s32 *, 4) =
            M2C_FIELD(source_fields, s32 *, 0x2C);
        M2C_FIELD(child_fields, s32 *, 8) =
            M2C_FIELD(source_fields, s32 *, 0x30);
    }
    }
    frame.beam_offset_y_fixed8 = -0x1000;
    BiosCpuFastSet(0x05000220, 0x02002880, 8);
    beam_palette_update = 0;
    goto test_loop_35;
loop_35:
    {
        {
            register s32 *beam_world_x_target_seed asm("r1") = frame.beam_sprite + 0x28;
            asm volatile("" : "+r"(beam_world_x_target_seed));
            frame.beam_world_x_address = beam_world_x_target_seed;
        }
        if (beam_palette_update <= 0x10U) {
            register u32 max_channel asm("r5");
            register u16 *output_base asm("sl");
            register u16 *output_copy asm("ip");
            register u32 next_outer asm("r9");
            u32 inner;

            inner = 0;
            {
                register u16 *output_seed asm("r2") = (u16 *)0x020028A0;
                asm volatile("" : "+r"(output_seed));
                output_base = output_seed;
                asm volatile("" : "+r"(output_base), "+r"(output_seed));
            }
            {
                register u32 next_seed asm("r3") = beam_palette_update + 1;
                asm volatile("" : "+r"(next_seed));
                next_outer = next_seed;
            }
            max_channel = 0x1F;
            asm volatile("" : "+r"(max_channel));
            output_copy = output_base;
            asm volatile("" : "+r"(output_copy));
first_inner_palette:
            {
                register u32 offset asm("r2") = inner << 1;
                register u16 *input_base asm("r1") = (u16 *)0x02002880;
                register u32 input_address asm("r0");
                register u32 battle_phase_or_palette_color asm("r0");
                register u32 red asm("r3");
                register u32 green asm("r1");
                register u32 blue asm("r4");
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
                blue = (battle_phase_or_palette_color >> 26) & max_channel;

                blend = (max_channel - red) * beam_palette_update;
                if (blend < 0) {
                    blend += 0xF;
                }
                blend >>= 4;
                blend = red + blend;
                blend <<= 24;
                red = (u32)blend >> 24;

                blend = (max_channel - green) * beam_palette_update;
                if (blend < 0) {
                    blend += 0xF;
                }
                blend >>= 4;
                blend = green + blend;
                blend <<= 24;
                green = (u32)blend >> 24;

                blend = (max_channel - blue) * beam_palette_update;
                if (blend < 0) {
                    blend += 0xF;
                }
                blend >>= 4;
                blend = blue + blend;
                blend <<= 24;

                asm volatile("" : "+r"(offset), "+r"(output_copy));
                offset = (u32)((u8 *)output_copy + offset);
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
                goto first_inner_palette;
            }
            {
                register u16 *call_source asm("r0") = output_base;
                register u32 call_destination asm("r1") = 0x05000220;

                asm volatile("" : "+r"(call_source), "+r"(call_destination));
                QueueCopy(call_source, call_destination, 0x20);
            }
            {
                register u32 restored_outer asm("r2") = next_outer;
                register u32 narrowed_outer asm("r0");
                asm volatile("" : "+r"(restored_outer));
                narrowed_outer = restored_outer << 24;
                beam_palette_update = narrowed_outer >> 24;
            }
        }
        {
        register void *flag_child asm("r1") = frame.beam_sprite;

        asm volatile("" : "+r"(flag_child));
        if ((M2C_FIELD(flag_child, u16 *, 0x12) == 0) && (M2C_FIELD(flag_child, s32 *, 0) & 4)) {
            void *dismiss = *(void *volatile *)&frame.beam_sprite;

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
        }
        gPerspectiveCamera.pose.position.world_x += frame.camera_x_velocity_fixed8;
        {
        register void *motion_child asm("r3") = frame.beam_sprite;
        asm volatile("" : "+r"(motion_child));
        if (M2C_FIELD(motion_child, u16 *, 0x12) == 0) {
            temp_r1_5 = gBattleUnitSprites[1];
            M2C_FIELD(temp_r1_5, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r1_5, s32 *, 0x28) + zoid_or_beam_velocity_fixed8);
        }
        gPerspectiveCamera.pose.position.depth_offset += 0x100;
        {
            register s32 *beam_world_x_target asm("r2") = frame.beam_world_x_address;
            asm volatile("" : "+r"(beam_world_x_target));
            *beam_world_x_target += zoid_or_beam_velocity_fixed8;
        }
        if (M2C_FIELD(motion_child, u16 *, 0x12) != 0) {
            register s32 beam_offset_y_limit asm("r0") = (s32)0xFFFFE000;
            register s32 beam_offset_y_value asm("r3") = frame.beam_offset_y_fixed8;

            asm volatile("" : "+r"(beam_offset_y_limit), "+r"(beam_offset_y_value));
            if (beam_offset_y_value > beam_offset_y_limit) {
                beam_offset_y_value -= 0x40;
                frame.beam_offset_y_fixed8 = beam_offset_y_value;
            }
        }
        }
        {
            register void *beam_sprite_for_offset asm("r1") =
                *(void *volatile *)&frame.beam_sprite;
            register s32 rounded_beam_offset_y_fixed8 asm("r0") = frame.beam_offset_y_fixed8;

            asm volatile("" : "+r"(beam_sprite_for_offset), "+r"(rounded_beam_offset_y_fixed8));
            var_r0_4 = rounded_beam_offset_y_fixed8;
            if (var_r0_4 < 0) {
                var_r0_4 += 0xFF;
            }
            M2C_FIELD(beam_sprite_for_offset, s16 *, 0xA) = (s16) (var_r0_4 >> 8);
        }
        YieldTaskForUpdates(1);
    }
test_loop_35:
    if (gPerspectiveCamera.pose.position.depth_offset <= 0xFFFF) {
        goto loop_35;
    }
    SetSpriteAnimation(frame.beam_sprite, 2);
    M2C_FIELD(frame.beam_sprite, s32 *, 0) = (s32) (M2C_FIELD(frame.beam_sprite, s32 *, 0) & ~0x30);
    created_glowing_sprite = CreateSpriteFromTable(0x087AFA94, 6, 0, 0, 0, 0x80, 2, 0x7E0, 0x080A6DB1);
    frame.glowing_sprite = created_glowing_sprite;
    {
        register void *child_fields asm("r3") = created_glowing_sprite + 0x28;
        register void *source_fields asm("r2") = gBattleUnitSprites[0];

        asm volatile("" : "+r"(created_glowing_sprite), "+r"(child_fields),
                     "+r"(source_fields));
        M2C_FIELD(child_fields, s32 *, 0) =
            M2C_FIELD(source_fields, s32 *, 0x28);
        M2C_FIELD(child_fields, s32 *, 4) =
            M2C_FIELD(source_fields, s32 *, 0x2C);
        M2C_FIELD(child_fields, s32 *, 8) =
            M2C_FIELD(source_fields, s32 *, 0x30);
        M2C_FIELD(created_glowing_sprite, s32 *, 0x34) = 0;
    }
    {
        register s32 motion_seed asm("r0") = 0xC00;
        asm volatile("" : "+r"(motion_seed));
        glowing_x_velocity_fixed8 = motion_seed;
    }
    BiosCpuFastSet(0x05000200, 0x02002880, 8);
    glowing_palette_update_or_strength = 0;
    PlaySong(0x54);
    do {
        {
        register s32 motion_test asm("r1") = glowing_x_velocity_fixed8;
        asm volatile("" : "+r"(motion_test));
        if (motion_test > 0) {
            register s32 motion_step asm("r2") = -0x80;
            asm volatile("" : "+r"(motion_step));
            glowing_x_velocity_fixed8 += motion_step;
        }
        }
        {
            register s32 *position asm("r0") = frame.glowing_sprite + 0x28;
            asm volatile("" : "+r"(position));
            *position += glowing_x_velocity_fixed8;
        }
        temp_r1_6 = gPerspectiveCamera.pose.position.world_x;
        var_r0_5 = 0xFFF80000 - temp_r1_6;
        if (var_r0_5 < 0) {
            var_r0_5 += 7;
        }
        gPerspectiveCamera.pose.position.world_x = temp_r1_6 + (var_r0_5 >> 3);
        if (glowing_palette_update_or_strength <= 0x20U) {
            register u32 max_channel asm("r5");
            register u16 *output_base asm("sl");
            register u16 *output_copy asm("ip");
            register u32 next_outer asm("r9");
            u32 inner;

            inner = 0;
            {
                register u16 *output_seed asm("r3") = (u16 *)0x020028A0;
                asm volatile("" : "+r"(output_seed));
                output_base = output_seed;
                asm volatile("" : "+r"(output_base), "+r"(output_seed));
            }
            {
                register u32 next_seed asm("r0") = glowing_palette_update_or_strength + 1;
                asm volatile("" : "+r"(next_seed));
                next_outer = next_seed;
            }
            max_channel = 0x1F;
            asm volatile("" : "+r"(max_channel));
            glowing_palette_update_or_strength = max_channel - glowing_palette_update_or_strength;
            output_copy = output_base;
            asm volatile("" : "+r"(output_copy));
second_inner_palette:
            {
                register u32 offset asm("r2") = inner << 1;
                register u16 *input_base asm("r1") = (u16 *)0x02002880;
                register u32 input_address asm("r0");
                register u32 battle_phase_or_palette_color asm("r0");
                register u32 red asm("r3");
                register u32 green asm("r1");
                register u32 blue asm("r4");
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
                blue = (battle_phase_or_palette_color >> 26) & max_channel;

                blend = (max_channel - red) * glowing_palette_update_or_strength;
                if (blend < 0) {
                    blend += 0x1F;
                }
                blend >>= 5;
                blend = red + blend;
                blend <<= 24;
                red = (u32)blend >> 24;

                blend = (max_channel - green) * glowing_palette_update_or_strength;
                if (blend < 0) {
                    blend += 0x1F;
                }
                blend >>= 5;
                blend = green + blend;
                blend <<= 24;
                green = (u32)blend >> 24;

                blend = (max_channel - blue) * glowing_palette_update_or_strength;
                if (blend < 0) {
                    blend += 0x1F;
                }
                blend >>= 5;
                blend = blue + blend;
                blend <<= 24;

                asm volatile("" : "+r"(offset), "+r"(output_copy));
                offset = (u32)((u8 *)output_copy + offset);
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
                goto second_inner_palette;
            }
            {
                register u16 *call_source asm("r0") = output_base;
                register u32 call_destination asm("r1") = 0x05000200;

                asm volatile("" : "+r"(call_source), "+r"(call_destination));
                QueueCopy(call_source, call_destination, 0x20);
            }
            {
                register u32 restored_outer asm("r2") = next_outer;
                register u32 narrowed_outer asm("r0");
                asm volatile("" : "+r"(restored_outer));
                narrowed_outer = restored_outer << 24;
                glowing_palette_update_or_strength = narrowed_outer >> 24;
            }
        }
        YieldTaskForUpdates(1);
    } while (glowing_palette_update_or_strength <= 0x1FU);
    {
        register s32 *flags asm("r3") = frame.portrait_sprite;
        asm volatile("" : "+r"(flags));
        *flags &= 0xFFFDFFFF;
    }
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x17, 0, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x0801850D);
    {
        register s32 *flag_load asm("r1") = frame.portrait_sprite;
        register u32 flags asm("r0");

        asm volatile("" : "+r"(flag_load));
        flags = *flag_load;
        {
            register u32 flag_mask asm("r1") = 0x20000;
            asm volatile("" : "+r"(flag_mask));
            flags |= flag_mask;
        }
        {
            register s32 *flag_store asm("r2") =
                *(s32 *volatile *)&frame.portrait_sprite;
            asm volatile("" : "+r"(flag_store));
            *flag_store = flags;
        }
    }
    RunMenuScript(0x08017BE6);
    StartTask(4, 0x080A6E15);
    dialogue_delay_updates = 0;
    do {
        register u32 next_delay asm("r0") = dialogue_delay_updates + 1;
        asm volatile("" : "+r"(next_delay));
        next_delay <<= 24;
        dialogue_delay_updates = next_delay >> 24;
        YieldTaskForUpdates(1);
    } while ((u32) dialogue_delay_updates <= 0x3BU);
    {
        register s32 *flags asm("r3") = frame.portrait_sprite;
        asm volatile("" : "+r"(flags));
        *flags &= 0xFFFDFFFF;
    }
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x16, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x0801854A);
    QueuePilotPortraitGraphics(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018575);
    QueuePilotPortraitGraphics(0x39, 0, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x080185DE);
    QueuePilotPortraitGraphics(0x39, 2, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018737);
    QueuePilotPortraitGraphics(0x34, 2, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x080187CE);
    QueuePilotPortraitGraphics(0x34, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x0801885A);
    DestroySprite(frame.portrait_sprite);
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
