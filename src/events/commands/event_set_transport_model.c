#include "m2c_prelude.h"
#include "../event_script.h"
extern void SeekEventCommand(u8, s32, s32) asm("func_080A016C");
extern s32 gPlayerCatalogFlags[] asm("D_020217B4");
extern u8 gFieldSaveState[] asm("D_0202ECF4");

s32 EventSetTransportModel(u8 script_slot, void **script_cursor) asm("func_080A47D4");

s32 EventSetTransportModel(u8 script_slot, void **script_cursor) {
    s32 *catalog_words;
    register s32 catalog_bits asm("r1");
    register s32 model_catalog_bit asm("r2");
    u8 model_id;
    gFieldSaveState[2] = EVENT_COMMAND_BYTE(*script_cursor, EventTransportModelCommand, model_id);
    if (EVENT_COMMAND_BYTE(*script_cursor, EventTransportModelCommand, model_id) == 0x6C) {
        gFieldSaveState[3] = 1;
    }
    model_id = EVENT_COMMAND_BYTE(*script_cursor, EventTransportModelCommand, model_id);
    switch (model_id) {
    case 0x69:
        catalog_words = gPlayerCatalogFlags;
        catalog_bits = catalog_words[4];
        model_catalog_bit = 0x80000;
        goto record_transport_model;
    case 0x6A:
        catalog_words = gPlayerCatalogFlags;
        catalog_bits = catalog_words[4];
        model_catalog_bit = 0x100000;
        goto record_transport_model;
    case 0x6B:
        catalog_words = gPlayerCatalogFlags;
        catalog_bits = catalog_words[4];
        model_catalog_bit = 0x400000;
        goto record_transport_model;
    case 0x6C:
        catalog_words = gPlayerCatalogFlags;
        catalog_bits = catalog_words[4];
        model_catalog_bit = 0x200000;
record_transport_model:
        catalog_words[4] = catalog_bits | model_catalog_bit;
        break;
    }
    SeekEventCommand(script_slot, -1, 0);
    return 0;
}
