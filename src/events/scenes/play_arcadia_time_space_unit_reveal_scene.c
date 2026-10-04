#include "m2c_prelude.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"

M2C_UNK ResetScanlineEvents() asm("func_0809258C");                            /* extern */
M2C_UNK InsertScanlineEvent() asm("func_080925A4");                            /* extern */
M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                /* extern */
M2C_UNK StopTask(s32) asm("func_08092E0C");                         /* extern */
M2C_UNK PlaySong(s32) asm("func_08092E84");                         /* extern */
M2C_UNK StopSong(s32) asm("func_08092EA0");                         /* extern */
M2C_UNK ClearSpritePools() asm("func_08094330");                            /* extern */
void *CreateSprite(M2C_UNK, M2C_UNK, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484"); /* extern */
M2C_UNK DestroySprite() asm("func_08094554");                            /* extern */
M2C_UNK StartScreenTransition(s32, s32) asm("func_08096308");                    /* extern */
s32 IsScreenTransitionComplete() asm("func_0809669C");                                /* extern */
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");                     /* extern */
M2C_UNK LoadZoidBodyGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A1F8"); /* extern */
M2C_UNK LoadSceneBackgroundGraphics(u8, s32, s32, s32, s32) asm("func_0809A5B4");      /* extern */
M2C_UNK QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8"); /* extern */
M2C_UNK LoadSpriteGraphicsFromTable(M2C_UNK, s32, s32, s32) asm("func_0809AA64");      /* extern */
M2C_UNK ResetBattleAnimation(M2C_UNK) asm("func_080D0AF0");                     /* extern */
M2C_UNK SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");                    /* extern */
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");                         /* extern */

asm(".set sub_080A83C8_state, 0x0203055C");
asm(".set func_08094484_4, func_08094484");
extern u8 gArcadiaSceneBattleSetup asm("sub_080A83C8_state");
void *CreateSprite_4(M2C_UNK, M2C_UNK, s32, s32) asm("func_08094484_4");

void PlayArcadiaTimeSpaceUnitRevealScene(void) asm("func_080A83C8");

void PlayArcadiaTimeSpaceUnitRevealScene(void) {
    s32 unit_camera_world_z;
    s32 rounded_camera_world_z;
    s32 next_camera_world_z;
    u16 *tile_word_cursor;
    u16 tile_word;
    u16 tile_word_index;
    u16 streak_slot_index;
    u16 flash_or_wait_update;
    void *arcadia_unit_sprite;
    register void *scanline_event_address asm("r0");
    s32 zero;
    s32 clear_mask;
    s32 *streak_sprite_slots;
    register u8 *camera_address asm("r6");
    register s32 object_zero asm("r5");

    {
        u8 *battle_setup_address = &gArcadiaSceneBattleSetup;
        asm volatile("" : "+r"(battle_setup_address));
        tile_word_index = 0;
        battle_setup_address[2] = 0xD;
    }
    *(s32 *)0x02021690 = GAME_MODE_BATTLE_SCENE;
    YieldTaskForUpdates(1);
    ClearSpritePools();
    LoadZoidBodyGraphics(0x74, 0, 1, 0, 0, 0x02002880);
    tile_word_cursor = (u16 *)0x06004000;
    do {
        tile_word = *tile_word_cursor;
        if ((tile_word & 0xFF00) == 0x1000) {
            tile_word &= 0xFF;
        }
        if ((tile_word & 0xFF) == 0x10) {
            tile_word &= 0xFF00;
        }
        *tile_word_cursor = tile_word;
        tile_word_cursor += 1;
        tile_word_index += 1;
    } while ((u32) tile_word_index <= 0x1FFFU);
    ResetBattleAnimation(0xFFFFFF00);
    SetBattleAnimationCameraMode(0x10, 0);
    {
        u8 *battle_setup_address = &gArcadiaSceneBattleSetup;
        asm volatile("" : "+r"(battle_setup_address));
        LoadSceneBackgroundGraphics(battle_setup_address[2], 2, 3, 2, 1);
    }
    LoadSpriteGraphicsFromTable(0x087AC9F8, 0x77, 0, 0);
    scanline_event_address = (void *)0x020314A4;
    zero = 0;
    M2C_FIELD(scanline_event_address, s8 *, 0) = 0x7F;
    {
    register u32 config_flags asm("r2") = M2C_FIELD(scanline_event_address, u8 *, 1);
    asm volatile("" : "+r"(config_flags));
    clear_mask = 2;
    asm volatile("" : "+r"(clear_mask));
    clear_mask = -clear_mask;
    {
        register u32 config_value asm("r1") = clear_mask;
        asm volatile("" : "+r"(config_value));
        config_value &= config_flags;
        M2C_FIELD(scanline_event_address, u8 *, 1) = config_value;
    }
    }
    M2C_FIELD(scanline_event_address, s16 *, 2) = zero;
    M2C_FIELD(scanline_event_address, s32 *, 4) = 0x080A0099;
    M2C_FIELD(scanline_event_address, s32 *, 0xC) = zero;
    M2C_FIELD(scanline_event_address, s32 *, 8) = zero;
    InsertScanlineEvent();
    scanline_event_address = (void *)0x020314B4;
    M2C_FIELD(scanline_event_address, u8 *, 0) = 0xA0;
    clear_mask &= M2C_FIELD(scanline_event_address, u8 *, 1);
    M2C_FIELD(scanline_event_address, u8 *, 1) = clear_mask;
    M2C_FIELD(scanline_event_address, s16 *, 2) = zero;
    M2C_FIELD(scanline_event_address, s32 *, 4) = 0x080A00D5;
    M2C_FIELD(scanline_event_address, s32 *, 0xC) = zero;
    M2C_FIELD(scanline_event_address, s32 *, 8) = zero;
    InsertScanlineEvent();
    StartTask(4, 0x080A829D);
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    {
    register s32 scene_x asm("r5") = 0x3C2;
    s32 scene_y = 0xD;
    register s32 *out_args asm("sp");
    asm volatile("" : "+r"(scene_x), "+r"(scene_y));
    for (;;) {
        YieldTaskForUpdates(1);
        {
            register s32 *wait_ptr asm("r0") = (s32 *)0x02031C10;
            register s32 wait_value asm("r1");
            register s32 wait_target asm("r0");
            asm volatile("" : "+r"(wait_ptr));
            wait_value = *wait_ptr;
            asm volatile("" : "+r"(wait_value));
            wait_target = 0x96;
            asm volatile("" : "+r"(wait_target));
            wait_target <<= 1;
            if (wait_value == wait_target) {
                break;
            }
        }
    }
    ResetScanlineEvents();
    out_args[0] = 0x68;
    out_args[1] = scene_x;
    out_args[2] = scene_y;
    out_args[3] = 8;
    out_args[4] = 0;
    CreateSprite_4(0x08359850, 0x0835985C, 0, 8);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(1, 0, 0, scene_x, scene_y, 0x02002880);
    RunMenuScript(0x080189C6);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    }
    goto poll_11;
wait_11:
    YieldTaskForUpdates(1);
poll_11:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_11;
    }
    StopTask(4);
    streak_slot_index = 0;
    streak_sprite_slots = (s32 *)0x02031C14;
    do {
        if (streak_sprite_slots[streak_slot_index] != 0) {
            DestroySprite();
        }
        streak_slot_index += 1;
    } while ((u32) streak_slot_index <= 0x1FU);
    *(s32 *)0x02021690 = GAME_MODE_BATTLE;
    *(s32 *)0x02030558 = 0xFF10;
    {
        register u8 *battle_setup_address asm("r0") = &gArcadiaSceneBattleSetup;
        s32 motion_zero = 0;
        asm volatile("" : "+r"(battle_setup_address), "+r"(motion_zero));
        battle_setup_address[1] = motion_zero;
    camera_address = (u8 *)0x030033C4;
    M2C_FIELD(camera_address, s32 *, 0) = motion_zero;
    M2C_FIELD(camera_address, s32 *, 4) = 0x20000;
    M2C_FIELD(camera_address, s32 *, 8) = 0x8000;
    object_zero = 0;
    asm volatile("" : "+r"(object_zero));
    M2C_FIELD(camera_address, s16 *, 0xC) = 0x10;
    M2C_FIELD(camera_address, s16 *, 0x10) = motion_zero;
    M2C_FIELD(camera_address, s16 *, 0xE) = motion_zero;
    M2C_FIELD(camera_address, s32 *, 0x14) = 0x78;
    M2C_FIELD(camera_address, s32 *, 0x18) = 0x58;
    M2C_FIELD(camera_address, s32 *, 0x1C) = 0x80;
    M2C_FIELD(camera_address, s32 *, 0x60) = 0x20000;
    YieldTaskForUpdates(1);
    LoadSpriteGraphicsFromTable(0x087AF9D4, 0x13, 0, 0);
    arcadia_unit_sprite = CreateSprite(0x0821024C, 0x08210258, 0, 0, motion_zero, motion_zero, motion_zero, 0x2C8, 0x080BADD5);
    *(void **)0x02032E8C = arcadia_unit_sprite;
    M2C_FIELD(arcadia_unit_sprite, s32 *, 0x28) = 0xFFFFD000;
    M2C_FIELD(arcadia_unit_sprite, s32 *, 0x2C) = motion_zero;
    M2C_FIELD(arcadia_unit_sprite, s32 *, 0x30) = motion_zero;
    *(s8 *)0x02032EEC = object_zero;
    }
    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    if (M2C_FIELD(camera_address, s32 *, 4) != 0) {
        register u8 *camera_world_z_check asm("r5") = camera_address;
        u8 *camera_world_z_store = camera_world_z_check;
        asm volatile("" : "+r"(camera_world_z_check), "+r"(camera_world_z_store));
        do {
            unit_camera_world_z = M2C_FIELD(camera_world_z_store, s32 *, 4);
            if (unit_camera_world_z <= 0x7FFF) {
                rounded_camera_world_z = unit_camera_world_z;
                if (unit_camera_world_z < 0) {
                    rounded_camera_world_z = unit_camera_world_z + 7;
                }
                next_camera_world_z = unit_camera_world_z - (rounded_camera_world_z >> 3);
            } else {
                next_camera_world_z = unit_camera_world_z + 0xFFFFF000;
            }
            M2C_FIELD(camera_world_z_store, s32 *, 4) = next_camera_world_z;
            {
                register u8 *camera_world_z_limit_address asm("r1") = camera_world_z_check;
                asm volatile("" : "+r"(camera_world_z_limit_address));
                if ((s32) M2C_FIELD(camera_world_z_limit_address, s32 *, 4) <= 0x3F) {
                    M2C_FIELD(camera_world_z_limit_address, s32 *, 4) = 0;
                }
            }
            YieldTaskForUpdates(1);
        } while (M2C_FIELD(camera_world_z_check, s32 *, 4) != 0);
    }
    StartTask(5, 0x0809FF55);
    PlaySong(0x56);
    flash_or_wait_update = 0;
    do {
        *(s16 *)0x0300004E = 0xBF;
        *(s16 *)0x03000052 = 0x10;
        YieldTaskForUpdates(1);
        *(s16 *)0x03000052 = 0;
        YieldTaskForUpdates(1);
        flash_or_wait_update += 1;
    } while ((u32) flash_or_wait_update <= 2U);
    *(s16 *)0x0300004E = 0;
    PlaySong(0x4A);
    flash_or_wait_update = 0;
    do {
        YieldTaskForUpdates(1);
        flash_or_wait_update += 1;
    } while ((u32) flash_or_wait_update <= 0x77U);
    StopSong(0x4A);
    StopTask(5);
    CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(1, 6, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x0801899A);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    goto poll_34;
wait_34:
    YieldTaskForUpdates(1);
poll_34:
    if ((IsScreenTransitionComplete() << 0x18) == 0) {
        goto wait_34;
    }
    *(s32 *)0x02021690 = -1;
    YieldTaskForUpdates(1);
}
