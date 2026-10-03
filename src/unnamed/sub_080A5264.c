#include "m2c_prelude.h"
#include "../game/game_state.h"

extern void PlayOrContinueSong(u8) asm("func_08092E74");
extern void PlaySong(int) asm("func_08092E84");
extern void StopSong(u8) asm("func_08092EA0");
extern void DestroySprite(void) asm("func_08094554");
extern u8 func_08098B58(int);
extern void RunMenuScript(int) asm("func_08098BB4");
extern int SeekEventCommand(u8, int, int) asm("func_080A016C");
extern void func_080E7868(u8, s16);
extern void CopyBytes(void *, int, int) asm("func_080ED038");
extern void CopyString(void *, int) asm("func_080ED128");

extern s32 D_02031744;
extern u8 D_02030666;
extern u16 D_02031756[];
extern s32 D_087EF410[];

s32 sub_080A5264(u8 arg0, u8 **arg1) {
    u8 st;
    u8 idx;
    u8 *ptr;

    StopSong(*(u8 *)0x02030667);
    PlaySong(0x35);
    *(u8 *)0x02030664 = 1;
    if (D_02031744 != 0) {
        DestroySprite();
        D_02031744 = 0;
    }
    if ((*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) || (*(u8 *)0x02031748 != 0)) {
        st = D_02030666;
        if (st != 0) {
            if (st == 1) {
                RunMenuScript(0x080177F5);
                goto block_7;
            }
        } else {
block_7:
            RunMenuScript(0x080177ED);
            D_02030666 = 2;
        }
    }
    CopyBytes(D_02031756, 0x08103DD8, 0x15);
    D_02031756[10] = 0x201;
    CopyString(&D_02031756[11], D_087EF410[(*arg1)[1]]);
    idx = func_08098B58(D_087EF410[(*arg1)[1]]);
    D_02031756[idx + 11] = 1;
    CopyBytes(&D_02031756[12] + idx, 0x08103DF0, 0xE);
    *(u16 **)0x0200A888 = D_02031756;
    RunMenuScript(0x080177D5);
    ptr = *arg1;
    func_080E7868(ptr[1], (s16)(ptr[2] | (ptr[3] << 8)));
    StopSong(0x35);
    PlayOrContinueSong(*(u8 *)0x02030667);
    SeekEventCommand(arg0, -1, 0);
    return 0;
}
