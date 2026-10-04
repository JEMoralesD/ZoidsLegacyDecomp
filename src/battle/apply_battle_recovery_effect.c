#include "m2c_prelude.h"
#include "battle.h"
extern u8 gBattleState[];
void ApplyBattleRecoveryEffect(u8 side, u8 unit_slot, void *effect) {
    u8 *unit = gBattleState + (side * 0x1380) + (unit_slot * 0x270);
    s32 effect_kind = BATTLE_EFFECT_KIND_MASK & *(u16 *)((u8 *)effect + 4);
    switch (effect_kind) {
    case BATTLE_EFFECT_HP_RECOVERY:
        *(u16 *)(unit + 6) = *(u16 *)(unit + 6) + *(u16 *)((u8 *)effect + 6);
        return;
    case BATTLE_EFFECT_EP_RECOVERY:
        *(u16 *)(unit + 8) = *(u16 *)(unit + 8) + *(u16 *)((u8 *)effect + 6);
        return;
    }
}
