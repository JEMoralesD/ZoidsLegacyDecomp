#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u8 gPlayerMoneyOffset asm("D_off_6A04");
s32 SubtractPlayerMoney(u32 amount) asm("func_080E5E90");

s32 SubtractPlayerMoney(u32 amount) {
    register s32 player_state_address asm("r0");
    register s32 money_offset asm("r3");
    u32 *money_slot;
    player_state_address = (s32)gPlayerStateBytes;
    money_offset = (s32)&gPlayerMoneyOffset;
    money_slot = (u32 *)(player_state_address + money_offset);
    {
        u32 current_money = *money_slot;
        if (current_money > amount) { *money_slot = current_money - amount; return PLAYER_STORAGE_SUBTRACT_REMAINS; }
        *money_slot = 0;
        return PLAYER_STORAGE_SUBTRACT_DEPLETED;
    }
}
