#include "m2c_prelude.h"
M2C_UNK CopyString(u8 *) asm("func_80ED128");

u8 *AppendString(u8 *destination) asm("func_08099F5C");

u8 *AppendString(u8 *destination) {
    register u8 *terminator asm("r2") = destination;
    u8 *original_destination = terminator;
    if (*original_destination != 0) {
        do {
            terminator += 1;
        } while (*terminator != 0);
    }
    /* CopyString reads the source pointer already held in r1. */
    CopyString(terminator);
    return original_destination;
}
