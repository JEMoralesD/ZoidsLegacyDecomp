#include "m2c_prelude.h"
M2C_UNK func_080EB944(s32);
extern u8 gSongTable[];
extern s32 gMusicPlayerTable[];

void sub_08092E4C(s32 arg0) {
    s32 *table;
    u8 *entries;
    arg0 <<= 16;
    table = gMusicPlayerTable;
    entries = gSongTable;
    func_080EB944(table[*(u16 *)(entries + ((u32)arg0 >> 13) + 4) * 3]);
}
