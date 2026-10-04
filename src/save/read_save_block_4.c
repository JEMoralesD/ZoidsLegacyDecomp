#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 ReadSaveBlock4(void) { return TransferSaveBlock(1, 4, 0); }
