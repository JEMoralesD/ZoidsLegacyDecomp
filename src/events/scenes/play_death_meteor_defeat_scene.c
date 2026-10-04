#include "m2c_prelude.h"
#include "../../battle/battle_display.h"
#include "../../graphics/camera.h"
#include "../../graphics/screen_effects.h"

extern struct PerspectiveCamera gPerspectiveCamera asm("D_030033C4");
extern volatile u16 gBlendControlShadow asm("D_0300004E");
extern volatile u16 gBlendAlphaShadow asm("D_03000050");
extern s32 *gBattleUnitSprites[] asm("D_02032E8C");
extern u8 gBattleUnitSpriteMotionStates[] asm("D_02032EEC");

M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
s32 *CreateSpriteFromTable(M2C_UNK, s32, u16, s16, s32, s32, s32, s32, s32) asm("func_08094374"); /* extern */
asm(".set func_08094374_4, func_08094374");
extern s32 *CreateSpriteFromTable_4(s32, s32, s32, s32) asm("func_08094374_4");
s32 *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
asm(".set func_08094484_4, func_08094484");
extern s32 *CreateSprite_4(s32, s32, s32, s32) asm("func_08094484_4");
M2C_UNK DestroySprite(s32 *) asm("func_08094554");                       /* extern */
M2C_UNK SetSpriteAnimation(s32 *) asm("func_08094564");                       /* extern */
M2C_UNK QueueCopy(M2C_UNK, M2C_UNK, s32) asm("func_08095208");       /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadZoidIconGraphics(s32, s32, s32, s32) asm("func_0809A4CC");          /* extern */
M2C_UNK QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(s32, s32, s32, s32) asm("func_0809AA64");          /* extern */
M2C_UNK BiosCpuFastSet(M2C_UNK, s32, s32) asm("func_080ECD28");           /* extern */
s32 CallFunctionR0(s32) asm("func_080ECD5C");                             /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

void PlayDeathMeteorDefeatScene(void) asm("func_080A8800");

void PlayDeathMeteorDefeatScene(void) {
    s32 *effect_sprites[32];
    s32 *portrait_sprite;
    s32 beam_x_velocity_fixed8;
    s32 beam_offset_y_fixed8;
    u32 camera_x_velocity_low16;
    s32 defeat_phase;
    s16 temp_r1;
    s16 temp_r3_3;
    s16 temp_r4;
    s32 temp_r4_2;
    s32 rounded_beam_offset_y;
    s32 **clear_slot;
    s32 **cleanup_slot;
    s32 **destroy_slot;
    register s32 **explosion_slot asm("r5");
    s32 *temp_r0;
    s32 *temp_r0_10;
    s32 *temp_r0_4;
    s32 *temp_r0_5;
    s32 *temp_r0_7;
    s32 *rng;
    s32 *temp_r1_2;
    s32 *destroy_candidate;
    s32 *temp_r2_2;
    s32 effect_active_flag;
    register s32 temp_r0_6 asm("r0");
    s32 camera_depth_offset;
    s32 camera_world_x;
    s32 camera_depth_before_easing;
    s32 temp_r2;
    s32 temp_r2_3;
    register s32 temp_r3 asm("r3");
    s32 temp_r3_2;
    register s32 temp_r5 asm("r5");
    register s32 temp_r5_2 asm("r5");
    s32 white_blend_strength;
    s32 negative_camera_world_x;
    s32 camera_depth_delta;
    s32 updates_since_explosion;
    register s32 explosion_spawn_interval asm("sl");
    u16 temp_r0_2;
    register u16 temp_r0_8 asm("r0");
    u32 temp_r0_3;
    u32 temp_r0_9;
    register u32 initial_palette_color_index asm("r6");
    register u32 white_hold_updates asm("r6");
    register u32 beam_palette_color_index asm("r6");
    register u32 camera_hold_updates asm("r6");
    register u32 pulse_hold_updates asm("r6");
    register u32 clear_slot_index asm("r6");
    register u32 cleanup_slot_index asm("r6");
    register u32 explosion_slot_index asm("r6");
    u32 explosion_animation;
    register u32 destroy_slot_index asm("r6");
    register u32 initial_palette_update asm("r8");
    register u32 beam_palette_update asm("r8");
    register u32 explosion_elapsed_updates asm("r8");
    register void *var_ip asm("ip");
    register u32 palette_next_outer_2 asm("r9");
    register u32 tail_next asm("r9");
    register u32 palette_max_2 asm("r4");
    register void *var_r4 asm("r4");
    register void *palette_base asm("sl");
    register u32 palette_next_outer asm("r9");
    register s32 zero_r4 asm("r4");
    register u32 zero_r5 asm("r5");

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
        register s8 *battle_setup_address asm("r0") = (s8 *)0x0203055C;
        register struct PerspectiveCamera *config asm("r1");

        zero_r4 = 0;
        asm volatile("" : "+r"(battle_setup_address), "+r"(zero_r4));
        battle_setup_address[1] = zero_r4;
        config = &gPerspectiveCamera;
        asm volatile("" : "+r"(config));
        config->pose.position.world_x = zero_r4;
        config->pose.position.world_z = zero_r4;
        config->pose.position.depth_offset = 0x10000;
        zero_r5 = 0;
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
    temp_r0 = CreateSprite(0x0821024C, 0x08210258, 0, 0,
        zero_r4, zero_r4, zero_r4, 0x82C8, 0x080BAE19);
    gBattleUnitSprites[0] = temp_r0;
    M2C_FIELD(temp_r0, s32 *, 0x28) = zero_r4;
    M2C_FIELD(temp_r0, s32 *, 0x2C) = zero_r4;
    M2C_FIELD(temp_r0, s32 *, 0x30) = zero_r4;
    gBattleUnitSpriteMotionStates[0] = zero_r5;
    LoadSpriteGraphicsFromTable(0x087AF9D4, 7, 0x80, 2);
    LoadSpriteGraphicsFromTable(0x087AF9D4, 4, 0x100, 4);
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
    {
        register s32 palette_target asm("r0") = 0x05000200;
        var_r4 = (void *)0x02002880;
        asm volatile("" : "+r"(palette_target), "+r"(var_r4));
        BiosCpuFastSet(palette_target, var_r4, 8);
    }
    zero_r5 = 0;
    initial_palette_update = zero_r5;
    palette_base = var_r4;
    do {
        initial_palette_color_index = 0;
        palette_next_outer = initial_palette_update + 1;
        var_r4 = palette_base;
loop_6:
        {
        u32 green_blend_1;
        u32 packed_1;
        register u32 red_blend_1 asm("r0");
        register u32 red_source_1 asm("r2");

        red_blend_1 = M2C_FIELD(var_r4, u16 *, 0);
        asm volatile("add %0, r7, #0" : "=r"(red_source_1));
        red_source_1 &= red_blend_1;
        temp_r2 = red_source_1;
        red_blend_1 <<= 0x10;
        temp_r5 = (red_blend_1 >> 0x15) & 0x1F;
        temp_r3 = (red_blend_1 >> 0x1A) & 0x1F;
        red_blend_1 = (0x1F - temp_r2) * initial_palette_update;
        red_blend_1 >>= 5;
        red_blend_1 = temp_r2 + red_blend_1;
        red_blend_1 <<= 0x18;
        temp_r2 = red_blend_1 >> 0x18;
        red_blend_1 = 0x1F - temp_r5;
        asm volatile("" : "+r"(red_blend_1));
        green_blend_1 = red_blend_1;
        green_blend_1 *= initial_palette_update;
        green_blend_1 >>= 5;
        asm volatile("add %0, %1, %0"
                     : "+r"(green_blend_1)
                     : "r"(temp_r5));
        green_blend_1 <<= 0x18;
        red_blend_1 = 0x1F - temp_r3;
        red_blend_1 *= initial_palette_update;
        red_blend_1 >>= 5;
        red_blend_1 = temp_r3 + red_blend_1;
        red_blend_1 <<= 0x18;
        packed_1 = temp_r2;
        packed_1 |= green_blend_1 >> 0x13;
        packed_1 |= red_blend_1 >> 0xE;
        M2C_FIELD(var_r4, u16 *, 0x20) = packed_1;
        }
        var_r4 += 2;
        initial_palette_color_index += 1;
        if (initial_palette_color_index <= 0xFU) {
            goto loop_6;
        }
        QueueCopy(0x020028A0, 0x05000200, 0x20);
        initial_palette_update = palette_next_outer;
        YieldTaskForUpdates(1);
    } while (initial_palette_update <= 0x20U);
    temp_r0_4 = CreateSpriteFromTable(0x087AFA94, 7, 3U, 0, 0, 0x80, 2, 0x87E0, 0x080BADD5);
    effect_sprites[0] = temp_r0_4;
    M2C_FIELD(temp_r0_4, s32 *, 0x28) = 0xFFFF8000;
    M2C_FIELD(temp_r0_4, s32 *, 0x2C) = 0;
    M2C_FIELD(temp_r0_4, s32 *, 0x30) = 0;
    beam_x_velocity_fixed8 = 0x2000;
    beam_offset_y_fixed8 = 0xE000;
    camera_x_velocity_low16 = 0x2000;
    defeat_phase = DEATH_METEOR_DEFEAT_BEAM_DESCENDING;
    beam_palette_update = 0;
    PlaySong(0x6B);
loop_9:
    switch (defeat_phase) {                                 /* irregular */
    case DEATH_METEOR_DEFEAT_BEAM_DESCENDING:
        if (M2C_FIELD(effect_sprites[0], s32 *, 0) & 4) {
            SetSpriteAnimation(effect_sprites[0]);
        }
        M2C_FIELD(effect_sprites[0], s32 *, 0x28) = (s32) (M2C_FIELD(effect_sprites[0], s32 *, 0x28) + (s16) beam_x_velocity_fixed8);
        temp_r1 = (s16) beam_offset_y_fixed8;
        if ((s32) temp_r1 < (s32)0xFFFFF000) {
            beam_offset_y_fixed8 = (s32) (u16) (temp_r1 + 0x40);
        } else {
            LoadZoidIconGraphics(0x87, 6, 0x40, 1);
            {
                register s32 constructor_sp0 asm("r2");
                register s32 constructor_arg9 asm("r0");

                asm volatile("ldr %0, [sp, #164]"
                             : "=r"(constructor_sp0)
                             : "g"(defeat_phase)
                             : "memory");
                asm volatile(
                    "str %0, [sp, #0]\n\t"
                    "mov r0, #64\n\t"
                    "str r0, [sp, #4]\n\t"
                    "mov r0, #1\n\t"
                    "str r0, [sp, #8]\n\t"
                    "mov r0, #178\n\t"
                    "lsl r0, r0, #2\n\t"
                    "str r0, [sp, #12]"
                    : "+r"(constructor_sp0)
                    :
                    : "r0", "memory");
                constructor_arg9 = 0x080BADD5;
                asm volatile("str %0, [sp, #16]"
                             : "+r"(constructor_arg9)
                             :
                             : "memory");
                temp_r0_5 = CreateSprite_4(
                    ({ register s32 arg asm("r0") = 0x0821024C;
                       asm volatile("" : "+r"(arg)); arg; }),
                    ({ register s32 arg asm("r1") = 0x08210258;
                       asm volatile("" : "+r"(arg)); arg; }),
                    0, 0);
            }
            gBattleUnitSprites[1] = temp_r0_5;
            M2C_FIELD(temp_r0_5, s32 *, 0x28) = (s32) (M2C_FIELD(effect_sprites[0], s32 *, 0x28) + 0x6000);
            M2C_FIELD(temp_r0_5, s32 *, 0x2C) = (s32) M2C_FIELD(effect_sprites[0], s32 *, 0x2C);
            M2C_FIELD(temp_r0_5, s32 *, 0x30) = (s32) M2C_FIELD(effect_sprites[0], s32 *, 0x30);
            gBattleUnitSpriteMotionStates[1] = (u8) defeat_phase;
            DestroySprite(effect_sprites[0]);
            defeat_phase = DEATH_METEOR_DEFEAT_GENO_FLAME_MOVING;
        }
        {
        register s32 *field_child asm("r1") = effect_sprites[0];

        asm volatile("" : "+r"(field_child));
        rounded_beam_offset_y = (s16) beam_offset_y_fixed8;
        if ((s32) rounded_beam_offset_y < 0) {
            rounded_beam_offset_y += 0xFF;
        }
        M2C_FIELD(field_child, s16 *, 0xA) = (s16) (rounded_beam_offset_y >> 8);
        }
        break;
    case DEATH_METEOR_DEFEAT_GENO_FLAME_MOVING:
        temp_r1_2 = gBattleUnitSprites[1];
        temp_r4 = (s16) beam_x_velocity_fixed8;
        M2C_FIELD(temp_r1_2, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r1_2, s32 *, 0x28) + temp_r4);
        temp_r0_6 = 0xFF & temp_r4;
        if (temp_r0_6 == 0) {
            {
                register s32 constructor_stack asm("r0") = temp_r0_6;

                asm volatile(
                    "str %0, [sp, #0]\n\t"
                    "mov %0, #128\n\t"
                    "lsl %0, %0, #1\n\t"
                    "str %0, [sp, #4]\n\t"
                    "mov %0, #4\n\t"
                    "str %0, [sp, #8]"
                    : "+r"(constructor_stack)
                    :
                    : "memory");
                constructor_stack = 0x85C0;
                asm volatile("str %0, [sp, #12]"
                             : "+r"(constructor_stack)
                             :
                             : "memory");
                constructor_stack = 0x080BADD5;
                asm volatile("str %0, [sp, #16]"
                             : "+r"(constructor_stack)
                             :
                             : "memory");
                temp_r0_7 = CreateSpriteFromTable_4(
                    ({ register s32 arg asm("r0") = 0x087AFA94;
                       asm volatile("" : "+r"(arg)); arg; }),
                    ({ register s32 arg asm("r1") = 4;
                       asm volatile("" : "+r"(arg)); arg; }),
                    0, 0);
            }
            effect_sprites[1] = temp_r0_7;
            temp_r2_2 = gBattleUnitSprites[1];
            M2C_FIELD(temp_r0_7, s32 *, 0x28) = (s32) (M2C_FIELD(temp_r2_2, s32 *, 0x28) + 0x1000);
            M2C_FIELD(temp_r0_7, s32 *, 0x2C) = (s32) M2C_FIELD(temp_r2_2, s32 *, 0x2C);
            M2C_FIELD(temp_r0_7, s32 *, 0x30) = (s32) M2C_FIELD(temp_r2_2, s32 *, 0x30);
            PlaySong(0x64);
        }
        beam_x_velocity_fixed8 = (s32) (u16) (temp_r4 - 0x80);
        camera_x_velocity_low16 = (u32) ((camera_x_velocity_low16 << 0x10) + 0xFF800000) >> 0x10;
        break;
    }
    gPerspectiveCamera.pose.position.world_x = (s32) (gPerspectiveCamera.pose.position.world_x + (s16) camera_x_velocity_low16);
    camera_depth_offset = gPerspectiveCamera.pose.position.depth_offset;
    if (camera_depth_offset > 0x8000) {
        gPerspectiveCamera.pose.position.depth_offset = (s32) (camera_depth_offset + 0xFFFFFE00);
    }
    if (beam_palette_update <= 0x10U) {
        beam_palette_color_index = 0;
        palette_next_outer_2 = beam_palette_update + 1;
        palette_max_2 = 0x1F;
        white_blend_strength = 0x10 - beam_palette_update;
        {
            register u8 *palette_input_2 asm("r0");
            palette_input_2 = (u8 *)0x020028A0;
            asm volatile("" : "+r"(palette_input_2));
            palette_input_2 -= 0x20;
            var_ip = palette_input_2;
        }
        do {
            asm volatile(
                ".syntax unified\n\t"
                "mov r1, %0\n\t"
                "ldrh r0, [r1]\n\t"
                "adds r2, %1, #0\n\t"
                "ands r2, r0\n\t"
                "lsls r0, r0, #16\n\t"
                "lsrs r5, r0, #21\n\t"
                "ands r5, %1\n\t"
                "lsrs r3, r0, #26\n\t"
                "ands r3, %1\n\t"
                "subs r0, %1, r2\n\t"
                "muls r0, %2\n\t"
                "lsrs r0, r0, #4\n\t"
                "adds r0, r2, r0\n\t"
                "lsls r0, r0, #24\n\t"
                "lsrs r2, r0, #24\n\t"
                "subs r0, %1, r5\n\t"
                "adds r1, r0, #0\n\t"
                "muls r1, %2\n\t"
                "lsrs r1, r1, #4\n\t"
                "adds r1, r5, r1\n\t"
                "lsls r1, r1, #24\n\t"
                "subs r0, %1, r3\n\t"
                "muls r0, %2\n\t"
                "lsrs r0, r0, #4\n\t"
                "adds r0, r3, r0\n\t"
                "lsls r0, r0, #24\n\t"
                "lsrs r1, r1, #19\n\t"
                "orrs r2, r1\n\t"
                "lsrs r0, r0, #14\n\t"
                "orrs r2, r0\n\t"
                "mov r3, %0\n\t"
                "strh r2, [r3, #32]\n\t"
                "movs r5, #2\n\t"
                "add %0, r5\n\t"
                ".syntax divided"
                : "+r"(var_ip)
                : "r"(palette_max_2), "r"(white_blend_strength)
                : "r0", "r1", "r2", "r3", "r5", "cc", "memory");
            beam_palette_color_index += 1;
        } while (beam_palette_color_index <= 0xFU);
        QueueCopy(0x020028A0, 0x05000200, 0x20);
        beam_palette_update = palette_next_outer_2;
    }
    YieldTaskForUpdates(1);
    temp_r3_3 = (s16) camera_x_velocity_low16;
    if (temp_r3_3 != 0) {
        goto loop_9;
    }
    portrait_sprite = CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, (s32) temp_r3_3);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x36, 1, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018A3A);
    {
        register s32 *flag_store asm("r2");
        s32 flag_value = *portrait_sprite | 0x20000;

        asm volatile("ldr %0, [sp, #148]"
                     : "=r"(flag_store)
                     : "g"(portrait_sprite)
                     : "memory");
        *flag_store = flag_value;
    }
    RunMenuScript(0x08017BE6);
    if (gPerspectiveCamera.pose.position.world_x > 0xFF) {
        do {
            camera_world_x = gPerspectiveCamera.pose.position.world_x;
            negative_camera_world_x = 0 - camera_world_x;
            if (negative_camera_world_x < 0) {
                negative_camera_world_x += 7;
            }
            gPerspectiveCamera.pose.position.world_x = (s32) (camera_world_x + (negative_camera_world_x >> 3));
            camera_depth_before_easing = gPerspectiveCamera.pose.position.depth_offset;
            camera_depth_delta = 0x10000 - camera_depth_before_easing;
            if (camera_depth_delta < 0) {
                camera_depth_delta += 7;
            }
            gPerspectiveCamera.pose.position.depth_offset = (s32) (camera_depth_before_easing + (camera_depth_delta >> 3));
            YieldTaskForUpdates(1);
        } while (gPerspectiveCamera.pose.position.world_x > 0xFF);
    }
    gPerspectiveCamera.pose.position.world_x = 0;
    gPerspectiveCamera.pose.position.depth_offset = 0x10000;
    camera_hold_updates = 0;
    do {
        YieldTaskForUpdates(1);
        camera_hold_updates += 1;
    } while (camera_hold_updates <= 0x1DU);
    StartTask(4, 0x080A876D);
    pulse_hold_updates = 0;
    do {
        YieldTaskForUpdates(1);
        pulse_hold_updates += 1;
    } while (pulse_hold_updates <= 0x3BU);
    LoadSpriteGraphicsFromTable(0x087AC9F8, 6, 0x40, 1);
    {
    register s32 *clear_zero asm("r1");

    clear_slot_index = 0;
    clear_zero = 0;
    asm volatile("" : "+r"(clear_zero));
    clear_slot = effect_sprites;
    do {
        *clear_slot = clear_zero;
        clear_slot += 1;
        clear_slot_index += 1;
    } while (clear_slot_index <= 0x1FU);
    }
    {
        register u32 outer_zero asm("r3") = 0;
        asm volatile("" : "+r"(outer_zero));
        explosion_elapsed_updates = outer_zero;
    }
    updates_since_explosion = 0;
    {
        register s32 countdown_seed asm("r5") = 0x12;
        asm volatile("" : "+r"(countdown_seed));
        explosion_spawn_interval = countdown_seed;
    }
loop_46:
    {
    register u32 outer_check asm("r0") = explosion_elapsed_updates;
    asm volatile("" : "+r"(outer_check));
    if (outer_check == 0xF0) {
        StopTask(4);
        DestroySprite(*(s32 **)0x02031C94);
        StartScreenTransition(SCREEN_TRANSITION_FADE_TO_WHITE, 0x3C);
    }
    }
    cleanup_slot_index = 0;
    {
        register u32 tail_seed asm("r1") = 1;
        asm volatile("" : "+r"(tail_seed));
        tail_seed += explosion_elapsed_updates;
        tail_next = tail_seed;
    }
    {
    register s32 cleanup_mask asm("r2") = 1;

    asm volatile("" : "+r"(cleanup_mask));
    cleanup_slot = effect_sprites;
    do {
        temp_r0_10 = *cleanup_slot;
        if (temp_r0_10 != 0) {
            effect_active_flag = *temp_r0_10 & cleanup_mask;
            if (effect_active_flag == 0) {
                *cleanup_slot = (s32 *) effect_active_flag;
            }
        }
        cleanup_slot += 1;
        cleanup_slot_index += 1;
    } while (cleanup_slot_index <= 0x1FU);
    }
    if ((updates_since_explosion == explosion_spawn_interval) || (explosion_spawn_interval == 0)) {
        explosion_slot_index = 0;
        rng = (s32 *)0x03000010;
        explosion_slot = effect_sprites;
loop_56:
        if (*explosion_slot == 0) {
            if (explosion_elapsed_updates <= 0x3BU) {
                explosion_animation = 2;
            } else {
                explosion_animation = (u32) (CallFunctionR0(*rng) * 3) >> 0xF;
            }
            temp_r4_2 = (s16) (((u32) (CallFunctionR0(*rng) * 0x41) >> 0xF) + 0x58);
            {
                register s32 spawn_y asm("r0") =
                    (s32) (s16) (0x58 - ((u32) (CallFunctionR0(*rng) * 0x41) >> 0xF));
                register u32 spawn_subtype asm("r2") = (u16) explosion_animation;

                asm volatile("" : "+r"(spawn_subtype));
                asm volatile(
                    "str %0, [sp, #0]\n\t"
                    "mov %0, #64\n\t"
                    "str %0, [sp, #4]\n\t"
                    "mov %0, #1\n\t"
                    "str %0, [sp, #8]\n\t"
                    "mov %0, #208\n\t"
                    "lsl %0, %0, #2\n\t"
                    "str %0, [sp, #12]\n\t"
                    "mov %0, #0\n\t"
                    "str %0, [sp, #16]"
                    : "+r"(spawn_y)
                    :
                    : "memory");
                *explosion_slot = CreateSpriteFromTable_4(0x087ACDD8, 6,
                    spawn_subtype, temp_r4_2);
            }
            PlaySong(0x5B);
        } else {
            explosion_slot += 1;
            explosion_slot_index += 1;
            if (explosion_slot_index <= 0x1FU) {
                goto loop_56;
            }
        }
        updates_since_explosion = 0;
        if (explosion_spawn_interval != 0) {
            explosion_spawn_interval -= 1;
        }
    } else {
        updates_since_explosion += 1;
    }
    explosion_elapsed_updates = tail_next;
    YieldTaskForUpdates(1);
    if (explosion_elapsed_updates <= 0x12BU) {
        goto loop_46;
    }
    {
    register s32 active_mask asm("r5");

    destroy_slot_index = 0;
    destroy_slot = effect_sprites;
    active_mask = 1;
    asm volatile("" : "+r"(active_mask));
    do {
        destroy_candidate = *destroy_slot;
        if (*destroy_candidate & active_mask) {
            DestroySprite(destroy_candidate);
        }
        destroy_slot += 1;
        destroy_slot_index += 1;
    } while (destroy_slot_index <= 0x1FU);
    }
    PlaySong(0x81);
    white_hold_updates = 0;
    do {
        YieldTaskForUpdates(1);
        white_hold_updates += 1;
    } while (white_hold_updates <= 0x1DU);
    DestroySprite(gBattleUnitSprites[0]);
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_WHITE, 0x1E);
    goto test_ready_2;
wait_ready_2:
    YieldTaskForUpdates(1);
test_ready_2:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_2;
    }
    {
        register s32 *flag_store_2 asm("r2");
        s32 flag_value_2 = *portrait_sprite & 0xFFFDFFFF;

        asm volatile("ldr %0, [sp, #148]"
                     : "=r"(flag_store_2)
                     : "g"(portrait_sprite)
                     : "memory");
        *flag_store_2 = flag_value_2;
    }
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(1, 5, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018A79);
    QueuePilotPortraitGraphics(3, 5, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018AD7);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    goto test_ready_3;
wait_ready_3:
    YieldTaskForUpdates(1);
test_ready_3:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_ready_3;
    }
    *(s32 *)0x02021690 = -1;
    YieldTaskForUpdates(1);
}
