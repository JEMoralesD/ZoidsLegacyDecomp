#include "m2c_prelude.h"
u8 UpdatePerspectiveScanlineBuffers() asm("func_08093B7C");
M2C_UNK SetDisplayWindowBounds(s32, s32, s32, s32) asm("func_0809544C");
M2C_UNK UpdateBattleUnitSpriteMotion() asm("func_080BB0B0");
M2C_UNK UpdateBattleCameraTransition() asm("func_080BB474");
M2C_UNK UpdateCameraRotationMatrix() asm("func_080BB764");
M2C_UNK YieldTaskForUpdates(s32) asm("func_080ED17C");

extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern s32 gPerspectiveCamera asm("D_030033C4");

void RunBattleCameraTask(void) asm("func_080BB8EC");

void RunBattleCameraTask(void) {
    register s32 *bg_scroll asm("r5") = gFieldCameraScrollOffsets;
    u8 far_clip_row;
    s32 world_x;
    s32 window_bottom;

loop_1:
    UpdateBattleCameraTransition();
    far_clip_row = UpdatePerspectiveScanlineBuffers();
    UpdateCameraRotationMatrix();
    world_x = gPerspectiveCamera;
    if (world_x < 0) {
        world_x += 3;
    }
    bg_scroll[0] = world_x >> 2;
    bg_scroll[1] = (0x80 - far_clip_row) << 8;
    window_bottom = 0xFF00;
    if (far_clip_row != 0xFF) {
        window_bottom = far_clip_row;
    }
    SetDisplayWindowBounds(0xF0, window_bottom, 0, 0);
    UpdateBattleUnitSpriteMotion();
    YieldTaskForUpdates(1);
    goto loop_1;
}
