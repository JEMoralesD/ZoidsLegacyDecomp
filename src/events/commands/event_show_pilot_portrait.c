#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
#include "../../game/game_state.h"

extern void QueuePilotPortraitGraphics(s32, s32, s32, s32, s32, s32) asm("func_0809A9C8");
extern void *CreateSprite(s32, s32, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");

s32 EventShowPilotPortrait(u8 script_slot, struct EventPilotPortraitCommand **script_cursor) asm("func_080A1550");

s32 EventShowPilotPortrait(u8 script_slot, struct EventPilotPortraitCommand **script_cursor) {
    register u8 saved_script_slot asm("r8") = script_slot;
    if (*(s32 *)0x02021690 != GAME_MODE_BATTLE_SCENE) {
        register struct EventPilotPortraitCommand *command asm("r0") = *script_cursor;
        register s32 pilot_id asm("r3") = command->pilot_id;
        register s32 portrait_variant asm("r1") = command->portrait_variant;
        register s32 pulse_portrait_variant asm("r2");
        register s32 tile_offset asm("r4");
        register s32 palette_bank asm("r5");
        register s32 *portrait_sprite_address asm("r6");

        if (pilot_id == 75) {
            register u32 player_state_address asm("r0") = 0x020218E4;
            register u32 pulse_portrait_offset asm("r2") = 0x6809;
            asm volatile("" : "+r"(player_state_address), "+r"(pulse_portrait_offset));
            pulse_portrait_variant = *(u8 *)(player_state_address + pulse_portrait_offset);
        } else {
            pulse_portrait_variant = 0;
        }
        tile_offset = 0x3C2;
        palette_bank = 14;
        QueuePilotPortraitGraphics(pilot_id, portrait_variant, pulse_portrait_variant, tile_offset, palette_bank,
            0x02002880);
        portrait_sprite_address = (s32 *)0x02031744;
        if (*portrait_sprite_address == 0) {
            *portrait_sprite_address = (s32)CreateSprite(0x08359850, 0x0835985C, 0, 8,
                104, tile_offset, palette_bank, 8, 0);
        }
    } else {
        register struct EventPilotPortraitCommand *command asm("r0") = *script_cursor;
        register s32 pilot_id asm("r3") = command->pilot_id;
        register s32 portrait_variant asm("r1") = command->portrait_variant;
        register s32 pulse_portrait_variant asm("r2");
        s32 *portrait_sprite_address;
        register s32 tile_offset asm("r5");
        register s32 palette_bank asm("r6");
        register s32 existing_sprite asm("r4");

        if (pilot_id == 75) {
            register u32 player_state_address asm("r0") = 0x020218E4;
            register u32 pulse_portrait_offset asm("r2") = 0x6809;
            asm volatile("" : "+r"(player_state_address), "+r"(pulse_portrait_offset));
            pulse_portrait_variant = *(u8 *)(player_state_address + pulse_portrait_offset);
        } else {
            pulse_portrait_variant = 0;
        }
        tile_offset = 0x3DC;
        palette_bank = 15;
        QueuePilotPortraitGraphics(pilot_id, portrait_variant, pulse_portrait_variant, tile_offset, palette_bank,
            0x02002880);
        portrait_sprite_address = (s32 *)0x02031744;
        existing_sprite = *portrait_sprite_address;
        if (existing_sprite == 0) {
            if (*(u8 *)0x02031748 == 0) {
                if (*(u8 *)0x02033F36 == 0) {
                    *portrait_sprite_address = (s32)CreateSprite(0x08359850, 0x0835985C,
                        0, -1, 104 + 10, tile_offset, palette_bank, 8, existing_sprite);
                } else {
                    *portrait_sprite_address = (s32)CreateSprite(0x08359850, 0x0835985C,
                        0, 193, 104 + 10, tile_offset, palette_bank, 8, existing_sprite);
                }
            } else {
                *portrait_sprite_address = (s32)CreateSprite(0x08359850, 0x0835985C, 0, 8,
                    104, tile_offset, palette_bank, 8, existing_sprite);
            }
        }
    }
    {
        register s32 next_command_selector asm("r1") = 1;
        register u8 script_slot_r0 asm("r0");

        next_command_selector = -next_command_selector;
        script_slot_r0 = saved_script_slot;
        SeekEventCommand(script_slot_r0, next_command_selector, 0);
    }
    return 0;
}
