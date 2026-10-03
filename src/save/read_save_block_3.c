#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 ReadSaveBlock3(void) { return TransferSaveBlock(1, 3, 0); }
