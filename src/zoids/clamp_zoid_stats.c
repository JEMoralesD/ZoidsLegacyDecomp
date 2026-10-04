#include "m2c_prelude.h"
#include "../game/player_state.h"
void ClampZoidStats(void *zoid) asm("func_080E57D0");

void ClampZoidStats(void *zoid) {
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)) > ZOID_MAX_HP_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_hp)) = ZOID_MAX_HP_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)) > ZOID_DCP_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(dcp)) = ZOID_DCP_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_ep)) > ZOID_MAX_EP_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(max_ep)) = ZOID_MAX_EP_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(ep_regen)) > ZOID_EP_REGEN_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(ep_regen)) = ZOID_EP_REGEN_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(speed)) > ZOID_SPEED_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(speed)) = ZOID_SPEED_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)) > ZOID_MOBILITY_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(mobility)) = ZOID_MOBILITY_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(defense)) > ZOID_DEFENSE_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(defense)) = ZOID_DEFENSE_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(armor_rate)) > ZOID_ARMOR_RATE_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(armor_rate)) = ZOID_ARMOR_RATE_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) > ZOID_SENSOR_ACCURACY_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(sensor_accuracy)) = ZOID_SENSOR_ACCURACY_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(load_capacity)) > ZOID_LOAD_CAPACITY_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(load_capacity)) = ZOID_LOAD_CAPACITY_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(evasion_score)) > ZOID_EVASION_SCORE_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(evasion_score)) = ZOID_EVASION_SCORE_LIMIT;
    }
    if ((s32) M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(initiative)) > ZOID_INITIATIVE_LIMIT) {
        M2C_FIELD(zoid, s16 *, PLAYER_ZOID_OFFSET(initiative)) = ZOID_INITIATIVE_LIMIT;
    }
}
