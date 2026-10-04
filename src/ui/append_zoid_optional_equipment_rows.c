#include "player_selection.h"

M2C_UNK AppendWindowTextItem(s32, s32) asm("func_080988C8");
M2C_UNK RunMenuScript(M2C_UNK) asm("func_08098BB4");
M2C_UNK AppendString(s32, s32) asm("func_08099F5C");
M2C_UNK BuildZoidEquipmentStats(s32, M2C_UNK, s32, u8, void *) asm("func_080E59BC");
void *AcquireEquipmentStatBuffer(void) asm("func_080E669C");
M2C_UNK ReleaseEquipmentStatBuffer(void) asm("func_080E66B8");
M2C_UNK CopyBytes(void *, M2C_UNK, s32) asm("func_080ED038");

void AppendZoidOptionalEquipmentRows(s32 zoid_address, M2C_UNK pilot_address) asm("func_080B9080");

void AppendZoidOptionalEquipmentRows(s32 zoid_address, M2C_UNK pilot_address)
{
    register M2C_UNK text asm("r1");
    register s32 saved_zoid_address asm("r8") = zoid_address;
    register M2C_UNK saved_pilot_address asm("sl") = pilot_address;
    s32 row;
    u8 index;
    void *equipment_stats;
    u16 *buffer;
    register void *line_output asm("r9");
    register void *output asm("r0");

    equipment_stats = AcquireEquipmentStatBuffer();
    RunMenuScript(0x0800586B);
    index = 0;
    buffer = (u16 *)0x02030564;
    asm volatile("" : "+r"(buffer));
    asm volatile("" : : "r"(buffer));
    line_output = buffer + 1;
    do {
        register u32 raw_row asm("r0") = index + 4;
        register u32 field asm("r1") = raw_row << 2;

        field += saved_zoid_address;
        field += PLAYER_ZOID_OFFSET(equipment[0].item_id);
        field = *(u16 *)field;
        row = raw_row;
        asm volatile("" : : "r"(row), "r"(row));
        if (field != 0) {
            BuildZoidEquipmentStats(saved_zoid_address, saved_pilot_address, 0, row, equipment_stats);
            {
                register u32 flags asm("r1") = EQUIPMENT_RECORD_FIELD(equipment_stats, u16, flags);
                register u32 one asm("r3") = 1;
                register u32 test asm("r0") = one;

                test &= flags;
                if (test == 0) {
                    register u16 *destination asm("r2") =
                        (u16 *)0x02030564;

                    *destination = one;
                if (!(EQUIPMENT_RECORD_FIELD(equipment_stats, s32, attributes) & 0x10)) {
                    output = destination + 1;
                    text = OPTIONAL_RANGED_WEAPON_TAG_ROM;
                } else {
                    output = destination + 1;
                    text = OPTIONAL_MELEE_WEAPON_TAG_ROM;
                }
                } else {
                    *buffer = 0x401;
                    output = buffer + 1;
                    text = OPTIONAL_COMMAND_TAG_ROM;
                }
            }
            CopyBytes(output, text, 7);
        } else {
            *buffer = 0x401;
            CopyBytes(buffer + 1, OPTIONAL_EMPTY_SLOT_TAG_ROM, 7);
        }
        AppendString((s32)line_output,
            M2C_FIELD((M2C_FIELD(((row * 4) + saved_zoid_address), u16 *, PLAYER_ZOID_OFFSET(equipment[0].item_id)) * 4),
                s32 *, 0x087EE170));
        {
            register s32 call0 asm("r0") = 8;
            register s32 call1 asm("r1") = (s32)line_output;

            call1 -= 2;
            AppendWindowTextItem(call0, call1);
        }
        index += 1;
    } while ((u32)index <= 3);
    ReleaseEquipmentStatBuffer();
}
