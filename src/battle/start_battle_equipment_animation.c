#include "m2c_prelude.h"
#include "battle.h"
#include "battle_animation.h"
extern struct BattleAnimationState gBattleAnimationState asm("D_02033FD0");
extern struct BattleAnimationState D_02033FD0b;

extern struct EquipmentRecord gEquipmentCatalog[] asm("D_087B2524");
extern u8 D_087ED68C[];
extern struct BattleAnimationRecord gBattleAnimationRecords[] asm("D_087D818C");

extern void ResetBattleAnimationResourceAllocation(void) asm("func_080D2328");
extern void StartBattleAnimation(s32, u8, u8, u16, s32, s32) asm("func_080D0B60");
extern void StartZoidEquipmentAnimation(u8, u8) asm("func_080D04A0");

void StartBattleEquipmentAnimation(u8 side, u8 zoid_id, u16 item_id, u8 equipment_slot, u8 variant) asm("func_080D0CA0");

void StartBattleEquipmentAnimation(u8 side, u8 zoid_id, u16 item_id, u8 equipment_slot, u8 variant) {
    s32 var_r2;
    struct EquipmentRecord *pb7;
    register s32 idx asm("r0");
    register s32 pbase asm("r1");
    register s32 var_r2_2 asm("r2");
    gBattleAnimationState.presentation_kind = BATTLE_ANIMATION_EQUIPMENT;
    ResetBattleAnimationResourceAllocation();
    if (equipment_slot <= 2 && (pb7 = gEquipmentCatalog, var_r2 = item_id * 2, (pb7[item_id].flags & 1) == 0)) {
        gBattleAnimationState.sprite_flags = D_087ED68C[equipment_slot + zoid_id * 3] << 6;
    } else {
        D_02033FD0b.sprite_flags = 0x40;
        var_r2 = item_id * 2;
    }
    idx = (var_r2 + item_id) << 5;
    pbase = (s32)gBattleAnimationRecords;
    var_r2_2 = idx + pbase;
    if ((u8)(equipment_slot - 1) <= 2) {
        var_r2_2 += 0xC;
    }
    StartBattleAnimation(var_r2_2, side, zoid_id, item_id, equipment_slot, variant);
    StartZoidEquipmentAnimation(zoid_id, equipment_slot);
}
