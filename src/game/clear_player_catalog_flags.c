#include "m2c_prelude.h"
#include "player_state.h"
M2C_UNK BiosCpuSet(M2C_UNK *, M2C_UNK, M2C_UNK) asm("func_080ECD2C");

void ClearPlayerCatalogFlags(void) asm("func_08099FCC");

void ClearPlayerCatalogFlags(void) {
    s32 clear_source = 0;

    BiosCpuSet(&clear_source, PLAYER_CATALOG_FLAGS_RAM, PLAYER_CATALOG_CLEAR_CONTROL);
}
