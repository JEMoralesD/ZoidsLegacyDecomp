#include "m2c_prelude.h"
#include "../game/player_state.h"
void RestoreDestroyedZoidHp(void *zoid_record) asm("func_080E6090");

void RestoreDestroyedZoidHp(void *zoid_record) {
    register void *zoid asm("r1") = zoid_record;
    register u16 zoid_flags asm("r2") = M2C_FIELD(zoid, u16 *, PLAYER_ZOID_OFFSET(flags));
    if (zoid_flags & PLAYER_ZOID_DESTROYED) {
        u16 clear_destroyed_mask = 0xFFF7;
        clear_destroyed_mask &= zoid_flags;
        M2C_FIELD(zoid, u16 *, PLAYER_ZOID_OFFSET(flags)) = clear_destroyed_mask;
        M2C_FIELD(zoid, u16 *, PLAYER_ZOID_OFFSET(current_hp)) = M2C_FIELD(zoid, u16 *, PLAYER_ZOID_OFFSET(max_hp));
    }
}
