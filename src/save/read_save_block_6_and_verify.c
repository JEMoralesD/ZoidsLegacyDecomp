#include "m2c_prelude.h"
int TransferSaveBlock() asm("func_8093EC8");
u8 ReadSaveBlock6(void) { return TransferSaveBlock(1, 6, 0); }
u8 VerifySaveBlock0(void) { return TransferSaveBlock(2, 0, 0); }
u8 VerifySaveBlock1(void) { return TransferSaveBlock(2, 1, 0); }
u8 VerifySaveBlock2(void) { return TransferSaveBlock(2, 2, 0); }
u8 VerifySaveBlock3(void) { return TransferSaveBlock(2, 3, 0); }
u8 VerifySaveBlock4(void) { return TransferSaveBlock(2, 4, 0); }
u8 VerifySaveBlock5(void) { return TransferSaveBlock(2, 5, 0); }
u8 VerifySaveBlock6(void) { return TransferSaveBlock(2, 6, 0); }
