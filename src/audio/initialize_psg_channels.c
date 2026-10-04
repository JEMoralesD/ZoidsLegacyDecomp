#include "sound_engine.h"

void BiosCpuSet(void *, void *, void *) asm("func_80ECD2C");
extern u8 D_080EC901, D_080EB605, D_080EB619, D_080ECA59, D_080EB59D, D_080EBB9D,
    D_080EB329, D_080EBF65, D_080EC02D, D_080EC241, D_080EC189, D_080EC0E1, D_zero;

void InitializePsgChannels(void *psg_channels) asm("func_080EB98C");

void InitializePsgChannels(void *psg_channels) {
    void *sound_state;
    s32 signature;
    s32 clear_source;
    register s32 zero asm("r1");
    register s32 zero_symbol asm("r0");
    u8 *sound_register;
    register s16 *psg_volume_control asm("r3");
    s32 *command_handlers;
    u8 *channel_field;

    *(s16 *)0x04000084 = SOUND_INITIAL_MASTER_CONTROL;
    psg_volume_control = (s16 *)0x04000080;
    *psg_volume_control = 0;
    sound_register = (u8 *)0x04000063;
    sound_register[0] = PSG_INITIAL_ENVELOPE;
    sound_register += 6;
    sound_register[0] = PSG_INITIAL_ENVELOPE;
    sound_register += 0x10;
    sound_register[0] = PSG_INITIAL_ENVELOPE;
    sound_register -= 0x14;
    sound_register[0] = PSG_CHANNEL_RESTART;
    sound_register += 8;
    sound_register[0] = PSG_CHANNEL_RESTART;
    sound_register += 0x10;
    sound_register[0] = PSG_CHANNEL_RESTART;
    sound_register -= 0xD;
    sound_register[0] = 0;
    *(u8 *)psg_volume_control = PSG_INITIAL_OUTPUT_VOLUME;
    sound_state = *(void **)0x03007FF0;
    signature = SOUND_ENGINE_WORD(sound_state, signature);
    if (signature == SOUND_ENGINE_SIGNATURE) {
        SOUND_ENGINE_WORD(sound_state, signature) = signature + 1;
        command_handlers = (s32 *)MUSIC_COMMAND_TABLE_RAM;
        command_handlers[MUSIC_HANDLER_MEMORY_ACCESS] = (s32) &D_080EC901;
        command_handlers[MUSIC_HANDLER_LFO_SPEED] = (s32) &D_080EB605;
        command_handlers[MUSIC_HANDLER_MODULATION_DEPTH] = (s32) &D_080EB619;
        command_handlers[MUSIC_HANDLER_EXTENDED_COMMAND] = (s32) &D_080ECA59;
        command_handlers[MUSIC_HANDLER_END_TIE] = (s32) &D_080EB59D;
        command_handlers[MUSIC_HANDLER_SAMPLE_RATE] = (s32) &D_080EBB9D;
        command_handlers[MUSIC_HANDLER_STOP_TRACK] = (s32) &D_080EB329;
        command_handlers[MUSIC_HANDLER_UPDATE_FADE] = (s32) &D_080EBF65;
        command_handlers[MUSIC_HANDLER_UPDATE_VOLUME_AND_PITCH] = (s32) &D_080EC02D;
        SOUND_ENGINE_FIELD(sound_state, void **, psg_channels) = psg_channels;
        SOUND_ENGINE_WORD(sound_state, update_psg_channels) = (s32) &D_080EC241;
        SOUND_ENGINE_WORD(sound_state, stop_psg_channel) = (s32) &D_080EC189;
        SOUND_ENGINE_WORD(sound_state, calculate_psg_frequency) = (s32) &D_080EC0E1;
        zero_symbol = (s32) &D_zero;
        zero = 0;
        SOUND_ENGINE_SIGNED_BYTE(sound_state, max_scanlines) = zero_symbol;
        clear_source = zero;
        BiosCpuSet(&clear_source, psg_channels, (void *)SOUND_PSG_CHANNELS_CLEAR_CONTROL);
        M2C_FIELD(psg_channels, s8 *, PSG_CHANNEL_OFFSET(channel_id)) = 1;
        M2C_FIELD(psg_channels, s8 *, PSG_CHANNEL_OFFSET(output_pan_mask)) = PSG_CHANNEL_1_PAN_MASK;
        channel_field = (u8 *)psg_channels + sizeof(struct PsgChannelState) + PSG_CHANNEL_OFFSET(channel_id);
        *channel_field = 2;
        channel_field += PSG_CHANNEL_OFFSET(output_pan_mask) - PSG_CHANNEL_OFFSET(channel_id);
        *channel_field = PSG_CHANNEL_2_PAN_MASK;
        channel_field += sizeof(struct PsgChannelState) + PSG_CHANNEL_OFFSET(channel_id) - PSG_CHANNEL_OFFSET(output_pan_mask);
        *channel_field = 3;
        channel_field += PSG_CHANNEL_OFFSET(output_pan_mask) - PSG_CHANNEL_OFFSET(channel_id);
        *channel_field = PSG_CHANNEL_3_PAN_MASK;
        channel_field += sizeof(struct PsgChannelState) - PSG_CHANNEL_OFFSET(output_pan_mask);
        channel_field[PSG_CHANNEL_OFFSET(channel_id)] = 4;
        channel_field[PSG_CHANNEL_OFFSET(output_pan_mask)] = PSG_CHANNEL_4_PAN_MASK;
        SOUND_ENGINE_WORD(sound_state, signature) = signature;
    }
}

/*
 * This wrapper calls a BIOS service with swi 0x2A.
 * C has no expression for this software interrupt.
 */
void BiosGetSoundDriverJumpList(void) asm("func_080EBAA4");

__attribute__((naked)) void BiosGetSoundDriverJumpList(void) {
    asm(".syntax unified\n"
        "swi 0x2a\n"
        "bx lr\n"
        ".syntax divided");
}
