#include "player_selection.h"

extern void PlaySong(s32) asm("func_08092E84");
extern void *CreateSprite(void *, void *, s32, s32, s32, s32, s32, s32, s32) asm("func_08094484");
extern void DestroySprite(void *) asm("func_08094554");
extern void ResetMenuKeyRepeat(void) asm("func_08096F3C");
extern void RequestWindowRefresh(void) asm("func_080972C8");
extern void ClearWindow(s32) asm("func_080986B4");
extern void RunMenuScript(s32) asm("func_08098BB4");
extern void YieldTaskForUpdates(s32) asm("func_080ED17C");

void RunEquipmentStatHelpMenu(u8 equipment_id) asm("func_080B7210");

void RunEquipmentStatHelpMenu(u8 equipment_id) {
    register u32 selection asm("r5");
    register u32 displayed asm("r8");
    register u8 selected_equipment_id asm("r6");
    void *cursor_sprite;
    void *scroll_arrow_sprite;
    selected_equipment_id = equipment_id;
    RunMenuScript(0x080048DE);
    selection = 0;
    displayed = 1;
    cursor_sprite = CreateSprite((void *)0x080ED830, (void *)0x080ED864, 0,
                          120, *(u8 *)EQUIPMENT_STAT_HELP_CURSOR_Y_ROM, 0x3EE, 15, 32, selection);
    scroll_arrow_sprite = CreateSprite((void *)0x08105A20, (void *)0x08105A2C, 0,
                                 216, 152, 0x343, 15, 8, selection);
    ResetMenuKeyRepeat();

    do {
        if (selection != displayed) {
            register struct EquipmentRecord *equipment_records asm("r3") = (struct EquipmentRecord *)0x087B2524;
            register u32 equipment_scale asm("r0");
            register u32 equipment_offset asm("r2");

            equipment_scale = (u32)selected_equipment_id * 3;
            equipment_offset = equipment_scale << 3;
            if ((*(u16 *)((u8 *)equipment_records + 2 + equipment_offset) & 1) == 0) {
                register s32 *messages asm("r0") = (s32 *)WEAPON_STAT_HELP_SCRIPTS_ROM;
                register u32 selected asm("r1");

                asm("" : : "r"(messages));
                selected = selection << 2;
                selected += (u32)messages;
                RunMenuScript(*(s32 *)selected);
            } else if (selection <= WEAPON_HELP_ACCURACY) {
                register u32 effect_kind_address asm("r0");
                u8 command_effect_kind;
                s32 message;
                register u32 effect_kind_index asm("r0");

                effect_kind_address = (u32)equipment_records + 4;
                effect_kind_address = equipment_offset + effect_kind_address;
                command_effect_kind = *(u8 *)effect_kind_address;

                if (command_effect_kind != 0) {
                    register s32 *messages asm("r2") = (s32 *)COMMAND_EFFECT_STAT_HELP_SCRIPTS_ROM;
                    register u32 selected asm("r1");

                    asm("" : : "r"(messages));
                    selected = selection << 2;
                    effect_kind_index = (u32)command_effect_kind - 1;
                    effect_kind_index <<= 3;
                    selected += effect_kind_index;
                    selected += (u32)messages;
                    message = *(s32 *)selected;
                    if (message != 0) {
                        RunMenuScript(message);
                        goto message_done;
                    }
                }
                {
                    ClearWindow(7);
                    RequestWindowRefresh();
                }
            } else {
                register s32 *messages asm("r0") = (s32 *)COMMAND_COMMON_STAT_HELP_SCRIPTS_ROM;
                register u32 selected asm("r1");

                asm("" : : "r"(messages));
                selected = (selection - WEAPON_HELP_RANGE_AND_AREA) << 2;
                selected += (u32)messages;
                RunMenuScript(*(s32 *)selected);
            }
message_done:
            {
                register u32 selection_address asm("r0") = EQUIPMENT_STAT_HELP_CURSOR_Y_ROM;

                asm("" : "+r"(selection_address));
                selection_address = selection + selection_address;
                *(u16 *)((u8 *)cursor_sprite + 6) = *(u8 *)selection_address;
            }
            displayed = selection;
        }
        YieldTaskForUpdates(1);
        if ((*(volatile u16 *)0x03006034 & 0x40) != 0 && selection != 0) {
            register u32 next asm("r0") = selection - 1;

            next <<= 24;
            selection = next >> 24;
            PlaySong(0x40);
        }
        if ((*(volatile u16 *)0x03006034 & 0x80) != 0 && selection <= WEAPON_HELP_CLASSIFICATION) {
            register u32 next asm("r0") = selection + 1;

            next <<= 24;
            selection = next >> 24;
            PlaySong(0x40);
        }
    } while ((*(volatile u16 *)0x0300000E & 2) == 0);

    DestroySprite(cursor_sprite);
    DestroySprite(scroll_arrow_sprite);
    RunMenuScript(0x080048F6);
    PlaySong(63);
}
