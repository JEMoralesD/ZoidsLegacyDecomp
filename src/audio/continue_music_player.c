#include "m2c_prelude.h"
void ContinueMusicPlayer(void *player) {
    register s32 lock asm("r3") = M2C_FIELD(player, s32 *, 0x34);
    if (lock == 0x68736D53) {
        M2C_FIELD(player, s32 *, 4) = (s32)(M2C_FIELD(player, s32 *, 4) & 0x7FFFFFFF);
    }
}
