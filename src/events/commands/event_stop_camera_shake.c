#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../game/game_state.h"
extern int StopTask() asm("func_08092E0C");
extern int SeekEventCommand() asm("func_80A016C");

extern s32 gGameMode;
extern s32 gFieldCameraScrollOffsets[] asm("D_03000054");
extern s32 gEventCameraShakeXFixed8 asm("D_02031980");
extern s32 gEventCameraShakeYFixed8 asm("D_02031984");
extern u8  gEventMapId;
extern s32 gFieldBackgroundTilemaps[] asm("D_02032E88");
extern u16 gFieldMapDimensions[] asm("D_020324A4");
extern s32 gPerspectiveCamera[] asm("D_030033C4");

int EventStopCameraShake(u8 script_slot) asm("func_080A226C");

int EventStopCameraShake(u8 script_slot) {
    s32 scroll_x_fixed8, scroll_y_fixed8, scroll_limit_fixed8;

    StopTask(5);
    if (gGameMode == GAME_MODE_FIELD) {
        scroll_x_fixed8 = gFieldCameraScrollOffsets[0] - gEventCameraShakeXFixed8;
        gFieldCameraScrollOffsets[0] = scroll_x_fixed8;
        gFieldCameraScrollOffsets[1] = gFieldCameraScrollOffsets[1] - gEventCameraShakeYFixed8;
        if (gEventMapId != 0 && gFieldBackgroundTilemaps[1] != 0) {
            if (scroll_x_fixed8 < 0) {
                gFieldCameraScrollOffsets[0] = 0;
            } else {
                scroll_limit_fixed8 = (gFieldMapDimensions[0] << 11) - 0xF000;
                if (scroll_x_fixed8 > scroll_limit_fixed8) {
                    gFieldCameraScrollOffsets[0] = scroll_limit_fixed8;
                }
            }
            scroll_y_fixed8 = gFieldCameraScrollOffsets[1];
            if (scroll_y_fixed8 < 0) {
                gFieldCameraScrollOffsets[1] = 0;
            } else {
                scroll_limit_fixed8 = (gFieldMapDimensions[1] << 11) - 0xA000;
                if (scroll_y_fixed8 > scroll_limit_fixed8) {
                    gFieldCameraScrollOffsets[1] = scroll_limit_fixed8;
                }
            }
            gFieldCameraScrollOffsets[2] = gFieldCameraScrollOffsets[0];
            gFieldCameraScrollOffsets[3] = gFieldCameraScrollOffsets[1];
        }
    } else {
        gPerspectiveCamera[6] = 0x58;
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
