#include "m2c_prelude.h"
#include "../../game/game_state.h"
#include "../../game/player_state.h"
#include "../event_script.h"

extern void PlayOrContinueSong(u8) asm("func_08092E74");
extern void PlaySong(int) asm("func_08092E84");
extern void StopSong(u8) asm("func_08092EA0");
extern void DestroySprite(void) asm("func_08094554");
extern u8 CountEncodedTextGlyphs(int) asm("func_08098B58");
extern void RunMenuScript(int) asm("func_08098BB4");
extern int SeekEventCommand(u8, int, int) asm("func_080A016C");
extern void AddPulseEffect(u8, s16) asm("func_080E7868");
extern void CopyBytes(void *, int, int) asm("func_080ED038");
extern void CopyString(void *, int) asm("func_080ED128");

extern s32 gEventPortraitSprite asm("D_02031744");
extern u8 gEventDialogueMenuState asm("D_02030666");
extern u16 gEventMessageBuffer[] asm("D_02031756");
extern s32 gAuxiliaryEffectNameTable[] asm("D_087EF410");

s32 EventGrantPulseEffect(u8 script_slot, u8 **command_cursor) asm("func_080A5264");

s32 EventGrantPulseEffect(u8 script_slot, u8 **command_cursor) {
    u8 previous_menu_state;
    u8 effect_name_glyph_count;
    u8 *effect_command_bytes;

    StopSong(*(u8 *)0x02030667);
    PlaySong(EVENT_PULSE_GROWTH_SOUND);
    *(u8 *)FIELD_EVENT_ACTIVE_RAM = 1;
    if (gEventPortraitSprite != 0) {
        DestroySprite();
        gEventPortraitSprite = 0;
    }
    if ((*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) || (*(u8 *)0x02031748 != 0)) {
        previous_menu_state = gEventDialogueMenuState;
        if (previous_menu_state != 0) {
            if (previous_menu_state == 1) {
                RunMenuScript(0x080177F5);
                goto open_skill_message_menu;
            }
        } else {
open_skill_message_menu:
            RunMenuScript(0x080177ED);
            gEventDialogueMenuState = 2;
        }
    }
    CopyBytes(gEventMessageBuffer, 0x08103DD8, 0x15);
    gEventMessageBuffer[10] = 0x201;
    CopyString(&gEventMessageBuffer[11], gAuxiliaryEffectNameTable[(*command_cursor)[EVENT_COMMAND_OFFSET(EventPulseEffectCommand, effect_kind)]]);
    effect_name_glyph_count = CountEncodedTextGlyphs(gAuxiliaryEffectNameTable[(*command_cursor)[EVENT_COMMAND_OFFSET(EventPulseEffectCommand, effect_kind)]]);
    gEventMessageBuffer[effect_name_glyph_count + 11] = 1;
    CopyBytes(&gEventMessageBuffer[12] + effect_name_glyph_count, 0x08103DF0, 0xE);
    *(u16 **)EVENT_PLAYER_ASSET_MESSAGE_POINTER_RAM = gEventMessageBuffer;
    RunMenuScript(0x080177D5);
    effect_command_bytes = *command_cursor;
    AddPulseEffect(effect_command_bytes[EVENT_COMMAND_OFFSET(EventPulseEffectCommand, effect_kind)], (s16)(effect_command_bytes[EVENT_COMMAND_OFFSET(EventPulseEffectCommand, effect_value_low)] | (effect_command_bytes[EVENT_COMMAND_OFFSET(EventPulseEffectCommand, effect_value_high)] << 8)));
    StopSong(EVENT_PULSE_GROWTH_SOUND);
    PlayOrContinueSong(*(u8 *)0x02030667);
    SeekEventCommand(script_slot, EVENT_SCAN_NEXT, 0);
    return EVENT_CONTINUE;
}
