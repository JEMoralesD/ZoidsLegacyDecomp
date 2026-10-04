#include "m2c_prelude.h"
#include "../battle/battle.h"
u8 GetZoidFormId(u8 *zoid, u8 form_index) {
    u8 result;
    int temp_r2;
    result = form_index;
    temp_r2 = *zoid;
    switch (temp_r2) {
      case ZOID_LIGER_ZERO: case ZOID_ZERO_SCHNEIDER: case ZOID_ZERO_JAEGER: case ZOID_ZERO_PANZER: case ZOID_ZERO_EMPIRE: case ZOID_ZERO_X:
        result += ZOID_LIGER_ZERO;
        return result;
      case ZOID_BERSERK_FURY: case ZOID_STRUM_FURY: case ZOID_JAGD_FURY: case ZOID_BERSERK_FURY_Z:
        return form_index + ZOID_BERSERK_FURY;
    }
    return *zoid;
}
