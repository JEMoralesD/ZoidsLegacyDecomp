#include "m2c_prelude.h"
struct DeferredFunctionCall {
    s32 function_address;
    s32 argument;
};

M2C_UNK CallFunctionR2(s32, s32, s32) asm("func_080ECD64");               /* extern */

void RunDeferredFunctionCall(void *request) asm("func_080929B0");

void RunDeferredFunctionCall(void *request) {
    s32 argument;

    argument = ((struct DeferredFunctionCall *)request)->argument;
    CallFunctionR2(argument, argument, ((struct DeferredFunctionCall *)request)->function_address);
}
