#include "m2c_prelude.h"
#include "game_state.h"
void ClearGameCompletionFlag(void) asm("func_08099F80");

void ClearGameCompletionFlag(void) {
    *(s8 *)GAME_COMPLETION_FLAG_RAM = 0;
}
