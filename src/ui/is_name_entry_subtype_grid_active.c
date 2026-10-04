#include "m2c_prelude.h"
#include "name_entry.h"
s32 IsNameEntrySubtypeGridActive(void) asm("func_0809C434");

s32 IsNameEntrySubtypeGridActive(void) {
    if ((*(u8 *)NAME_ENTRY_CATEGORY_RAM == NAME_ENTRY_SUBTYPE_CATEGORY) && (*(u8 *)NAME_ENTRY_SUBTYPE_RAM != NAME_ENTRY_NO_SUBTYPE)) {
        return 1;
    }
    return 0;
}
