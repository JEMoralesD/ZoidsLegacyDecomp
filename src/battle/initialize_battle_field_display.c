#include "m2c_prelude.h"
extern u8 gBattleSetup[];
M2C_UNK StartTask(s32, M2C_UNK) asm("func_08092D8C");                    /* extern */
M2C_UNK ClearSpritePools() asm("func_8094330");                                /* extern */
M2C_UNK ConfigureDisplayWindows(s32, s32, s32, s32, s32, s32, s32, s32) asm("func_0809538C"); /* extern */
M2C_UNK InitializeWindowGraphics(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08096FBC"); /* extern */
M2C_UNK LoadBattleTerrainAndSkyGraphics(u8, s32, s32, s32, s32) asm("func_0809A848");          /* extern */
M2C_UNK CreateBattleUnitSprites() asm("func_080BAC54");                                /* extern */
M2C_UNK ResetBattleUnitPopupSlots() asm("func_080C9EE0");                                /* extern */
M2C_UNK BiosCpuFastSet(M2C_UNK, M2C_UNK, s32) asm("func_80ECD28");           /* extern */

void InitializeBattleFieldDisplay(void) asm("func_080BB940");

void InitializeBattleFieldDisplay(void) {
    *(s16 *)0x0300004C = 0x1741;
    M2C_FIELD((void *)0x04000008, s16 *, 0) = 0x87;
    M2C_FIELD((void *)0x04000008, s16 *, 4) = 0x218B;
    ClearSpritePools();
    CreateBattleUnitSprites();
    InitializeWindowGraphics(1, 2, 0x80, 0x340, 0x3C0, 2, 0xE, 0, 0x3E6, 0xF);
    LoadBattleTerrainAndSkyGraphics(gBattleSetup[1], 2, 1, 1, 0);
    ConfigureDisplayWindows(1, 0, 0, 0, 0, 0, 0x3B, 0x3E);
    BiosCpuFastSet(0x08277CE4, 0x050003C0, 8);
    ResetBattleUnitPopupSlots();
    *(s8 *)0x03000075 = 2;
    *(s8 *)0x0300603D = 2;
    StartTask(7, 0x080BB8ED);
}
