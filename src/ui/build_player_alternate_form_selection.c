#include "player_selection.h"
u8 GetZoidFormIndex(u8) asm("func_080E523C");
void BuildPlayerAlternateFormSelection(void *zoid_record) asm("func_080B6768");

void BuildPlayerAlternateFormSelection(void *zoid_record) {
    u8 form_index;
    u8 *selected_form_ids;
    *(u8 *)PLAYER_FORM_SELECTION_COUNT_RAM = 0;
    form_index = 0;
    selected_form_ids = (u8 *)PLAYER_FORM_SELECTION_IDS_RAM;
    do {
        if ((((s32) M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(form_flags)) >> form_index) & 1) && (GetZoidFormIndex(M2C_FIELD(zoid_record, u8 *, PLAYER_ZOID_OFFSET(model_id))) != form_index)) {
            *(u8 *)(*(u8 *)PLAYER_FORM_SELECTION_COUNT_RAM + (s32)selected_form_ids) = form_index;
            *(u8 *)PLAYER_FORM_SELECTION_COUNT_RAM += 1;
        }
        form_index += 1;
    } while ((u32) form_index <= 5U);
}
