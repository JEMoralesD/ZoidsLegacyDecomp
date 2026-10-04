#include "m2c_prelude.h"
#include "../../battle/battle_display.h"
#include "../../graphics/camera.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"

extern s32 gGameMode;
extern s32 gBattlePhase;
extern struct BattleSceneSetupView gBattleSetup;
extern struct PerspectiveCamera gPerspectiveCamera asm("D_030033C4");
extern volatile u16 gBlendControlShadow asm("D_0300004E");
extern volatile u16 gBlendAlphaShadow asm("D_03000050");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern struct SceneProjectedSpriteView *gBattleUnitSprites[2] asm("D_02032E8C");
extern u8 gBattleUnitSpriteMotionStates[2] asm("D_02032EEC");
extern struct SceneScanlineEventView gSceneTopScanlineEvent asm("D_020314A4");
extern struct SceneScanlineEventView gSceneBottomScanlineEvent asm("D_020314B4");

void ResetScanlineEvents(void) asm("func_0809258C");
void InsertScanlineEvent(void) asm("func_080925A4");
void PlaySong(s32) asm("func_08092E84");
void ClearSpritePools(void) asm("func_08094330");
struct SceneProjectedSpriteView *CreateSpriteFromTable(s32, s32, s32, s32, s32,
    s32, s32, s32, s32) asm("func_08094374");
struct SceneProjectedSpriteView *CreateSprite(s32, s32, s32, s32, s32,
    s32, s32, s32, s32) asm("func_08094484");
void StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void RunMenuScript(s32) asm("func_08098BB4");
void LoadZoidBodyGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A1F8");
void LoadZoidIconGraphics(s32, s32, s32, s32) asm("func_0809A4CC");
void QueueZoidIconGraphics(s32, s32, s32, s32, s32) asm("func_0809A52C");
void LoadSceneBackgroundGraphics(u8, s32, s32, s32, s32) asm("func_0809A5B4");
void QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8");
void LoadSpriteGraphicsFromTable(s32, s32, s32, s32) asm("func_0809AA64");
void ResetBattleAnimation(s32) asm("func_080D0AF0");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void PlayZeroPanzerLaunchScene(void) asm("func_080A7D28");

void PlayZeroPanzerLaunchScene(void)
{
    struct SceneProjectedSpriteView *sprites[8];
    s32 task_mask;
    s32 camera_x_velocity_fixed8;
    s32 zoid_offset_y_fixed8;
    s32 zoid_y_velocity_fixed8;
    s32 whale_x_velocity_fixed8;
    u8 delay_updates;
    u8 effect_sprite_index;

    gGameMode = GAME_MODE_BATTLE;
    gBattlePhase = 0xFF10;
    gBattleSetup.reserved01 = 0;
    gPerspectiveCamera.pose.position.world_x = 0xFFFFC000;
    gPerspectiveCamera.pose.position.world_z = 0;
    gPerspectiveCamera.pose.position.depth_offset = 0x8000;
    gPerspectiveCamera.pose.orientation.angles.pitch = 0x20;
    gPerspectiveCamera.pose.orientation.angles.roll = 0;
    gPerspectiveCamera.pose.orientation.angles.yaw = 0;
    gPerspectiveCamera.projection.screen_center_x = 0x78;
    gPerspectiveCamera.projection.screen_center_y = 0x78;
    gPerspectiveCamera.projection.focal_length = 0x80;
    gPerspectiveCamera.far_clip_depth = 0x20000;
    YieldTaskForUpdates(1);
    LoadZoidIconGraphics(0x95, 0, 0, 0);

    {
        struct SceneProjectedSpriteView *sprite = CreateSprite(
            0x0821024C, 0x08210258, 0, 0, 0, 0, 0, 0x2C8, 0x080BADD5);
        gBattleUnitSprites[0] = sprite;
        sprite->world_x_fixed8 = 0;
        sprite->world_y_fixed8 = 0;
        sprite->world_z_fixed8 = 0;
        sprite->offset_y = -0x20;
    }
    gBattleUnitSpriteMotionStates[0] = 0;
    LoadSpriteGraphicsFromTable(0x087AF9D4, 7, 0x40, 1);

    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    gBlendControlShadow = 0x740;
    gBlendAlphaShadow = 0x810;
    delay_updates = 0;
    do {
        YieldTaskForUpdates(1);
        delay_updates++;
    } while (delay_updates <= 0xF);

    sprites[0] = CreateSpriteFromTable(0x087AFA94, 7, 3, 0, 0, 0x40, 1,
        0x7C0, 0x080BADD5);
    sprites[0]->world_x_fixed8 = 0x4000;
    sprites[0]->world_y_fixed8 = 0;
    sprites[0]->world_z_fixed8 = 0;
    sprites[0]->offset_y = -0x40;
    camera_x_velocity_fixed8 = -0x1000;
    PlaySong(0x44);
    while (sprites[0]->world_x_fixed8 > -0x10000) {
        gPerspectiveCamera.pose.position.world_x += camera_x_velocity_fixed8;
        sprites[0]->world_x_fixed8 -= 0x2000;
        YieldTaskForUpdates(1);
    }

    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        camera_x_velocity_fixed8 = (s32)(camera_x_velocity_fixed8 + ((u32)camera_x_velocity_fixed8 >> 31)) >> 1;
        gPerspectiveCamera.pose.position.world_x += camera_x_velocity_fixed8;
        sprites[0]->world_x_fixed8 -= 0x2000;
        YieldTaskForUpdates(1);
    }

    delay_updates = 0;
    do {
        YieldTaskForUpdates(1);
        delay_updates++;
    } while (delay_updates <= 0x1D);

    gBattleSetup.terrain_id = 0xFF;
    gGameMode = GAME_MODE_BATTLE_SCENE;
    YieldTaskForUpdates(1);
    ClearSpritePools();
    LoadZoidBodyGraphics(0x1C, 0, 1, 0, 0, 0x02002880);
    ResetBattleAnimation(0xFFFFFF00);
    SetBattleAnimationCameraMode(0xE, 0);
    LoadSceneBackgroundGraphics(gBattleSetup.terrain_id, 2, 3, 2, 1);

    {
        register struct SceneScanlineEventView *task asm("r0") = &gSceneTopScanlineEvent;
        register u8 flags asm("r2");

        task->scanline = 0x5F;
        flags = task->flags;
        task_mask = -2;
        task->flags = task_mask & flags;
        task->reserved02 = 0;
        task->callback = 0x080A0099;
        task->previous = 0;
        task->next = 0;
    }
    InsertScanlineEvent();

    gSceneBottomScanlineEvent.scanline = 0xA0;
    gSceneBottomScanlineEvent.flags &= task_mask;
    gSceneBottomScanlineEvent.reserved02 = 0;
    gSceneBottomScanlineEvent.callback = 0x080A00D5;
    gSceneBottomScanlineEvent.previous = 0;
    gSceneBottomScanlineEvent.next = 0;
    InsertScanlineEvent();

    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    {
        s32 *scroll = gFieldCameraScrollOffsets;
        s32 finished = 0x1000;
wait_scroll:
        if (*scroll != finished) {
            YieldTaskForUpdates(1);
            goto wait_scroll;
        }
    }

    ResetScanlineEvents();
    CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2,
        0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x1E, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018969);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    gGameMode = GAME_MODE_BATTLE;
    gBattlePhase = 0xFF10;
    gBattleSetup.reserved01 = 0;
    gPerspectiveCamera.pose.position.world_x = 0;
    gPerspectiveCamera.pose.position.world_z = 0;
    gPerspectiveCamera.pose.position.depth_offset = 0x8000;
    gPerspectiveCamera.pose.orientation.angles.pitch = 0x20;
    gPerspectiveCamera.pose.orientation.angles.roll = 0;
    gPerspectiveCamera.pose.orientation.angles.yaw = 0;
    gPerspectiveCamera.projection.screen_center_x = 0x78;
    gPerspectiveCamera.projection.screen_center_y = 0x78;
    gPerspectiveCamera.projection.focal_length = 0x80;
    gPerspectiveCamera.far_clip_depth = 0x20000;
    YieldTaskForUpdates(1);
    LoadZoidIconGraphics(0x95, 0, 0, 0);

    {
        struct SceneProjectedSpriteView *sprite = CreateSprite(
            0x0821024C, 0x08210258, 0, 0, 0, 0, 0, 0x2C8, 0x080BADD5);
        gBattleUnitSprites[0] = sprite;
        sprite->world_x_fixed8 = 0;
        sprite->world_y_fixed8 = 0;
        sprite->world_z_fixed8 = 0;
        sprite->offset_y = -0x20;
    }
    gBattleUnitSpriteMotionStates[0] = 0;
    LoadSpriteGraphicsFromTable(0x087AF9D4, 4, 0x80, 2);

    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    delay_updates = 0;
    do {
        YieldTaskForUpdates(1);
        delay_updates++;
    } while (delay_updates <= 0xF);

    LoadSpriteGraphicsFromTable(0x087AF9D4, 8, 0x40, 1);
    {
        struct SceneProjectedSpriteView *sprite = CreateSprite(
            0x0821024C, 0x08210258, 0, 0, 0, 0x40, 1,
            0x3C8, 0x080BAE61);
        gBattleUnitSprites[1] = sprite;
        sprite->world_x_fixed8 = 0;
        sprite->world_y_fixed8 = 0;
        sprite->world_z_fixed8 = 0;
    }
    gBattleUnitSpriteMotionStates[1] = 0;

    zoid_offset_y_fixed8 = -0x3000;
    zoid_y_velocity_fixed8 = 0;
    do {
        zoid_y_velocity_fixed8 += 0x80;
        zoid_offset_y_fixed8 += zoid_y_velocity_fixed8;
        if (zoid_offset_y_fixed8 > 0) {
            zoid_offset_y_fixed8 = 0;
        }
        {
            struct SceneProjectedSpriteView *sprite = gBattleUnitSprites[1];
            register s32 rounded asm("r0") = zoid_offset_y_fixed8;

            if (zoid_offset_y_fixed8 < 0) {
                rounded += 0xFF;
            }
            sprite->offset_y = rounded >> 8;
        }
        YieldTaskForUpdates(1);
    } while (zoid_offset_y_fixed8 != 0);

    QueueZoidIconGraphics(0x1C, 0, 0x40, 1, 0x02002880);
    effect_sprite_index = 0;
    do {
        sprites[effect_sprite_index] = CreateSpriteFromTable(0x087AFA94, 4, 0, 0, 0,
            0x80, 2, 0x3C0, 0x080BAE61);
        sprites[effect_sprite_index]->world_x_fixed8 = (effect_sprite_index << 11) - 0x1000;
        sprites[effect_sprite_index]->world_y_fixed8 = 0;
        sprites[effect_sprite_index]->world_z_fixed8 = 0;
        effect_sprite_index++;
    } while (effect_sprite_index <= 4);

    PlaySong(0x4E);
    do {
        YieldTaskForUpdates(1);
        effect_sprite_index = 0;
        while (effect_sprite_index <= 3 && !(sprites[effect_sprite_index]->flags & 1)) {
            effect_sprite_index++;
        }
    } while (effect_sprite_index <= 3);

    PlaySong(0x7E);
    whale_x_velocity_fixed8 = 0;
    while (gBattleUnitSprites[0]->world_x_fixed8 > -0x10000) {
        if (whale_x_velocity_fixed8 > -0x200) {
            whale_x_velocity_fixed8 -= 0x10;
        }
        gBattleUnitSprites[0]->world_x_fixed8 += whale_x_velocity_fixed8;
        YieldTaskForUpdates(1);
    }

    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    gGameMode = -1;
    YieldTaskForUpdates(1);
}
