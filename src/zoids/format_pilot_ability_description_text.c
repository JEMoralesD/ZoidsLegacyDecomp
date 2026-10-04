#include "m2c_prelude.h"
#include "../game/player_state.h"
void AppendString(s8 *, s32) asm("func_08099F5C");
void CopyBytes(s8 *, s32, s32) asm("func_080ED038");
extern s32 gPilotProficiencyDescriptionPrefix asm("D_08109404");
extern s32 gPilotProficiencyDescriptions asm("D_087A5B29");
extern s32 gPilotAbilityDescriptionTable[] asm("D_087EF370");

void FormatPilotAbilityDescriptionText(s32 ability_kind_word, s32 ability_value_word, s8 *destination_text) asm("func_080E77A4");

void FormatPilotAbilityDescriptionText(s32 ability_kind_word, s32 ability_value_word, s8 *destination_text) {
    register s32 ability_kind asm("r5");
    register s32 stored_ability_value asm("r6");

    ability_kind_word = ability_kind_word << 24;
    ability_kind = (u32)ability_kind_word >> 24;
    ability_value_word = ability_value_word << 16;
    stored_ability_value = (u32)ability_value_word >> 16;

    *destination_text = 0;
    switch (ability_kind) {
    case PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_ZOID_PROFICIENCY_2:
    case PILOT_ABILITY_ZOID_PROFICIENCY_3: {
        register s32 signed_proficiency_index asm("r0");

        CopyBytes(destination_text, (s32)&gPilotProficiencyDescriptionPrefix, PILOT_PROFICIENCY_DESCRIPTION_PREFIX_BYTES);
        signed_proficiency_index = stored_ability_value << 16;
        signed_proficiency_index >>= 16;
        asm volatile("" : "+r"(signed_proficiency_index));
        AppendString(destination_text, signed_proficiency_index * PILOT_PROFICIENCY_DESCRIPTION_STRIDE + (s32)&gPilotProficiencyDescriptions);
        break;
    }
    }
    {
        register s32 *description_table asm("r0");
        register s32 description_address asm("r1");

        description_table = gPilotAbilityDescriptionTable;
        asm volatile("" : "+r"(description_table));
        description_address = ability_kind << 2;
        asm volatile("" : "+r"(description_address));
        description_address += (s32)description_table;
        asm volatile("" : "+r"(description_address));
        description_address = *(s32 *)description_address;
        asm volatile("" : "+r"(description_address));
        AppendString(destination_text, description_address);
    }
}
