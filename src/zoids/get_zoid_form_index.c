#include "m2c_prelude.h"
#include "../game/player_state.h"
s32 GetZoidFormIndex(u8 zoid_model_id) asm("func_080E523C");

s32 GetZoidFormIndex(u8 zoid_model_id) {
    switch (zoid_model_id) {
    case ZOID_LIGER_ZERO:
        return 0;
    case ZOID_ZERO_SCHNEIDER:
        return 1;
    case ZOID_ZERO_JAEGER:
        return 2;
    case ZOID_ZERO_PANZER:
        return 3;
    case ZOID_ZERO_EMPIRE:
        return 4;
    case ZOID_ZERO_X:
        return 5;
    case ZOID_STRUM_FURY:
        return 1;
    case ZOID_JAGD_FURY:
        return 2;
    case ZOID_BERSERK_FURY_Z:
        return 3;
    default:
        return 0;
    }
}
