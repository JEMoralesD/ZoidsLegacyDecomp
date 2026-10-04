#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 WriteSaveBlock4(void) { return TransferSaveBlock(0, 4, 0); }
