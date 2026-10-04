#include "m2c_prelude.h"
#include "../game/player_state.h"
#include "../events/event_script.h"

void CopyBytes(void *, void *, s32) asm("func_080ED038");
void ClearEventFlag(s32) asm("func_0809F7F0");

extern u8 gBackupPlayerPilots[] asm("D_0202DD80");
extern u8 gPlayerPilots[] asm("D_02027378");
extern u8 gCurrentPlayerState[] asm("D_020218E4");
extern u8 gBackupPlayerState[] asm("D_020282EC");
extern u8 gFieldSaveState[] asm("D_0202ECF4");

void RestoreBitFromPlayerBackup(void) asm("func_080BB7EC");

void RestoreBitFromPlayerBackup(void)
{
    register u8 *current_pilot_table_or_destination asm("r5");
    register u8 *backup_pilot asm("r2");
    register u32 pilot_slot asm("r4");
    u8 *backup_pilot_records;
    register u8 *scan asm("r0");
    register u8 *current_pilot_destination asm("r3");
    u8 first_backup_pilot_id;

    pilot_slot = 1;
    backup_pilot_records = gBackupPlayerPilots;
    scan = backup_pilot_records;
    scan += 0x40;
    backup_pilot = scan;
    first_backup_pilot_id = *scan;
    current_pilot_table_or_destination = gPlayerPilots;
    if (first_backup_pilot_id != BIT_PILOT_ID) {
        do {
            register u32 next asm("r0");
            next = pilot_slot + 1;
            next <<= 24;
            pilot_slot = next >> 24;
            if (pilot_slot > 0x34) {
                break;
            }
            scan = (u8 *)(pilot_slot * 0x40);
            scan += (u32)backup_pilot_records;
            backup_pilot = scan;
        } while (*scan != BIT_PILOT_ID);
    }

    pilot_slot = 1;
    scan = current_pilot_table_or_destination;
    scan += 0x40;
    current_pilot_destination = scan;
    if (((*(u16 *)(scan + 2) & 0xFF00) >> 8) != BIT_PILOT_ID) {
        do {
            register u32 next asm("r0");
            next = pilot_slot + 1;
            next <<= 24;
            pilot_slot = next >> 24;
            if (pilot_slot > 0x34) {
                break;
            }
            scan = (u8 *)(pilot_slot * 0x40);
            scan += (u32)current_pilot_table_or_destination;
            current_pilot_destination = scan;
        } while (((*(u16 *)(scan + 2) & 0xFF00) >> 8) != BIT_PILOT_ID);
    }

    {
        register u8 *backup_pilot_source asm("r6");
        u16 current_pilot_flags;
        current_pilot_table_or_destination = current_pilot_destination;
        backup_pilot_source = backup_pilot;
        CopyBytes(current_pilot_table_or_destination, backup_pilot, 0x40);
        current_pilot_flags = *(u16 *)(current_pilot_table_or_destination + 2);
        {
            u32 mask = 0xFFFB;
            register u16 masked_current_pilot_flags asm("r1");
            masked_current_pilot_flags = mask;
            asm volatile("" : "+r"(masked_current_pilot_flags));
            masked_current_pilot_flags &= current_pilot_flags;
            *(u16 *)(current_pilot_table_or_destination + 2) = masked_current_pilot_flags;
            if (backup_pilot_source[1] != 0) {
                register u8 *current_player_state asm("r4");
                register u8 *backup_player_state asm("r3");
                register u32 current_zoid_address asm("r0");
                register u32 backup_zoid_address asm("r1");
                current_player_state = gCurrentPlayerState;
                current_zoid_address = current_pilot_table_or_destination[1] * 0x70;
                current_zoid_address += (u32)current_player_state;
                backup_player_state = gBackupPlayerState;
                backup_zoid_address = backup_pilot_source[1] * 0x70;
                backup_zoid_address += (u32)backup_player_state;
                current_zoid_address += 4;
                backup_zoid_address += 4;
                CopyBytes((void *)current_zoid_address,
                               (void *)backup_zoid_address, 0x70);
                {
                    register u8 *current_zoid_slot_header asm("r1");
                    register u16 current_zoid_flags asm("r2");
                    register u16 masked_current_zoid_flags asm("r0");
                    current_zoid_slot_header = (u8 *)(current_pilot_table_or_destination[1] * 0x70 + (u32)current_player_state);
                    current_zoid_flags = *(u16 *)(current_zoid_slot_header + 8);
                    masked_current_zoid_flags = mask;
                    asm volatile("" : "+r"(masked_current_zoid_flags));
                    masked_current_zoid_flags &= current_zoid_flags;
                    *(u16 *)(current_zoid_slot_header + 8) = masked_current_zoid_flags;
                }
                current_player_state[1]++;
            }
        }
    }
    ClearEventFlag(0x89);
    ClearEventFlag(0xCB);
    ClearEventFlag(0xCC);
    ClearEventFlag(0xCD);
    ClearEventFlag(0xCE);
    ClearEventFlag(0xCF);
    gFieldSaveState[0x21] = 0;
}
