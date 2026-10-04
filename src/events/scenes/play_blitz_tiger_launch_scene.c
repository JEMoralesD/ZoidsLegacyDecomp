#include "m2c_prelude.h"
#include "../../battle/battle_display.h"
#include "../../graphics/camera.h"
#include "../../graphics/screen_effects.h"
#include "../../game/game_state.h"

extern s32 gGameMode;
extern s32 gBattlePhase;
extern struct BattleSceneSetupView gBattleSetup;
extern struct PerspectiveCamera gPerspectiveCamera asm("D_030033C4");
extern struct BattleDisplaySprite *gBattleUnitSprites asm("D_02032E8C");
extern u8 gBattleUnitSpriteMotionStates asm("D_02032EEC");
extern struct SceneScanlineEventView gSceneTopScanlineEvent asm("D_020314A4");
extern struct SceneScanlineEventView gSceneBottomScanlineEvent asm("D_020314B4");
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");

void ResetScanlineEvents(void) asm("func_0809258C");
void InsertScanlineEvent(void) asm("func_080925A4");
void StopTask(s32) asm("func_08092E0C");
void ClearSpritePools(void) asm("func_08094330");
struct BattleDisplaySprite *CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
void StartScreenTransition(s32, s32) asm("func_08096308");
s32 IsScreenTransitionComplete(void) asm("func_0809669C");
void RunMenuScript(s32) asm("func_08098BB4");
void LoadZoidBodyGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A1F8");
void LoadZoidIconGraphics(s32, s32, s32, s32) asm("func_0809A4CC");
void LoadSceneBackgroundGraphics(u8, s32, s32, s32, s32) asm("func_0809A5B4");
void QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8");
void ResetBattleAnimation(s32) asm("func_080D0AF0");
void SetBattleAnimationCameraMode(s32, s32) asm("func_080D12A0");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

void PlayBlitzTigerLaunchScene(void) asm("func_080A7A98");

void PlayBlitzTigerLaunchScene(void)
{
    struct BattleDisplaySprite *sprite;
    s32 task_mask;

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

    sprite = CreateSprite(0x0821024C, 0x08210258, 0, 0, 0, 0, 0,
        0x2C8, 0x080BADD5);
    gBattleUnitSprites = sprite;
    sprite->user_data.position.x = 0;
    sprite->user_data.position.y = 0;
    sprite->user_data.position.z = 0;
    sprite->offset_y = -0x20;
    gBattleUnitSpriteMotionStates = 0;

    StartScreenTransition(SCREEN_TRANSITION_FADE_FROM_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(0x31, 0, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x080188CF);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    gBattleSetup.terrain_id = 0xFF;
    gGameMode = GAME_MODE_BATTLE_SCENE;
    YieldTaskForUpdates(1);
    ClearSpritePools();
    LoadZoidBodyGraphics(0x74, 0, 1, 0, 0, 0x02002880);
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
    gSceneBottomScanlineEvent.flags = task_mask & gSceneBottomScanlineEvent.flags;
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

    CreateSprite(0x08359850, 0x0835985C, 0, 8, 0x68, 0x3C2, 0xD, 8, 0);
    RunMenuScript(0x08017BD3);
    QueuePilotPortraitGraphics(1, 3, 0, 0x3C2, 0xD, 0x02002880);
    RunMenuScript(0x08018935);
    StartScreenTransition(SCREEN_TRANSITION_FADE_TO_BLACK, 0x10);
    while ((IsScreenTransitionComplete() << 24) == 0) {
        YieldTaskForUpdates(1);
    }

    StopTask(7);
    gGameMode = -1;
    YieldTaskForUpdates(1);
}
