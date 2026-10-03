#include "m2c_prelude.h"

typedef struct {
    u8 *data;
    u32 size;
} SaveBlock;

extern s32 WriteSramWithRetries(u32, u32, u32) asm("func_080ECCE4");
extern s32 CallFunctionR3(u32, u32, u32, u32) asm("func_080ECD68");
extern s32 CompareStrings(u32, u32) asm("func_080ED0CC");
extern SaveBlock gSaveBlockTable[];
extern u8 gSaveBuffer[];

s32 TransferSaveBlock(u32 transfer_mode, u32 block_index)
{
    register u32 raw_mode asm("r0") = transfer_mode;
    register u32 normalized_block_index asm("r1") = block_index;
    register s32 mode asm("r8");
    register u32 sram_address asm("r6");
    SaveBlock *entry;
    register u32 selected_block asm("r5");
    register u32 preceding_block_index asm("r2");
    u32 retry;
    s32 result;

    raw_mode = (u8)raw_mode;
    mode = raw_mode;
    normalized_block_index = (u8)normalized_block_index;
    selected_block = normalized_block_index;
    sram_address = 0x0E000000;

    if (raw_mode == 0) {
        WriteSramWithRetries(0x087A0A48, sram_address, 4);
    }

    sram_address += 4;
    preceding_block_index = 0;
    entry = gSaveBlockTable;
    while (preceding_block_index < selected_block) {
        register u8 *data asm("r0") = entry->data;
        register u32 size asm("r4") = entry->size;

        if (data == 0 && size == 0) {
            return 2;
        }
        {
            register u32 increment asm("r0") = size;

            increment += 12;
            sram_address += increment;
        }
        entry++;
        preceding_block_index++;
    }

    {
        register volatile u32 *write_state asm("r9");
        register u8 *buffer asm("r5");
        register u32 expected asm("r4");
        register s32 mismatch asm("r1");

        retry = 0;
        write_state = (volatile u32 *)0x03007750;
        buffer = gSaveBuffer;
        goto header_body;
header_increment:
        retry = (u8)(retry + 1);
        if (retry == 3) {
            return 0;
        }
header_body:
            {
                register volatile u32 *write_view asm("r1") = write_state;
                register u32 write_value asm("r3");

                write_value = *write_view;
                CallFunctionR3(sram_address, (u32)buffer, 11, write_value);
            }
            if (CallFunctionR3((u32)buffer, sram_address, 11,
                              *(volatile u32 *)0x03007754) != 0) {
                goto header_increment;
            }

            {
                register u8 *compare_buffer asm("r0") = gSaveBuffer;

                expected = 0x087A0A4C;
                result = CompareStrings((u32)compare_buffer, expected);
                mismatch = result;
                asm volatile("" : "+r"(mismatch));
            }

            if (mismatch != 0) {
                switch (mode) {
                case 1:
                    return 2;
                case 0:
                {
                    register s32 write_result asm("r1");

                    result = WriteSramWithRetries(expected, sram_address, 11);
                    write_result = result;
                    asm volatile("" : "+r"(write_result));
                    if (write_result != 0) {
                        return 0;
                    }
                    break;
                }
                case 2:
                    return 0;
                default:
                    break;
                }
            }
            goto header_done;
    }

header_done:
    sram_address += 11;
    {
        register s32 mode_view asm("r1") = mode;

        switch (mode_view) {
    case 0: {
        register u8 *source asm("r1") = entry->data;
        register u8 *destination asm("r3") = gSaveBuffer;
        register u8 *saved_buffer asm("r9");
        register u32 checksum asm("r5") = 0;
        register u32 send_index asm("r2") = 0;
        u32 send_size = entry->size;

        saved_buffer = destination;
        if (send_index < send_size) {
            do {
                register u32 sum asm("r0");

                *destination = *source;
                sum = *source;
                sum = checksum + sum;
                sum <<= 24;
                checksum = sum >> 24;
                source++;
                destination++;
                send_index++;
            } while (send_index < entry->size);
        }
        {
            register u32 inverse asm("r0");
            register u32 checksum_view asm("r5") = checksum;

            inverse = ~checksum_view;
            *destination = inverse;
        }
        if (WriteSramWithRetries((u32)saved_buffer, sram_address,
                          entry->size + 1) != 0) {
            return 0;
        }
        return 1;
    }
    case 1:
    case 2:
    {
        retry = 0;
        goto receive_body;
receive_increment:
        retry = (u8)(retry + 1);
        if (retry == 3) {
            return 0;
        }
receive_body:
        {
            register volatile u32 *receive_state asm("r0") =
                (volatile u32 *)0x03007750;
            register u8 *buffer asm("r5") = gSaveBuffer;
            register u32 transfer_size asm("r2") = entry->size + 1;
            register u32 receive_value asm("r3") = *receive_state;

            CallFunctionR3(sram_address, (u32)buffer, transfer_size,
                          receive_value);
            {
                register volatile u32 *read_state asm("r0") =
                    (volatile u32 *)0x03007754;
                register u32 read_size asm("r2") = entry->size + 1;
                register u32 read_value asm("r3") = *read_state;

                result = CallFunctionR3((u32)buffer, sram_address, read_size,
                                       read_value);
            }
            if (result == 0) {
                register u8 *source asm("r1") = buffer;
                register u8 *saved_source asm("r9");
                register u32 checksum asm("r5") = 0;
                register u32 receive_index asm("r2") = 0;
                register u32 size asm("r3") = entry->size;
                register u32 copy_size asm("r4") = size;

                saved_source = source;
                if (receive_index < copy_size) {
                    do {
                        register u32 sum asm("r0");

                        sum = *source;
                        sum = checksum + sum;
                        sum <<= 24;
                        checksum = sum >> 24;
                        source++;
                        receive_index++;
                    } while (receive_index < size);
                }
                {
                    register u32 inverse asm("r0");
                    register u32 checksum_view asm("r5") = checksum;

                    inverse = ~checksum_view;
                    inverse <<= 24;
                    checksum = inverse >> 24;
                }
                if (*source != checksum) {
                    goto bad_checksum;
                }

                if (mode == 1) {
                    register u8 *destination asm("r3");

                    source = saved_source;
                    destination = entry->data;
                    receive_index = 0;
                    if (receive_index < copy_size) {
                        do {
                            *destination = *source;
                            source++;
                            destination++;
                            receive_index++;
                        } while (receive_index < entry->size);
                    }
                }
                return 1;
            }
            goto receive_increment;
        }
    }
    default:
        return result;
        }
    }
bad_checksum:
    {
        register s32 bad_result asm("r0") = 0;

        asm volatile("" : "+r"(bad_result));
        return bad_result;
    }
}
