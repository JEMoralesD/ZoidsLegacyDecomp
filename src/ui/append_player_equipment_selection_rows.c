#include "player_selection.h"

extern void FormatNumberText(u8, s32, s32, void *) asm("func_08098284");
extern void AppendWindowTextItem(s32, const void *) asm("func_080988C8");
extern u8 CountEncodedTextGlyphs(const void *) asm("func_08098B58");
extern void AppendString(void *, const void *) asm("func_08099F5C");
extern void CopyString(void *, const void *) asm("func_080ED128");

extern u8 gPlayerEquipmentSelectionCount asm("D_02032379");
extern u8 gPlayerEquipmentSelectionIds[] asm("D_020322B2");
extern u8 gPlayerSelectionRowText[] asm("D_02030566");
extern u8 gPlayerSelectionNumberText[] asm("D_020305E4");
extern u8 gPlayerStateBytes[] asm("D_020218E4");
extern struct EquipmentRecord gEquipmentCatalog[] asm("D_087B2524");
extern const void *gEquipmentNameTable[] asm("D_087EE170");
extern u8 gPlayerSelectionSpaceText[] asm("D_081061C4");

void AppendPlayerEquipmentSelectionRows(s32 unused_window_id, s32 selection_flags, s32 zoid_model_id) asm("func_080AC468");

void AppendPlayerEquipmentSelectionRows(s32 unused_window_id, s32 selection_flags, s32 zoid_model_id)
{
    register u32 saved_selection_flags asm("r8");
    register u32 selected_zoid_model_id asm("r9");
    u32 selection_index;
    register u16 *text_prefix asm("sl");
    volatile u32 selected_size_code;

    (void)unused_window_id;
    selection_flags <<= 24;
    saved_selection_flags = (u32)selection_flags >> 24;
    zoid_model_id <<= 24;
    selected_zoid_model_id = (u32)zoid_model_id >> 24;
    selection_index = 0;
    if (selection_index >= gPlayerEquipmentSelectionCount) {
        return;
    }

    text_prefix = (u16 *)PLAYER_SELECTION_TEXT_BUFFER_RAM;
    {
        register u32 size_mask asm("r1") = EQUIPMENT_SELECTION_SIZE_MASK;
        register u32 size_flags asm("r2") = saved_selection_flags;

        asm volatile("" : "+r"(size_mask), "+r"(size_flags));
        selected_size_code = size_flags & size_mask;
    }
    do {
        struct EquipmentRecord *equipment;
        register u8 *status_to_set asm("r1");
        register u32 status_code asm("r0");
        register u32 compatibility_group_present asm("r0");
        volatile u32 next_selection_index;

        {
            register u8 *equipment_ids_or_record_offset asm("r0") = gPlayerEquipmentSelectionIds;
            register u32 equipment_id_or_name_address asm("r1");

            equipment_ids_or_record_offset = (u8 *)selection_index + (u32)equipment_ids_or_record_offset;
            asm volatile("ldrb %0, [%1]"
                         : "=r"(equipment_id_or_name_address)
                         : "r"(equipment_ids_or_record_offset));
            equipment_ids_or_record_offset = (u8 *)(equipment_id_or_name_address << 1);
            equipment_ids_or_record_offset += equipment_id_or_name_address;
            equipment_ids_or_record_offset = (u8 *)((u32)equipment_ids_or_record_offset << 3);
            {
                register struct EquipmentRecord *equipment_records asm("r1") = gEquipmentCatalog;
                register struct EquipmentRecord *selected_equipment_record asm("r6") =
                    (struct EquipmentRecord *)((u8 *)equipment_ids_or_record_offset + (u32)equipment_records);

                asm volatile("" : "+r"(equipment_records), "+r"(selected_equipment_record)
                             : "r"(equipment_ids_or_record_offset));
                equipment = selected_equipment_record;
            }
        }

        {
            register u8 *initial_status asm("r0") = (u8 *)PLAYER_EQUIPMENT_SELECTION_STATUS_RAM;
            register u32 zero asm("r3");

            asm volatile("" : "+r"(initial_status));
            initial_status = (u8 *)selection_index + (u32)initial_status;
            zero = 0;
            asm volatile("strb %1, [%0]"
                         : "+r"(initial_status), "+r"(zero));
        }
        asm volatile("ldrb %0, [%1, #21]"
                     : "=r"(compatibility_group_present)
                     : "r"(equipment));
        next_selection_index = selection_index + 1;
        if (compatibility_group_present != 0) {
            register u32 compatible_model_index asm("r4") = 0;
            register u8 *compatibility_table_bytes asm("r0") = (u8 *)EQUIPMENT_COMPATIBLE_MODEL_TABLE_ROM;
            register u8 *saved_compatibility_table asm("ip");
            register u32 compatibility_group_id asm("r2");
            register u32 compatibility_row_offset asm("r3");
            register u8 *first_model_address_or_id asm("r1");

            asm volatile("" : "+r"(compatible_model_index), "+r"(compatibility_table_bytes));
            asm volatile("ldrb %0, [%1, #21]"
                         : "=r"(compatibility_group_id)
                         : "r"(equipment), "r"(compatibility_table_bytes));
            compatibility_row_offset = compatibility_group_id << 3;
            first_model_address_or_id = (u8 *)compatibility_row_offset;
            first_model_address_or_id += (u32)compatibility_table_bytes;
            asm volatile("ldrb %0, [%0]" : "+r"(first_model_address_or_id));
            saved_compatibility_table = compatibility_table_bytes;
            asm volatile("" : "+r"(saved_compatibility_table));
            if ((u32)first_model_address_or_id != 0) {
                if ((u32)first_model_address_or_id == selected_zoid_model_id) {
                    goto model_compatibility_checked;
                }
            {
                register u8 *compatibility_table_base asm("r5") = saved_compatibility_table;
                register u32 compatibility_offset asm("r1") = compatibility_row_offset;

                asm volatile("" : "+r"(compatibility_table_base), "+r"(compatibility_offset));
                do {
                    register u32 next_compatible_model_index asm("r0") = compatible_model_index + 1;
                    register u8 *model_address_or_id asm("r0");

                    next_compatible_model_index <<= 16;
                    compatible_model_index = next_compatible_model_index >> 16;
                    if (compatible_model_index > EQUIPMENT_COMPATIBLE_MODEL_COUNT - 1) {
                        break;
                    }
                    model_address_or_id = (u8 *)compatible_model_index;
                    model_address_or_id += compatibility_offset;
                    model_address_or_id += (u32)compatibility_table_base;
                    asm volatile("ldrb %0, [%0]" : "+r"(model_address_or_id));
                    if ((u32)model_address_or_id == 0 || (u32)model_address_or_id == selected_zoid_model_id) {
                        break;
                    }
                } while (1);
            }
            }
            {
                register u32 model_address_or_id_final asm("r0");

                model_address_or_id_final = compatibility_group_id << 3;
                model_address_or_id_final = compatible_model_index + model_address_or_id_final;
                asm volatile("add %0, %1"
                             : "+r"(model_address_or_id_final)
                             : "r"(saved_compatibility_table));
                asm volatile("ldrb %0, [%0]" : "+r"(model_address_or_id_final));
            if (model_address_or_id_final != selected_zoid_model_id) {
                ((u8 *)PLAYER_EQUIPMENT_SELECTION_STATUS_RAM)[selection_index] = EQUIPMENT_SELECTION_MODEL_MISMATCH;
                {
                    register u32 rejected_text_prefix asm("r0") = PLAYER_SELECTION_REJECTED_TEXT_PREFIX;
                    register u16 *rejected_prefix_output asm("r3") = text_prefix;

                    asm volatile("" : "+r"(rejected_text_prefix), "+r"(rejected_prefix_output));
                    *rejected_prefix_output = rejected_text_prefix;
                }
            }
            }
model_compatibility_checked:
        }

        {
        register u8 *status_base asm("r1") = (u8 *)PLAYER_EQUIPMENT_SELECTION_STATUS_RAM;
        register u8 *status asm("r2");

        asm volatile("" : "+r"(status_base));
        status = (u8 *)selection_index + (u32)status_base;
        asm volatile("" : "+r"(status));

        if (*status == 0) {
            register u32 mount_constraints asm("r0") = equipment->mount_constraints;
            register u32 mount_requirement asm("r1") = mount_constraints & EQUIPMENT_MOUNT_MINIMUM_SIZE_MASK;

            if (mount_requirement == 0) {
                goto size_requirement_met;
            }
            if (mount_requirement == 4 && selected_size_code > 0x0F) {
                goto size_requirement_met;
            }
            if (mount_requirement == 8 && selected_size_code > 0x1F) {
                goto size_requirement_met;
            }
            if (mount_requirement == 0xC && selected_size_code > 0x2F) {
                goto size_requirement_met;
            }
            if (mount_requirement == 0x10 && selected_size_code > 0x3F) {
                goto size_requirement_met;
            }
            *status = EQUIPMENT_SELECTION_TOO_SMALL;
            {
                register u32 rejected_text_prefix asm("r3") = PLAYER_SELECTION_REJECTED_TEXT_PREFIX;
                register u16 *rejected_prefix_output asm("r2") = text_prefix;

                asm volatile("" : "+r"(rejected_text_prefix), "+r"(rejected_prefix_output));
                *rejected_prefix_output = rejected_text_prefix;
            }
            goto append_equipment_row;

size_requirement_met:
            mount_requirement = mount_constraints & EQUIPMENT_MOUNT_MODE_MASK;
            if (mount_requirement == 0) {
                goto check_equipment_kind;
            }
            if (mount_requirement == 1) {
                register u32 masked asm("r0") = saved_selection_flags & EQUIPMENT_SELECTION_MOUNT_MODE_MASK;

                if (masked == 4) {
                    goto check_equipment_kind;
                }
            }
            if (mount_requirement == 2) {
                register u32 masked asm("r0") = 0xC;
                register u32 flag_view asm("r3");

                asm volatile("" : "+r"(masked));
                flag_view = saved_selection_flags;
                asm volatile("" : "+r"(flag_view));
                masked &= flag_view;
                if (masked == 8 || masked == 0xC) {
                    goto check_equipment_kind;
                }
            }
            if (mount_requirement == 3) {
                register u32 masked asm("r0") = saved_selection_flags & EQUIPMENT_SELECTION_MOUNT_MODE_MASK;

                if (masked == 0xC) {
                    goto check_equipment_kind;
                }
            }
            status_to_set = (u8 *)PLAYER_EQUIPMENT_SELECTION_STATUS_RAM + selection_index;
            status_code = EQUIPMENT_SELECTION_MOUNT_MODE_MISMATCH;
            goto set_status;

check_equipment_kind:
            {
                    register u32 kind_allowed asm("r0");

                    if ((equipment->flags & EQUIPMENT_COMMAND) == 0) {
                        kind_allowed = saved_selection_flags;
                        asm volatile("" : "+r"(kind_allowed));
                        kind_allowed &= 1;
                    } else {
                        kind_allowed = saved_selection_flags & 2;
                    }
                    if (kind_allowed == 0) {
                        status_to_set = (u8 *)PLAYER_EQUIPMENT_SELECTION_STATUS_RAM + selection_index;
                        status_code = EQUIPMENT_SELECTION_KIND_MISMATCH;
                        goto set_status;
                    } else if (equipment->family_id == EQUIPMENT_SMOKE_FAMILY_ID && (saved_selection_flags & EQUIPMENT_SELECTION_REJECT_SMOKE_UNITS) != 0) {
                        status_to_set = (u8 *)PLAYER_EQUIPMENT_SELECTION_STATUS_RAM + selection_index;
                        status_code = EQUIPMENT_SELECTION_SMOKE_PROHIBITED;
                        goto set_status;
                    } else {
                        goto set_available;
                    }
            }
        }
        }
        goto append_equipment_row;

set_status:
        *status_to_set = status_code;
        {
            register u32 rejected_text_prefix asm("r0") = PLAYER_SELECTION_REJECTED_TEXT_PREFIX;
            register u16 *rejected_prefix_output asm("r3") = text_prefix;

            asm volatile("" : "+r"(rejected_text_prefix), "+r"(rejected_prefix_output));
            *rejected_prefix_output = rejected_text_prefix;
        }
        goto append_equipment_row;

set_available:
        {
            register u32 normal_text_prefix asm("r0") = PLAYER_SELECTION_NORMAL_TEXT_PREFIX;
            register u16 *normal_prefix_output asm("r1") = text_prefix;

            asm volatile("" : "+r"(normal_text_prefix), "+r"(normal_prefix_output));
            *normal_prefix_output = normal_text_prefix;
        }

append_equipment_row:
        {
            register u8 *output asm("r4") = (u8 *)0x02030566;
            register u8 *equipment_ids_or_record_offset asm("r2") = gPlayerEquipmentSelectionIds;
            register u32 equipment_id_or_name_address asm("r0");
            u16 name_columns;

            asm volatile("" : "+r"(output), "+r"(equipment_ids_or_record_offset));
            equipment_id_or_name_address = selection_index;
            equipment_id_or_name_address += (u32)equipment_ids_or_record_offset;
            asm volatile("ldrb %0, [%0]" : "+r"(equipment_id_or_name_address));
            equipment_id_or_name_address <<= 2;
            {
                register const void **text_table asm("r3") = gEquipmentNameTable;
                register const void *text asm("r1");

                asm volatile("" : "+r"(text_table) : "r"(equipment_id_or_name_address));
                asm volatile("add %0, %1"
                             : "+r"(equipment_id_or_name_address)
                             : "r"(text_table));
                asm volatile("ldr %0, [%1]"
                             : "=r"(text)
                             : "r"(equipment_id_or_name_address));
                CopyString(output, text);
            }
            name_columns = CountEncodedTextGlyphs(output);

            if (name_columns <= 9) {
                do {
                    AppendString((void *)0x02030566, gPlayerSelectionSpaceText);
                    name_columns++;
                } while (name_columns <= 9);
            }
        }
        {
            register u8 *output asm("r5") = (u8 *)0x02030566;

            asm volatile("" : "+r"(output));
            AppendString(output, (const void *)PLAYER_SELECTION_QUANTITY_SEPARATOR_ROM);
            {
                register u8 *player_state_or_quantity_bytes asm("r1") = gPlayerStateBytes;
                register u8 *equipment_ids_or_record_offset asm("r0") = gPlayerEquipmentSelectionIds;

                asm volatile("" : "+r"(player_state_or_quantity_bytes), "+r"(equipment_ids_or_record_offset));
                equipment_ids_or_record_offset = (u8 *)selection_index + (u32)equipment_ids_or_record_offset;
                {
                    register u32 equipment_quantities_offset asm("r2") = PLAYER_STATE_OFFSET(equipment_quantities);

                    asm volatile("" : "+r"(equipment_quantities_offset)
                                 : "r"(player_state_or_quantity_bytes), "r"(equipment_ids_or_record_offset));
                    player_state_or_quantity_bytes += equipment_quantities_offset;
                    asm volatile("ldrb %0, [%0]" : "+r"(equipment_ids_or_record_offset));
                    player_state_or_quantity_bytes += (u32)equipment_ids_or_record_offset;
                    FormatNumberText(*player_state_or_quantity_bytes, 2, 2, gPlayerSelectionNumberText);
                }
            }
            AppendString(output, gPlayerSelectionNumberText);
            {
                register u32 zero asm("r0") = 0;
                register u8 *line_start asm("r1");

                asm volatile("" : "+r"(zero));
                line_start = output - 2;
                asm volatile("" : "+r"(line_start) : "r"(zero));
                AppendWindowTextItem(zero, line_start);
            }
        }
        {
            register u32 next_index_value asm("r3") = next_selection_index;
            register u32 normalized_index asm("r0");

            asm volatile("" : "+r"(next_index_value));
            normalized_index = next_index_value << 16;
            selection_index = normalized_index >> 16;
        }
    } while (selection_index < gPlayerEquipmentSelectionCount);
}
