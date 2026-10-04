#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
s32 WriteIndefiniteArticleForItemName(u8 *, u8 *) asm("func_0809F770");
extern s32 AddOrReactivatePlayerPilot() asm("func_080E6C78");
extern s32 AddPlayerZoid() asm("func_080E5A18");
extern s32 AddTemporaryPlayerZoidWithPilot() asm("func_080E6EEC");
extern s32 AssignPlayerPilotToZoid() asm("func_080E6FA0");
extern s32 AssignZoidToPlayerTeam() asm("func_080E5FA8");
extern s32 InitializePlayerAuxiliaryPilot() asm("func_080E6E04");
extern s32 RemovePlayerPilot() asm("func_080E6E98");
extern s32 RemovePlayerPilotAndTemporaryZoid() asm("func_080E6F6C");
extern s32 AddPlayerZoidWithPilot() asm("func_080E5C8C");
extern s32 AddEquipmentToInventory() asm("func_080E5CE4");
extern s32 AddRecoveryItemsToInventory() asm("func_080E5D6C");
extern s32 UnlockZoidData() asm("func_080E5DC4");
extern s32 AddZoidCoresToInventory() asm("func_080E5E0C");
extern s32 AddPlayerMoney() asm("func_080E5E64");
extern s32 UnlockDeckCommand() asm("func_080E5EBC");
int GetPilotDisplayName() asm("func_080E7B64");
#include "../../game/game_state.h"

extern int CopyBytes() asm("func_080ED038");
extern int CopyString() asm("func_080ED128");
int CountEncodedTextGlyphs() asm("func_08098B58");
int FormatNumberText() asm("func_08098284");

extern void PlayOrContinueSong() asm("func_08092E74");
extern void PlaySong() asm("func_08092E84");
extern void StopSong() asm("func_08092EA0");
extern void DestroySprite() asm("func_08094554");
extern void RunMenuScript() asm("func_08098BB4");
extern void SeekEventCommand() asm("func_080A016C");

u32 EventUpdatePlayerAssetsAndRoster(u8 script_slot, u8 **cursor) asm("func_080A3A40");

u32 EventUpdatePlayerAssetsAndRoster(u8 script_slot, u8 **cursor)
{
    u8 saved_script_slot;
    u32 dialog_state_or_operation;
    u32 glyph_count_or_pilot_slot;

    saved_script_slot = script_slot;
    StopSong(*(u8 *)0x02030667);
    PlaySong(EVENT_PLAYER_ASSET_SOUND);
    *(u8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if (*(u32 *)0x02031744 != 0) {
        DestroySprite();
        *(u32 *)0x02031744 = 0;
    }
    if ((*(u32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) || (*(u8 *)0x02031748 != 0)) {
        dialog_state_or_operation = *(u8 *)0x02030666;
        if (dialog_state_or_operation != 0) {
            if (dialog_state_or_operation == 1) {
                RunMenuScript(0x080177F5);
                goto play_common_sound;
            }
        } else {
play_common_sound:
            RunMenuScript(0x080177ED);
            *(u8 *)0x02030666 = 2;
        }
    }
    dialog_state_or_operation = **cursor - EVENT_ADD_PLAYER_ZOID;
    switch (dialog_state_or_operation) {
    case EVENT_ADD_PLAYER_ZOID - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u16 *message_text;
        u16 *message_cursor;
        u32 message_glyph_count;
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        asm volatile("" : "+r"(message_text));
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        asm volatile(
            ".set c0_add_fix, 0\n\t"
            ".macro add dst, args:vararg\n\t"
            ".if c0_add_fix == 0\n\t.short 0x1C34\n\t"
            ".elseif c0_add_fix == 1\n\t.short 0x3416\n\t"
            ".elseif c0_add_fix == 2\n\t.short 0x4440\n\t"
            ".elseif c0_add_fix == 3\n\t.short 0x1C20\n\t"
            ".elseif c0_add_fix == 4\n\t"
            ".elseif c0_add_fix == 5\n\t.short 0x192C\n\t"
            ".elseif c0_add_fix == 6\n\t.short 0x1C20\n\t"
            ".elseif c0_add_fix == 7\n\t.short 0x1C38\n\t"
            ".elseif c0_add_fix == 8\n\t.short 0x300C\n\t"
            ".elseif c0_add_fix == 9\n\t.short 0x1980\n\t"
            ".elseif c0_add_fix == 10\n\t.short 0x1C30\n\t"
            ".elseif c0_add_fix == 11\n\t.short 0x301A\n\t.short 0x182D\n\t"
            ".elseif c0_add_fix == 12\n\t.short 0x4440\n\t"
            ".elseif c0_add_fix == 13\n\t.short 0x1C28\n\t"
            ".elseif c0_add_fix == 14\n\t.short 0x4440\n\t"
            ".elseif c0_add_fix == 15\n\t.short 0x183F\n\t"
            ".elseif c0_add_fix == 16\n\t.short 0x1C38\n\t"
            ".elseif c0_add_fix == 17\n\t.short 0x300D\n\t"
            ".elseif c0_add_fix == 18\n\t.short 0x1980\n\t"
            ".elseif c0_add_fix == 19\n\t.short 0x1C31\n\t"
            ".elseif c0_add_fix == 20\n\t.short 0x311C\n\t"
            ".else\n\t.short 0x1840\n\t"
            ".endif\n\t"
            ".set c0_add_fix, c0_add_fix + 1\n\t"
            ".endm\n\t"
            ".set c0_lsl_fix, 0\n\t"
            ".macro lsl dst, lhs, rhs\n\t"
            ".if c0_lsl_fix == 0\n\t.short 0x0080\n\t"
            ".elseif c0_lsl_fix == 1\n\t.short 0x0600\n\t"
            ".elseif c0_lsl_fix == 2\n\t.short 0x007D\n\t"
            ".elseif c0_lsl_fix == 3\n\t.short 0x0040\n\t"
            ".elseif c0_lsl_fix == 4\n\t.short 0x0080\n\t"
            ".elseif c0_lsl_fix == 5\n\t.short 0x0080\n\t"
            ".elseif c0_lsl_fix == 6\n\t.short 0x0600\n\t"
            ".elseif c0_lsl_fix == 7\n\t.short 0x0040\n\t"
            ".else\n\t.short 0x0078\n\t"
            ".endif\n\t"
            ".set c0_lsl_fix, c0_lsl_fix + 1\n\t"
            ".endm\n\t"
            ".set c0_lsr_fix, 0\n\t"
            ".macro lsr dst, lhs, rhs\n\t"
            ".if c0_lsr_fix == 0\n\t.short 0x0E07\n\t"
            ".else\n\t.short 0x0E00\n\t"
            ".endif\n\t"
            ".set c0_lsr_fix, c0_lsr_fix + 1\n\t"
            ".endm");
        message_cursor = message_text + 11;
        name_table = (u32 *)0x087EDD54;
        message_glyph_count = (u8)WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id)]);
        message_cursor += message_glyph_count;
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = message_glyph_count + 12;
        index <<= 1;
        index += (u32)message_text;
        address = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        *(u16 *)index = (u16)address;
        message_cursor = message_text + 13 + message_glyph_count;
        CopyString(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id)]);
        message_glyph_count += (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id)]);
        index = message_glyph_count + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text;
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B10, 14);
        asm volatile(".purgem add\n\t.purgem lsl\n\t.purgem lsr");
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        AddPlayerZoid(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, palette_variant));
        break;
    }
    case EVENT_ADD_FLAGGED_PLAYER_ZOID - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u32 message_glyph_count;
        u16 *message_text;
        u16 *message_cursor;
        register u16 *zoid_words_from_player_state asm("r1");
        u8 article_glyph_count;
        register u32 stored_zoid_slot asm("r4");
        register u32 player_state_base_address asm("r0");
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        asm volatile("" : "+r"(message_text));
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        asm volatile(
            ".set c1_add_fix, 0\n\t"
            ".macro add dst, args:vararg\n\t"
            ".if c1_add_fix == 0\n\t.short 0x1C34\n\t"
            ".elseif c1_add_fix == 1\n\t.short 0x3416\n\t"
            ".elseif c1_add_fix == 2\n\t.short 0x4440\n\t"
            ".elseif c1_add_fix == 3\n\t.short 0x1C20\n\t"
            ".elseif c1_add_fix == 4\n\t"
            ".elseif c1_add_fix == 5\n\t.short 0x192C\n\t"
            ".elseif c1_add_fix == 6\n\t.short 0x1C20\n\t"
            ".elseif c1_add_fix == 7\n\t.short 0x1C38\n\t"
            ".elseif c1_add_fix == 8\n\t.short 0x300C\n\t"
            ".elseif c1_add_fix == 9\n\t.short 0x1980\n\t"
            ".elseif c1_add_fix == 10\n\t.short 0x1C30\n\t"
            ".elseif c1_add_fix == 11\n\t.short 0x301A\n\t.short 0x182D\n\t"
            ".elseif c1_add_fix == 12\n\t.short 0x4440\n\t"
            ".elseif c1_add_fix == 13\n\t.short 0x1C28\n\t"
            ".elseif c1_add_fix == 14\n\t.short 0x4440\n\t"
            ".elseif c1_add_fix == 15\n\t.short 0x183F\n\t"
            ".elseif c1_add_fix == 16\n\t.short 0x1C38\n\t"
            ".elseif c1_add_fix == 17\n\t.short 0x300D\n\t"
            ".elseif c1_add_fix == 18\n\t.short 0x1980\n\t"
            ".elseif c1_add_fix == 19\n\t.short 0x1C31\n\t"
            ".elseif c1_add_fix == 20\n\t.short 0x311C\n\t"
            ".else\n\t.short 0x1840\n\t"
            ".endif\n\t"
            ".set c1_add_fix, c1_add_fix + 1\n\t"
            ".endm\n\t"
            ".set c1_lsl_fix, 0\n\t"
            ".macro lsl dst, lhs, rhs\n\t"
            ".if c1_lsl_fix == 0\n\t.short 0x0080\n\t"
            ".elseif c1_lsl_fix == 1\n\t.short 0x0600\n\t"
            ".elseif c1_lsl_fix == 2\n\t.short 0x007D\n\t"
            ".elseif c1_lsl_fix == 3\n\t.short 0x0040\n\t"
            ".elseif c1_lsl_fix == 4\n\t.short 0x0080\n\t"
            ".elseif c1_lsl_fix == 5\n\t.short 0x0080\n\t"
            ".elseif c1_lsl_fix == 6\n\t.short 0x0600\n\t"
            ".elseif c1_lsl_fix == 7\n\t.short 0x0040\n\t"
            ".else\n\t.short 0x0078\n\t"
            ".endif\n\t"
            ".set c1_lsl_fix, c1_lsl_fix + 1\n\t"
            ".endm\n\t"
            ".set c1_lsr_fix, 0\n\t"
            ".macro lsr dst, lhs, rhs\n\t"
            ".if c1_lsr_fix == 0\n\t.short 0x0E07\n\t"
            ".else\n\t.short 0x0E00\n\t"
            ".endif\n\t"
            ".set c1_lsr_fix, c1_lsr_fix + 1\n\t"
            ".endm");
        message_cursor = message_text + 11;
        name_table = (u32 *)0x087EDD54;
        article_glyph_count = WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id)]);
        message_cursor += article_glyph_count;
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = article_glyph_count + 12;
        index <<= 1;
        index += (u32)message_text;
        address = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        *(u16 *)index = (u16)address;
        message_cursor = message_text + 13 + article_glyph_count;
        CopyString(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id)]);
        message_glyph_count = article_glyph_count + (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id)]);
        index = message_glyph_count + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text;
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B10, 14);
        asm volatile(".purgem add\n\t.purgem lsl\n\t.purgem lsr");
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        stored_zoid_slot = (u8)AddPlayerZoid(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, model_id), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidCommand, palette_variant));
        player_state_base_address = 0x020218E4;
        asm volatile("" : "+r"(player_state_base_address));
        zoid_words_from_player_state = (u16 *)((stored_zoid_slot * 0x70) + player_state_base_address);
        {
            register u32 flags asm("r2") = M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(flags));
            register u32 mask asm("r0") = 0x10;

            mask |= flags;
            M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(flags)) = (u16)mask;
        }
        break;
    }
    case EVENT_ADD_PLAYER_ZOID_WITH_PILOT - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u32 message_glyph_count;
        u16 *message_text;
        u16 *message_cursor;
        u8 name_or_article_glyph_count;
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        asm volatile("" : "+r"(message_text));
        message_text[0] = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        asm volatile("" ::: "memory");
        asm volatile(
            ".set c2a_add_fix, 0\n\t"
            ".macro add dst, args:vararg\n\t"
            ".if c2a_add_fix == 0\n\t.short 0x1CB4\n\t"
            ".elseif c2a_add_fix == 1\n\t.short 0x1C01\n\t"
            ".elseif c2a_add_fix == 2\n\t.short 0x1C20\n\t"
            ".elseif c2a_add_fix == 3\n\t"
            ".elseif c2a_add_fix == 4\n\t.short 0x1C78\n\t"
            ".elseif c2a_add_fix == 5\n\t.short 0x1980\n\t"
            ".elseif c2a_add_fix == 6\n\t.short 0x1D31\n\t"
            ".elseif c2a_add_fix == 7\n\t.short 0x1840\n\t"
            ".else\n\t.short 0x1C30\n\t"
            ".endif\n\t"
            ".set c2a_add_fix, c2a_add_fix + 1\n\t"
            ".endm\n\t"
            ".set c2a_mov_fix, 0\n\t"
            ".macro mov dst, args:vararg\n\t"
            ".if c2a_mov_fix == 0\n\t.short 0x4651\n\t"
            ".elseif c2a_mov_fix == 1\n\t.short 0x4652\n\t"
            ".elseif c2a_mov_fix == 2\n\t.short 0x2101\n\t"
            ".elseif c2a_mov_fix == 3\n\t"
            ".elseif c2a_mov_fix == 4\n\t.short 0x2227\n\t"
            ".elseif c2a_mov_fix == 5\n\t.short 0x2035\n\t"
            ".else\n\t.short 0x2217\n\t"
            ".endif\n\t"
            ".set c2a_mov_fix, c2a_mov_fix + 1\n\t"
            ".endm\n\t"
            ".set c2a_lsl_fix, 0\n\t"
            ".macro lsl dst, lhs, rhs\n\t"
            ".if c2a_lsl_fix == 0\n\t.short 0x0600\n\t"
            ".elseif c2a_lsl_fix == 1\n\t.short 0x0040\n\t"
            ".else\n\t.short 0x0078\n\t"
            ".endif\n\t"
            ".set c2a_lsl_fix, c2a_lsl_fix + 1\n\t"
            ".endm\n\t"
            ".macro lsr dst, lhs, rhs\n\t"
            ".short 0x0E07\n\t"
            ".purgem lsr\n\t"
            ".endm\n\t"
            ".macro strh src, addr:vararg\n\t"
            ".short 0x8001\n\t"
            ".purgem strh\n\t"
            ".endm");
        message_cursor = message_text + 1;
        CopyString(message_cursor, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        name_or_article_glyph_count = CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        index = name_or_article_glyph_count + 1;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = name_or_article_glyph_count << 1;
        address = (u32)message_text + 4;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B20, 39);
        asm volatile(
            ".macro ldr dst, addr:vararg\n\t"
            ".short 0x4A30\n\t"
            ".purgem ldr\n\t"
            ".endm\n\t"
            ".macro str src, addr:vararg\n\t"
            ".short 0x6016\n\t"
            ".purgem str\n\t"
            ".endm");
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        asm volatile(
            ".macro ldr dst, addr:vararg\n\t"
            ".short 0x4830\n\t.short 0x4681\n\t"
            ".purgem ldr\n\t"
            ".endm");
        RunMenuScript(0x080177D5);
        PlaySong(EVENT_PLAYER_ASSET_SOUND);
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        asm volatile(".purgem add\n\t.purgem mov\n\t.purgem lsl");
        asm volatile(
            ".set c2b_add_fix, 0\n\t"
            ".macro add dst, args:vararg\n\t"
            ".if c2b_add_fix == 0\n\t.short 0x3414\n\t"
            ".elseif c2b_add_fix == 1\n\t.short 0x4440\n\t"
            ".elseif c2b_add_fix == 2\n\t.short 0x1C20\n\t"
            ".elseif c2b_add_fix == 3\n\t"
            ".elseif c2b_add_fix == 4\n\t.short 0x192C\n\t"
            ".elseif c2b_add_fix == 5\n\t.short 0x1C20\n\t"
            ".elseif c2b_add_fix == 6\n\t.short 0x1C38\n\t"
            ".elseif c2b_add_fix == 7\n\t.short 0x300C\n\t"
            ".elseif c2b_add_fix == 8\n\t.short 0x1980\n\t"
            ".elseif c2b_add_fix == 9\n\t.short 0x1C30\n\t"
            ".elseif c2b_add_fix == 10\n\t.short 0x301A\n\t.short 0x182D\n\t"
            ".elseif c2b_add_fix == 11\n\t.short 0x4440\n\t"
            ".elseif c2b_add_fix == 12\n\t.short 0x1C28\n\t"
            ".elseif c2b_add_fix == 13\n\t.short 0x4440\n\t"
            ".elseif c2b_add_fix == 14\n\t.short 0x183F\n\t"
            ".elseif c2b_add_fix == 15\n\t.short 0x1C38\n\t"
            ".elseif c2b_add_fix == 16\n\t.short 0x300D\n\t"
            ".elseif c2b_add_fix == 17\n\t.short 0x1980\n\t"
            ".elseif c2b_add_fix == 18\n\t.short 0x1C31\n\t"
            ".elseif c2b_add_fix == 19\n\t.short 0x311C\n\t"
            ".else\n\t.short 0x1840\n\t"
            ".endif\n\t"
            ".set c2b_add_fix, c2b_add_fix + 1\n\t"
            ".endm\n\t"
            ".set c2b_lsl_fix, 0\n\t"
            ".macro lsl dst, lhs, rhs\n\t"
            ".if c2b_lsl_fix == 0\n\t.short 0x0080\n\t"
            ".elseif c2b_lsl_fix == 1\n\t.short 0x0600\n\t"
            ".elseif c2b_lsl_fix == 2\n\t.short 0x007D\n\t"
            ".elseif c2b_lsl_fix == 3\n\t.short 0x0040\n\t"
            ".elseif c2b_lsl_fix == 4\n\t.short 0x0080\n\t"
            ".elseif c2b_lsl_fix == 5\n\t.short 0x0080\n\t"
            ".elseif c2b_lsl_fix == 6\n\t.short 0x0600\n\t"
            ".elseif c2b_lsl_fix == 7\n\t.short 0x0040\n\t"
            ".else\n\t.short 0x0078\n\t"
            ".endif\n\t"
            ".set c2b_lsl_fix, c2b_lsl_fix + 1\n\t"
            ".endm\n\t"
            ".set c2b_lsr_fix, 0\n\t"
            ".macro lsr dst, lhs, rhs\n\t"
            ".if c2b_lsr_fix == 0\n\t.short 0x0E07\n\t"
            ".else\n\t.short 0x0E00\n\t"
            ".endif\n\t"
            ".set c2b_lsr_fix, c2b_lsr_fix + 1\n\t"
            ".endm\n\t"
            ".set c2b_mov_fix, 0\n\t"
            ".macro mov dst, args:vararg\n\t"
            ".if c2b_mov_fix == 0\n\t.short 0x4688\n\t"
            ".elseif c2b_mov_fix == 1\n\t.short 0x4652\n\t"
            ".elseif c2b_mov_fix == 2\n\t.short 0x2203\n\t"
            ".elseif c2b_mov_fix == 3\n\t.short 0x4652\n\t"
            ".elseif c2b_mov_fix == 4\n\t.short 0x4651\n\t"
            ".elseif c2b_mov_fix == 5\n\t.short 0x2201\n\t"
            ".else\n\t.short 0x220E\n\t"
            ".endif\n\t"
            ".set c2b_mov_fix, c2b_mov_fix + 1\n\t"
            ".endm");
        message_cursor += 10;
        name_table = (u32 *)0x087EDD54;
        name_or_article_glyph_count = WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        message_cursor = (u16 *)(((u8)name_or_article_glyph_count * 2) + (u32)message_cursor);
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = name_or_article_glyph_count + 12;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        CopyString(message_text + 13 + name_or_article_glyph_count, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        message_glyph_count = name_or_article_glyph_count + (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        index = message_glyph_count + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text;
        asm volatile("" : "+r"(address));
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B10, 14);
        asm volatile(".purgem add\n\t.purgem lsl\n\t.purgem lsr\n\t.purgem mov");
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        asm volatile(
            ".macro ldr dst, addr:vararg\n\t"
            ".short 0x4648\n\t"
            ".purgem ldr\n\t"
            ".endm");
        RunMenuScript(0x080177D5);
        AddPlayerZoidWithPilot(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, palette_variant), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id));
        break;
    }
    case EVENT_ADD_FLAGGED_PLAYER_ZOID_WITH_PILOT - EVENT_ADD_PLAYER_ZOID:
    {
        register u32 *name_table asm("r8");
        u16 *message_text;
        u16 *initial_message_cursor;
        u16 *message_cursor;
        register u16 *zoid_words_from_player_state asm("r0");
        u32 message_glyph_count;
        u32 stored_zoid_slot;
        register u32 message_menu_script asm("r9");
        register u32 message_byte_offset asm("r5");
        u8 stored_pilot_slot;
        register u32 index asm("r0");
        register u32 address asm("r1");
        register u32 player_state_base_address asm("r1");
        register u32 reserve_r5 asm("r5");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        message_text[0] = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        initial_message_cursor = message_text + 1;
        CopyString(initial_message_cursor, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        message_glyph_count = (u8)CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        index = message_glyph_count + 1;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text + 4;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B20, 39);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        message_menu_script = 0x080177D5;
        RunMenuScript(message_menu_script);
        PlaySong(EVENT_PLAYER_ASSET_SOUND);
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        message_cursor = message_text + 11;
        name_table = (u32 *)0x087EDD54;
        message_glyph_count = (u8)WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        message_byte_offset = message_glyph_count << 1;
        message_cursor = (u16 *)(message_byte_offset + (u32)message_cursor);
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = message_glyph_count + 12;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        index = (u32)message_text;
        index += 26;
        message_byte_offset += index;
        CopyString((u16 *)message_byte_offset, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        message_glyph_count += (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        index = message_glyph_count + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text;
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B10, 14);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(message_menu_script);
        stored_zoid_slot = (u8)AddPlayerZoid(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, palette_variant));
        if (stored_zoid_slot != PLAYER_STORAGE_SLOT_NOT_FOUND) {
            player_state_base_address = 0x020218E4;
            asm volatile("" : "+r"(player_state_base_address));
            zoid_words_from_player_state = (u16 *)((stored_zoid_slot * 0x70) + player_state_base_address);
            M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(flags)) |= 0x10;
        }
        stored_pilot_slot = AddOrReactivatePlayerPilot(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id));
        if ((stored_zoid_slot != PLAYER_STORAGE_SLOT_NOT_FOUND) && (stored_pilot_slot != PLAYER_STORAGE_SLOT_NOT_FOUND)) {
            AssignPlayerPilotToZoid(stored_pilot_slot, stored_zoid_slot);
            player_state_base_address = 0x020218E4;
            asm volatile("" : "+r"(player_state_base_address));
            zoid_words_from_player_state = (u16 *)((stored_zoid_slot * 0x70) + player_state_base_address);
            M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(current_hp)) = M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(max_hp));
            M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(current_ep)) = M2C_FIELD(zoid_words_from_player_state, u16 *, PLAYER_STATE_OFFSET(zoids) + PLAYER_ZOID_OFFSET(max_ep));
        }
        break;
    }
    case EVENT_ADD_PLAYER_PILOT - EVENT_ADD_PLAYER_ZOID:
    {
        register u16 *message_text asm("r4");
        register u32 pilot_name_glyph_count asm("r7");
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[0] = (u16)index;
        asm volatile("" ::: "memory");
        CopyString(message_text + 1, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)));
        pilot_name_glyph_count = (u8)CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)));
        index = pilot_name_glyph_count + 1;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = pilot_name_glyph_count << 1;
        address = (u32)message_text + 4;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B20, 39);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        AddOrReactivatePlayerPilot(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id));
        asm volatile(
            ".macro bl target\n\t"
            "b \\target\n\t"
            ".purgem bl\n\t"
            ".endm");
        break;
    }
    case EVENT_REMOVE_PLAYER_PILOT - EVENT_ADD_PLAYER_ZOID:
    {
        u16 *message_text;
        register u32 index asm("r0");
        register u32 address asm("r1");
        register u8 *message_prefix_or_player_state_base asm("r0");
        register u8 *pilot_record asm("r2");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        asm volatile("" : "+r"(message_text));
        message_prefix_or_player_state_base = (u8 *)EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[0] = (u16)(u32)message_prefix_or_player_state_base;
        CopyString(message_text + 1, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)));
        glyph_count_or_pilot_slot = (u8)CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)));
        index = glyph_count_or_pilot_slot + 1;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        if (glyph_count_or_pilot_slot > 13) {
            index = glyph_count_or_pilot_slot << 1;
            address = (u32)message_text + 4;
            index += address;
            CopyBytes((u16 *)index, (void *)0x08103B48, 32);
        } else {
            index = glyph_count_or_pilot_slot << 1;
            address = (u32)message_text + 4;
            index += address;
            CopyBytes((u16 *)index, (void *)0x08103B68, 31);
        }
        address = EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM;
        message_prefix_or_player_state_base = (u8 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        *(u16 **)address = (u16 *)message_prefix_or_player_state_base;
        RunMenuScript(0x080177D5);
        glyph_count_or_pilot_slot = 1;
        message_prefix_or_player_state_base = (u8 *)0x020218E4;
        asm volatile("" : "+r"(message_prefix_or_player_state_base));
        pilot_record = message_prefix_or_player_state_base + 0x5AD4;
        goto check_case_5;
advance_case_5:
        pilot_record += 0x40;
        glyph_count_or_pilot_slot++;
        if (glyph_count_or_pilot_slot >= PLAYER_STORED_PILOT_COUNT) {
            goto done_case_5;
        }
check_case_5:
        if (*pilot_record != EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)) {
            goto advance_case_5;
        }
done_case_5:
        RemovePlayerPilot((u8)glyph_count_or_pilot_slot);
        *(u8 *)0x02030665 = 1;
        break;
    }
    case EVENT_ADD_TEMPORARY_PLAYER_ZOID_WITH_PILOT - EVENT_ADD_PLAYER_ZOID:
    {
        register u32 *name_table asm("r8");
        u16 *message_text;
        register u16 *message_cursor asm("r4");
        u32 message_glyph_count;
        register u32 index asm("r0");
        register u32 address asm("r1");
        register u32 message_byte_offset asm("r5");

        asm volatile("" ::: "memory");
        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        message_text[0] = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_cursor = message_text + 1;
        CopyString(message_cursor, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        message_glyph_count = (u8)CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        index = message_glyph_count + 1;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text + 4;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B20, 39);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        PlaySong(EVENT_PLAYER_ASSET_SOUND);
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        message_cursor += 10;
        name_table = (u32 *)0x087EDD54;
        message_glyph_count = (u8)WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        message_byte_offset = message_glyph_count << 1;
        message_cursor = (u16 *)(message_byte_offset + (u32)message_cursor);
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = message_glyph_count + 12;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        index = (u32)message_text;
        index += 26;
        message_byte_offset += index;
        CopyString((u16 *)message_byte_offset, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        message_glyph_count += (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id)]);
        index = message_glyph_count + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text;
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103B10, 14);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        AddTemporaryPlayerZoidWithPilot(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, palette_variant), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id));
        break;
    }
    case EVENT_ADD_TEMPORARY_PLAYER_ZOID_WITH_PILOT_TO_TEAM - EVENT_ADD_PLAYER_ZOID:
    {
        register u16 *message_text asm("r4");
        register u8 *team_zoid_slots asm("r1");
        register u32 pilot_name_glyph_count asm("r7");
        register u32 stored_zoid_slot_or_result asm("r4");
        u32 index;
        register u32 draw_index asm("r0");
        register u32 draw_address asm("r1");

        asm volatile("" ::: "memory");
        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        draw_index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[0] = (u16)draw_index;
        CopyString(message_text + 1, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        pilot_name_glyph_count = (u8)CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id)));
        draw_index = pilot_name_glyph_count + 1;
        draw_index <<= 1;
        draw_index += (u32)message_text;
        *(u16 *)draw_index = 1;
        draw_index = pilot_name_glyph_count << 1;
        draw_address = (u32)message_text + 4;
        draw_index += draw_address;
        CopyBytes((u16 *)draw_index, (void *)0x08103B20, 39);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        stored_zoid_slot_or_result = (u8)AddTemporaryPlayerZoidWithPilot(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, model_id), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, palette_variant), EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidPilotCommand, pilot_id));
        index = 0;
        asm volatile("" : "+r"(index) : : "r0", "r1", "r2", "r3", "r5", "r6");
        team_zoid_slots = (u8 *)0x020281F0;
        while (index < PLAYER_TEAM_SLOT_COUNT) {
            if (*(u8 *)(index + (u32)team_zoid_slots) == 0) {
                AssignZoidToPlayerTeam((u8)index, stored_zoid_slot_or_result);
                break;
            }
            index++;
        }
        break;
    }
    case EVENT_REMOVE_PLAYER_PILOT_AND_TEMPORARY_ZOID - EVENT_ADD_PLAYER_ZOID:
    {
        u16 *message_text;
        register u32 index asm("r0");
        register u32 address asm("r1");
        register u8 *message_prefix_or_player_state_base asm("r0");
        register u8 *pilot_record asm("r2");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        asm volatile("" : "+r"(message_text));
        message_prefix_or_player_state_base = (u8 *)EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[0] = (u16)(u32)message_prefix_or_player_state_base;
        CopyString(message_text + 1, GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)));
        glyph_count_or_pilot_slot = (u8)CountEncodedTextGlyphs(GetPilotDisplayName(EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)));
        index = glyph_count_or_pilot_slot + 1;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        if (glyph_count_or_pilot_slot > 13) {
            index = glyph_count_or_pilot_slot << 1;
            address = (u32)message_text + 4;
            index += address;
            CopyBytes((u16 *)index, (void *)0x08103B48, 32);
        } else {
            index = glyph_count_or_pilot_slot << 1;
            address = (u32)message_text + 4;
            index += address;
            CopyBytes((u16 *)index, (void *)0x08103B68, 31);
        }
        address = EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM;
        message_prefix_or_player_state_base = (u8 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        *(u16 **)address = (u16 *)message_prefix_or_player_state_base;
        RunMenuScript(0x080177D5);
        glyph_count_or_pilot_slot = 1;
        message_prefix_or_player_state_base = (u8 *)0x020218E4;
        asm volatile("" : "+r"(message_prefix_or_player_state_base));
        pilot_record = message_prefix_or_player_state_base + 0x5AD4;
        goto check_case_8;
advance_case_8:
        pilot_record += 0x40;
        glyph_count_or_pilot_slot++;
        if (glyph_count_or_pilot_slot >= PLAYER_STORED_PILOT_COUNT) {
            goto done_case_8;
        }
check_case_8:
        if (*pilot_record != EVENT_COMMAND_BYTE(*cursor, EventPlayerPilotCommand, pilot_id)) {
            goto advance_case_8;
        }
done_case_8:
        RemovePlayerPilotAndTemporaryZoid((u8)glyph_count_or_pilot_slot);
        *(u8 *)0x02030665 = 1;
        break;
    }
    case EVENT_INITIALIZE_PROTAGONIST_AUXILIARY_PILOT - EVENT_ADD_PLAYER_ZOID:
    {
        u16 *message_text;
        register u8 *message_prefix_or_player_state_base asm("r0");
        register u8 *pilot_record asm("r1");
        register u32 pilot_records_offset asm("r2");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103B88, 23);
        message_prefix_or_player_state_base = (u8 *)EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[11] = (u16)(u32)message_prefix_or_player_state_base;
        CopyBytes(message_text + 12, (void *)0x08103BA0, 27);
        message_text[25] = 1;
        CopyBytes(message_text + 26, (void *)0x08103BBC, 40);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        glyph_count_or_pilot_slot = 1;
        message_prefix_or_player_state_base = (u8 *)0x020218E4;
        pilot_records_offset = 0x5AD4;
        asm volatile("" : "+r"(pilot_records_offset));
        pilot_record = message_prefix_or_player_state_base + pilot_records_offset;
        goto check_case_9;
advance_case_9:
        pilot_record += 0x40;
        glyph_count_or_pilot_slot++;
        if (glyph_count_or_pilot_slot >= PLAYER_STORED_PILOT_COUNT) {
            goto done_case_9;
        }
check_case_9:
        if (*pilot_record != 1) {
            goto advance_case_9;
        }
done_case_9:
        asm volatile(
            ".macro ldr dst, addr:vararg\n\t"
            ".short 0x4802\n\t"
            ".purgem ldr\n\t"
            ".endm\n\t"
            ".macro add dst, args:vararg\n\t"
            ".short 0x1809\n\t"
            ".purgem add\n\t"
            ".endm");
        InitializePlayerAuxiliaryPilot(1, (glyph_count_or_pilot_slot << 6) + 0x02027378);
        break;
    }
    case EVENT_ADD_PLAYER_EQUIPMENT - EVENT_ADD_PLAYER_ZOID:
    {
        register u32 *name_table asm("r6");
        u16 *message_text;
        register u16 *message_cursor asm("r4");
        register u32 index asm("r0");
        register u32 address asm("r1");
        register u32 message_byte_offset asm("r5");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        message_cursor = message_text + 11;
        name_table = (u32 *)0x087EE170;
        glyph_count_or_pilot_slot = (u8)WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        message_byte_offset = glyph_count_or_pilot_slot << 1;
        message_cursor = (u16 *)(message_byte_offset + (u32)message_cursor);
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = glyph_count_or_pilot_slot + 12;
        index <<= 1;
        index += (u32)message_text;
        address = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        *(u16 *)index = (u16)address;
        index = (u32)message_text;
        index += 26;
        message_byte_offset += index;
        CopyString((u16 *)message_byte_offset, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        glyph_count_or_pilot_slot += (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        index = glyph_count_or_pilot_slot + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = glyph_count_or_pilot_slot << 1;
        address = (u32)message_text;
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103BE4, 16);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        if ((AddEquipmentToInventory(EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id), 1) << 24) == 0) {
            CopyBytes(message_text, (void *)0x08103BF4, 51);
            RunMenuScript(0x080177D5);
        }
        break;
    }
    case EVENT_ADD_PLAYER_RECOVERY_ITEM - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u16 *message_text;
        u16 *message_cursor;
        u32 message_glyph_count;
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        asm volatile(
            ".macro mov dst, args:vararg\n\t"
            ".short 0x4644\n\t"
            ".purgem mov\n\t"
            ".endm\n\t"
            ".macro ldr dst, args:vararg\n\t"
            ".short 0x4E2B\n\t"
            ".purgem ldr\n\t"
            ".endm\n\t"
            ".set c11_add_fix, 0\n\t"
            ".macro add dst, args:vararg\n\t"
            ".if c11_add_fix == 0\n\t.short 0x3416\n\t"
            ".elseif c11_add_fix == 1\n\t.short 0x1980\n\t"
            ".elseif c11_add_fix == 2\n\t.short 0x1C20\n\t"
            ".elseif c11_add_fix == 3\n\t"
            ".elseif c11_add_fix == 4\n\t.short 0x192C\n\t"
            ".elseif c11_add_fix == 5\n\t.short 0x1C20\n\t"
            ".elseif c11_add_fix == 6\n\t.short 0x1C38\n\t"
            ".elseif c11_add_fix == 7\n\t.short 0x300C\n\t"
            ".elseif c11_add_fix == 8\n\t.short 0x4440\n\t"
            ".elseif c11_add_fix == 9\n\t.short 0x301A\n\t"
            ".elseif c11_add_fix == 10\n\t.short 0x182D\n\t"
            ".elseif c11_add_fix == 11\n\t.short 0x1980\n\t"
            ".elseif c11_add_fix == 12\n\t.short 0x1C28\n\t"
            ".elseif c11_add_fix == 13\n\t.short 0x1980\n\t"
            ".elseif c11_add_fix == 14\n\t.short 0x183F\n\t"
            ".elseif c11_add_fix == 15\n\t.short 0x1C38\n\t"
            ".elseif c11_add_fix == 16\n\t.short 0x300D\n\t"
            ".elseif c11_add_fix == 17\n\t.short 0x4440\n\t"
            ".elseif c11_add_fix == 18\n\t.short 0x311C\n\t"
            ".else\n\t.short 0x1840\n\t"
            ".endif\n\t"
            ".set c11_add_fix, c11_add_fix + 1\n\t"
            ".endm\n\t"
            ".set c11_lsl_fix, 0\n\t"
            ".macro lsl dst, lhs, rhs\n\t"
            ".if c11_lsl_fix == 0\n\t.short 0x0080\n\t"
            ".elseif c11_lsl_fix == 1\n\t.short 0x0600\n\t"
            ".elseif c11_lsl_fix == 2\n\t.short 0x007D\n\t"
            ".elseif c11_lsl_fix == 3\n\t.short 0x0040\n\t"
            ".elseif c11_lsl_fix == 4\n\t.short 0x0080\n\t"
            ".elseif c11_lsl_fix == 5\n\t.short 0x0080\n\t"
            ".elseif c11_lsl_fix == 6\n\t.short 0x0600\n\t"
            ".elseif c11_lsl_fix == 7\n\t.short 0x0040\n\t"
            ".else\n\t.short 0x0078\n\t"
            ".endif\n\t"
            ".set c11_lsl_fix, c11_lsl_fix + 1\n\t"
            ".endm\n\t"
            ".set c11_lsr_fix, 0\n\t"
            ".macro lsr dst, lhs, rhs\n\t"
            ".if c11_lsr_fix == 0\n\t.short 0x0E07\n\t"
            ".else\n\t.short 0x0E00\n\t"
            ".endif\n\t"
            ".set c11_lsr_fix, c11_lsr_fix + 1\n\t"
            ".endm");
        message_cursor = message_text + 11;
        name_table = (u32 *)0x087EEE10;
        message_glyph_count = (u8)WriteIndefiniteArticleForItemName(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        message_cursor += message_glyph_count;
        CopyBytes(message_cursor, (void *)0x08103B0C, 3);
        index = message_glyph_count + 12;
        index <<= 1;
        index += (u32)message_text;
        address = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        *(u16 *)index = (u16)address;
        index = (u32)message_text;
        index += 26;
        message_cursor = (u16 *)((message_glyph_count << 1) + index);
        CopyString(message_cursor, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        message_glyph_count += (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        index = message_glyph_count + 13;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = message_glyph_count << 1;
        address = (u32)message_text;
        address += 28;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103C28, 13);
        asm volatile(".purgem add\n\t.purgem lsl\n\t.purgem lsr");
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        if ((AddRecoveryItemsToInventory(EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id), 1) << 24) == 0) {
            CopyBytes(message_text, (void *)0x08103BF4, 51);
            RunMenuScript(0x080177D5);
        }
        break;
    }
    case EVENT_UNLOCK_PLAYER_ZOID_DATA - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u16 *message_text;
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103C38, 31);
        index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[15] = (u16)index;
        CopyString(message_text + 16,
            (name_table = (u32 *)0x087EDD54, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidDataCommand, model_id)]));
        glyph_count_or_pilot_slot = (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidDataCommand, model_id)]);
        index = glyph_count_or_pilot_slot + 16;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = glyph_count_or_pilot_slot << 1;
        address = (u32)message_text;
        address += 34;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103C58, 12);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        if ((UnlockZoidData(EVENT_COMMAND_BYTE(*cursor, EventPlayerZoidDataCommand, model_id)) << 24) == 0) {
            CopyBytes(message_text, (void *)0x08103C64, 45);
            RunMenuScript(0x080177D5);
        }
        break;
    }
    case EVENT_ADD_PLAYER_ZOID_CORE - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u16 *message_text;
        register u32 core_name_glyph_count asm("r7");
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103C38, 31);
        index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[15] = (u16)index;
        CopyString(message_text + 16,
            (name_table = (u32 *)0x087EEE60, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]));
        core_name_glyph_count = (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id)]);
        index = core_name_glyph_count + 16;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = core_name_glyph_count << 1;
        address = (u32)message_text;
        address += 34;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103C94, 14);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        if ((AddZoidCoresToInventory(EVENT_COMMAND_BYTE(*cursor, EventPlayerItemCommand, item_id), 1) << 24) == 0) {
            CopyBytes(message_text, (void *)0x08103BF4, 51);
            RunMenuScript(0x080177D5);
        }
        break;
    }
    case EVENT_UNLOCK_PLAYER_DECK_COMMAND - EVENT_ADD_PLAYER_ZOID:
    {
        u32 *name_table;
        u16 *message_text;
        register u32 command_name_glyph_count asm("r7");
        register u32 index asm("r0");
        register u32 address asm("r1");

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103CA4, 33);
        index = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[16] = (u16)index;
        CopyString(message_text + 17,
            (name_table = (u32 *)0x087EF130, name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerDeckCommand, deck_command_id)]));
        command_name_glyph_count = (u8)CountEncodedTextGlyphs(name_table[EVENT_COMMAND_BYTE(*cursor, EventPlayerDeckCommand, deck_command_id)]);
        index = command_name_glyph_count + 17;
        index <<= 1;
        index += (u32)message_text;
        *(u16 *)index = 1;
        index = command_name_glyph_count << 1;
        address = (u32)message_text;
        address += 36;
        index += address;
        CopyBytes((u16 *)index, (void *)0x08103CC8, 29);
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        if ((UnlockDeckCommand(EVENT_COMMAND_BYTE(*cursor, EventPlayerDeckCommand, deck_command_id)) << 24) == 0) {
            CopyBytes(message_text, (void *)0x08103CE8, 45);
            RunMenuScript(0x080177D5);
        }
        break;
    }
    case EVENT_ADD_PLAYER_MONEY - EVENT_ADD_PLAYER_ZOID:
    {
        register u16 *message_text asm("r5");
        register u16 *message_cursor asm("r4");
        register u32 money_unit asm("r6");
        register u32 zero asm("r8");
        register u32 marker asm("r0");
        u32 money_amount;

        message_text = (u16 *)EVENT_PLAYER_ASSET_MESSAGE_BUFFER_RAM;
        CopyBytes(message_text, (void *)0x08103AF4, 23);
        zero = 0;
        marker = EVENT_PLAYER_ASSET_MESSAGE_HIGHLIGHT_PREFIX;
        message_text[11] = marker;
        money_amount = EVENT_COMMAND_BYTE(*cursor, EventPlayerMoneyCommand, hundreds);
        money_unit = EVENT_PLAYER_MONEY_UNIT;
        money_amount *= money_unit;
        message_cursor = message_text + 12;
        FormatNumberText(money_amount, 7, 0, message_cursor);
        glyph_count_or_pilot_slot = (u8)CountEncodedTextGlyphs(message_cursor);
        CopyBytes((u16 *)((glyph_count_or_pilot_slot * 2) + (u32)message_cursor),
            (void *)0x08103D18, 5);
        marker = glyph_count_or_pilot_slot + 13;
        message_text[marker] = 1;
        marker = glyph_count_or_pilot_slot + 14;
        message_text[marker] = zero;
        *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = message_text;
        RunMenuScript(0x080177D5);
        AddPlayerMoney(EVENT_COMMAND_BYTE(*cursor, EventPlayerMoneyCommand, hundreds) * money_unit);
        break;
    }
    default:
        break;
    }
    StopSong(EVENT_PLAYER_ASSET_SOUND);
    PlayOrContinueSong(*(u8 *)0x02030667);
    SeekEventCommand(saved_script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
