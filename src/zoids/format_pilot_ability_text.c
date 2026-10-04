#include "m2c_prelude.h"
#include "../game/player_state.h"

void FormatNumberText(s32, s32, s32, s32) asm("func_08098284");
void AppendString(s32, s32) asm("func_08099F5C");
void CopyString(s32, s32) asm("func_080ED128");

extern s32 gPilotAbilityNameTable[] asm("D_087EF2D0");
extern s32 gZoidProficiencyNames asm("D_087A5962");
extern s32 gPilotAbilityNumberText asm("D_020305E4");
extern s32 gPilotAbilityPercentSuffix asm("D_08109400");

void FormatPilotAbilityText(u8 ability_kind, u16 ability_value, s32 destination_text) asm("func_080E7664");

void FormatPilotAbilityText(u8 ability_kind, u16 ability_value, s32 destination_text) {
    u8 kind;
    u16 stored_value;
    s32 proficiency_name_address;

    kind = ability_kind;
    stored_value = ability_value;
    CopyString(destination_text, gPilotAbilityNameTable[kind]);

    switch (kind - PILOT_ABILITY_ZOID_PROFICIENCY_1) {
    case PILOT_ABILITY_ZOID_PROFICIENCY_1 - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_ZOID_PROFICIENCY_2 - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_ZOID_PROFICIENCY_3 - PILOT_ABILITY_ZOID_PROFICIENCY_1:
        proficiency_name_address = (s16)stored_value * 0x23 + (s32)&gZoidProficiencyNames;
        AppendString(destination_text, proficiency_name_address);
        break;
    case PILOT_ABILITY_RANGED_ACCURACY_PENALTY - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_MELEE_ACCURACY_PENALTY - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case 22:
    case 23:
    case PILOT_ABILITY_MISSILE_ACCURACY_PENALTY - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_LASER_ACCURACY_PENALTY - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_PARTICLE_ACCURACY_PENALTY - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_BALLISTIC_ACCURACY_PENALTY - PILOT_ABILITY_ZOID_PROFICIENCY_1:
        FormatNumberText(-(s16)stored_value, 4, 4, (s32)&gPilotAbilityNumberText);
        AppendString(destination_text, (s32)&gPilotAbilityNumberText);
        AppendString(destination_text, (s32)&gPilotAbilityPercentSuffix);
        break;
    case PILOT_ABILITY_MELEE_EVASION_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_RANGED_EVASION_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_MELEE_ACCURACY_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case 31:
    case 32:
    case PILOT_ABILITY_MISSILE_ACCURACY_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_LASER_ACCURACY_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_PARTICLE_ACCURACY_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
    case PILOT_ABILITY_BALLISTIC_ACCURACY_BONUS - PILOT_ABILITY_ZOID_PROFICIENCY_1:
        FormatNumberText((s16)stored_value, 4, 4, (s32)&gPilotAbilityNumberText);
        AppendString(destination_text, (s32)&gPilotAbilityNumberText);
        AppendString(destination_text, (s32)&gPilotAbilityPercentSuffix);
        break;
    }
}
