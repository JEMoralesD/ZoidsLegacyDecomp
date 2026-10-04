#include "player_selection.h"

extern u8 gTeamDetailSizeSprite[] asm("D_02032B90");
extern u8 D_020218E4[];
extern u8 gBattleSetup[];
extern u32 gZoidNameTable[];
extern void D_081062D4;
extern void D_081061A4;
extern void D_081062D8;
extern void D_08105BE8;
extern void D_08105C24;
extern void D_0810651C;
extern void D_08106380;
extern void D_08106534;
extern void D_0810654C;

void ClearWindow(s32) asm("func_080986B4");
void DestroySprite(void) asm("func_08094554");
s32 PrintWindowTextAt(void *, s32, s32, s32, s32) asm("func_080981F0");
void PrintWindowText(void *, s32, s32) asm("func_08098248");
void PrintWindowNumber(s32, s32, s32, s32, s32) asm("func_080984C4");
void PrintWindowNumberAt(s16, s32, s32, s32, s32, s32, s32) asm("func_0809844C");
void *CreateSprite(void *, void *, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
M2C_UNK GetPilotDisplayName(s32) asm("func_080E7B64");
s32 DivideSigned32(s16, s32) asm("func_080ECD98");

s32 DrawPlayerZoidTeamDetails(u8 *zoid_record, u8 empty_slot_size_limit) asm("func_080B3410");

s32 DrawPlayerZoidTeamDetails(u8 *zoid_record, u8 empty_slot_size_limit)
{
    ClearWindow(5);
    ClearWindow(6);
    if (*(void **)gTeamDetailSizeSprite != 0) {
        DestroySprite();
        *(void **)gTeamDetailSizeSprite = 0;
    }

    if (zoid_record != 0) {
        s32 current_hp;
        s32 detail_window_id;
        s32 max_hp;
        s32 low_hp_threshold;
        s32 hp_text_color;

        PrintWindowTextAt((void *)gZoidNameTable[M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(model_id))], 0, 5, 0, 0);
        PrintWindowTextAt(&D_081062D4, 0, 5, 0, 2);

        if (gBattleSetup[0] != 1) {
            current_hp = M2C_FIELD(zoid_record, s16 *, PLAYER_ZOID_OFFSET(current_hp));
            hp_text_color = 0;
            low_hp_threshold = DivideSigned32(M2C_FIELD(zoid_record, s16 *, PLAYER_ZOID_OFFSET(max_hp)), 10);
            {
                s32 hp_for_comparison = current_hp;
                asm volatile("" : "+&r"(hp_for_comparison) : "r"(current_hp));
                if (hp_for_comparison < (s16)low_hp_threshold)
                    hp_text_color = 1;
            }
            PrintWindowNumber(current_hp, 4, hp_text_color, 10, 5);
        } else {
            s32 max_hp_at_full_health;
            register s32 reserve_r2 asm("r2");
            asm volatile("" : "=r"(reserve_r2));
            max_hp_at_full_health = M2C_FIELD(zoid_record, s16 *, PLAYER_ZOID_OFFSET(max_hp));
            asm volatile("" :: "r"(reserve_r2));
            PrintWindowNumber(max_hp_at_full_health, 4, 0, 10, 5);
        }

        PrintWindowText(&D_081061A4, 0, 5);
        {
            register s32 reserve_r1 asm("r1");
            asm volatile("" : "=r"(reserve_r1));
            max_hp = M2C_FIELD(zoid_record, s16 *, PLAYER_ZOID_OFFSET(max_hp));
            asm volatile("" :: "r"(reserve_r1));
        }
        detail_window_id = 5;
        PrintWindowNumber(max_hp, 4, 0, 10, detail_window_id);
        PrintWindowTextAt(&D_081062D8, 0, 5, 0, 3);
        PrintWindowNumberAt(M2C_FIELD(zoid_record, s16 *, PLAYER_ZOID_OFFSET(max_ep)), 4, 0, 10, detail_window_id, 8, 3);
        *(void **)gTeamDetailSizeSprite = CreateSprite(&D_08105BE8, &D_08105C24,
            M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(size_class)), 108, 132, 0x355, 15, 8, 0);
        {
            s32 item_base = (s32)D_020218E4;
            s32 item_address = M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(pilot_slot)) << 6;

            item_address += item_base;
            item_address += PLAYER_STATE_OFFSET(pilots);
            PrintWindowText(GetPilotDisplayName(*(u8 *)item_address), 0, 6);
        }
    } else {
        PrintWindowTextAt(&D_0810651C, 0, 5, 0, (s32)zoid_record);
        switch (empty_slot_size_limit) {
        case EMPTY_TEAM_SLOT_ANY_SIZE:
            PrintWindowTextAt(&D_08106380, 2, 5, 0, 2);
            break;
        case EMPTY_TEAM_SLOT_SIZE_S_ONLY:
            PrintWindowTextAt(&D_08106534, 1, 5, 0, 2);
            break;
        case EMPTY_TEAM_SLOT_EXCLUDE_XL:
            PrintWindowTextAt(&D_0810654C, 1, 5, 0, empty_slot_size_limit);
            break;
        }
        PrintWindowText(GetPilotDisplayName(0), 0, 6);
    }
}
