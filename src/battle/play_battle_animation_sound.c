#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"

M2C_UNK PlaySong(u16) asm("func_08092E84");
extern u8 gBattleAnimationSoundSlots[] asm("D_020348D4");

M2C_UNK PlayBattleAnimationSound(s32 sound_slot) asm("func_080D2790");

M2C_UNK PlayBattleAnimationSound(s32 sound_slot) {
    u8 *base;

    sound_slot <<= 0x18;
    base = gBattleAnimationSoundSlots;
    asm volatile("" : "+r"(base));
    sound_slot = (u32)sound_slot >> 0x17;
    sound_slot += (u32)base;
    return PlaySong(*(u16 *)sound_slot);
}
