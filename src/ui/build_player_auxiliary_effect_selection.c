#include "player_selection.h"
void BuildPlayerAuxiliaryEffectSelection(u8 *auxiliary_record) asm("func_080B6810");

void BuildPlayerAuxiliaryEffectSelection(u8 *auxiliary_record) {
    u8 effect_kind;
    u8 effect_index;
    register u8 *selected_count asm("r2");
    u8 *effect_kinds;
    u8 *selected_kinds;
    u8 *kind_output;

    selected_count = (u8 *)PLAYER_AUXILIARY_EFFECT_SELECTION_COUNT_RAM;
    *selected_count = 0;
    effect_index = 0;
    effect_kinds = auxiliary_record + PLAYER_AUXILIARY_PILOT_OFFSET(effect_kinds);
    selected_kinds = (u8 *)PLAYER_AUXILIARY_EFFECT_SELECTION_KINDS_RAM;
    do {
        effect_kind = effect_kinds[effect_index];
        if (effect_kind != 0) {
            kind_output = (u8 *)(u32)*selected_count;
            kind_output += (u32)selected_kinds;
            *kind_output = effect_kind;
            *selected_count += 1;
        }
        effect_index += 1;
    } while ((u32)effect_index <= 9U);
}
