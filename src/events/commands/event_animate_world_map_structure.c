#include "m2c_prelude.h"
#include "../event_script.h"
#include "../../game/player_state.h"
void SetSpriteAnimation(s32 *, s32) asm("func_08094564");
void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
void YieldTaskForUpdates(s32) asm("func_080ED17C");

s32 EventAnimateWorldMapStructure(u8 script_slot) asm("func_080A6424");

s32 EventAnimateWorldMapStructure(u8 script_slot) {
    s32 *structure_sprite;
    s32 *structure_sprite_reloaded;
    u8 elapsed_updates;

    *(u8 *)0x02030664 = 1;
    structure_sprite = *(s32 **)0x020314A0;
    *structure_sprite = (*structure_sprite & ~0x38) | 0x20;
    SetSpriteAnimation(structure_sprite, 2);
    elapsed_updates = 0;
    do {
        elapsed_updates += 1;
        YieldTaskForUpdates(1);
    } while ((u32) elapsed_updates <= 0x3B);
    structure_sprite_reloaded = *(s32 **)0x020314A0;
    *structure_sprite_reloaded = (*structure_sprite_reloaded & ~0x30) | 8;
    SetSpriteAnimation(structure_sprite_reloaded, 1);
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
