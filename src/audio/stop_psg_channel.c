#include "sound_engine.h"
void StopPsgChannel(u8 channel_id) asm("func_080EC188");

void StopPsgChannel(u8 channel_id) {
    s8 *channel_register;
    s8 register_value;

    switch (channel_id) {
    case 1:
        channel_register = (s8 *)0x04000063;
        *channel_register = PSG_INITIAL_ENVELOPE;
        channel_register += 2;
        goto write_restart;
    case 2:
        channel_register = (s8 *)0x04000069;
        goto write_envelope;
    case 3:
        channel_register = (s8 *)0x04000070;
        register_value = 0;
        goto write_register;
    default:
        channel_register = (s8 *)0x04000079;
write_envelope:
        *channel_register = PSG_INITIAL_ENVELOPE;
        channel_register += 4;
write_restart:
        register_value = PSG_CHANNEL_RESTART;
write_register:
        *channel_register = register_value;
    }
}
