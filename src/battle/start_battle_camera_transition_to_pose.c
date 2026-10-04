#include "m2c_prelude.h"
#include "../graphics/camera.h"
#include "battle_display.h"
extern u8 gBattleCameraMode asm("D_02032EF9");
extern struct CameraPose gBattleCameraTarget asm("D_02032EFC");
extern struct CameraPose gPerspectiveCamera asm("D_030033C4");
extern u8 gBattleCameraTransitionComplete asm("D_02032F62");

void StartBattleCameraTransitionToPose(struct CameraWorldOffset *position, s32 *orientation_words, u8 immediate) asm("func_080BB424");

void StartBattleCameraTransitionToPose(struct CameraWorldOffset *position, s32 *orientation_words, u8 immediate) {
    s32 pitch_yaw, roll_padding;
    gBattleCameraMode = BATTLE_CAMERA_KEEP_TARGET;
    gBattleCameraTarget.position = *position;
    pitch_yaw = orientation_words[0];
    roll_padding = orientation_words[1];
    gBattleCameraTarget.orientation.words.pitch_yaw = pitch_yaw;
    gBattleCameraTarget.orientation.words.roll_padding = roll_padding;
    if (immediate == 1) {
        s32 current_pitch_yaw, current_roll_padding;
        gPerspectiveCamera.position = gBattleCameraTarget.position;
        current_pitch_yaw = gBattleCameraTarget.orientation.words.pitch_yaw;
        current_roll_padding = gBattleCameraTarget.orientation.words.roll_padding;
        gPerspectiveCamera.orientation.words.pitch_yaw = current_pitch_yaw;
        gPerspectiveCamera.orientation.words.roll_padding = current_roll_padding;
    }
    gBattleCameraTransitionComplete = 0;
}
