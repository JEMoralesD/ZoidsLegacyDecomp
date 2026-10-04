#include "m2c_prelude.h"
#include "name_entry.h"
M2C_UNK PrintWindowTextAt(s32, s32, s32, u8, s32) asm("func_080981F0");          /* extern */

void PrintNameEntryText(void) asm("func_0809C45C");

void PrintNameEntryText(void) {
    PrintWindowTextAt(*(s32 *)NAME_ENTRY_TEXT_BUFFER_RAM, 0, 0, *(u8 *)NAME_ENTRY_TEXT_COLUMN_RAM, 0);
}
