#include "m2c_prelude.h"
#include "../game/player_state.h"

extern struct PlayerZoidRecordView gPlayerZoidRecords[] asm("D_020218E8");

s32 RestoreAllUndestroyedPlayerZoidHp(void) asm("func_080BA540");

s32 RestoreAllUndestroyedPlayerZoidHp(void) {
    s32 restored_any_hp;
    u8 storage_slot;
    struct PlayerZoidRecordView *zoid;

    restored_any_hp = 0;
    storage_slot = 1;
    do {
        zoid = &gPlayerZoidRecords[storage_slot];
        if (zoid->model_id != 0 && !(zoid->flags & PLAYER_ZOID_DESTROYED) && M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(current_hp)) < (s16)zoid->max_hp) {
            M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(current_hp)) = zoid->max_hp;
            restored_any_hp = 1;
        }
        storage_slot += 1;
    } while ((u32)storage_slot <= 0xCE);
    return restored_any_hp;
}
