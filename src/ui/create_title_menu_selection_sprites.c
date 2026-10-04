#include "m2c_prelude.h"
#include "title_menu.h"
s32 CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_8094484");

void CreateTitleMenuSelectionSprites(u8 selection_mode) asm("func_0809B46C");

void CreateTitleMenuSelectionSprites(u8 selection_mode) {
    register s32 selection_frames asm("r3");
    register s32 selection_animations asm("r1");

    selection_frames = 0x08103770;
    selection_animations = 0x08103794;
    *(s32 *)0x020216A4 = CreateSprite(selection_frames, selection_animations, selection_mode == 0, 0x5C, 0x50, 0x37D, 0xE, 0x210, 0);
    *(s32 *)0x020216A8 = CreateSprite(0x080ED8F8, 0x080ED92C, 0, 0x58, (s32) ((*(u8 *)0x020216AC << 0x14) + 0x580000) >> 0x10, 0x3EF, 0xF, 0x120, 0);
}
