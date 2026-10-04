#include "m2c_prelude.h"
#include "player_state.h"
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern u8 gPlayerMoneyOffset asm("D_off_6A04");
s32 AddPlayerMoney(s32 amount) asm("func_080E5E64");

s32 AddPlayerMoney(s32 amount) {
    register s32 player_state_address asm("r1");
    register s32 money_offset asm("r3");
    u32 *money_slot;
    player_state_address = (s32)gPlayerStateBytes;
    money_offset = (s32)&gPlayerMoneyOffset;
    money_slot = (u32 *)(player_state_address + money_offset);
    {
        u32 updated_money = *money_slot + amount;
        if (updated_money <= PLAYER_MONEY_LIMIT) { *money_slot = updated_money; return PLAYER_STORAGE_ADD_WITHIN_LIMIT; }
        *money_slot = PLAYER_MONEY_LIMIT;
        return PLAYER_STORAGE_ADD_CAPPED;
    }
}
