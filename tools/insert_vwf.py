"""Build the translated Zoids Legacy ROM."""

import argparse
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import tempfile

import dialogue
import insert_scenes
import text_core

ROOT = Path(__file__).resolve().parent.parent
HOOK = 0x97DA8
STORY_CHOICE_HOOK = 0x9FBA8
STORY_CHOICE_WIDTH_HOOK = 0x9FC0C
DESCRIPTION_HOOK = 0xE2C24
PAYLOAD = 0x900000
BASE = dialogue.BASE
DECK_NAME_TABLE = 0x7EF130
CREDITS_HOOK = 0xE5128
CREDITS_LENGTH = 32
CREDITS_RESUME = BASE + 0xE5136 + 1
NUMBER_HOOK = 0x98284
TALL_CONVERTER_VENEER = 0x97BF4
STARTUP_GLYPH_VENEER = 0x97C24
COMPACT_CONVERTER_VENEER = 0x97D80
MEASURE_VENEER = 0x98B58
STORAGE_VENEER = 0x97CEC
NUMBER_AT_HOOK = 0x9844C
NUMBER_CURRENT_HOOK = 0x984C4
CURSOR_SET_HOOK = 0x981D0
FIELD_AT_HOOK = 0x981F0
EDITOR_FIELD_HOOK = 0x9C45C
FIELD_CURRENT_HOOK = 0x98248
REFRESH_HOOK = 0x972C8
CLEAR_HOOK = 0x986B4
CLEAR_ALL_HOOK = 0x98754
WINDOW_CREATE_HOOK = 0x98564
LIST_RENDER_HOOK = 0x9885C
LIST_ADD_HOOK = 0x988C8
LIST_REPLACE_HOOK = 0x989EC
MENU_COMMAND_HOOK = 0x98C8C
MENU_COMMAND_VENEER = 0x98C98
STARTUP_CALL = 0x9B200
COPY_HOOK = 0xED128
CONCAT_HOOK = 0x99F5C
PREFIX_HOOK = 0x9F770
CONSTRUCTION_POSITION_CALLS = (
    0xA3B8C, 0xA3C54, 0xA3CF4, 0xA3D8A, 0xA3E14, 0xA3EAA,
    0xA3F8C, 0xA3FFC, 0xA40AE, 0xA4146, 0xA41D0, 0xA426C,
    0xA43EE, 0xA44CC, 0xA4572, 0xA460A, 0xA46AA, 0xA474A,
    0xA50A4, 0xA52EA,
    0xAC61E, 0xAC734, 0xAC7F4,
)
VISUAL_MEASURE_CALLS = (
    0x98B06, 0x9EA32,
    0xAC8DC, 0xAC946, 0xAC992, 0xAC9E6,
    0xAD06A, 0xAD0B8, 0xAD9D8,
    0xAEA62, 0xAEA92, 0xAF520, 0xAF63E,
    0xB2384, 0xB2486, 0xB24BE, 0xB2626, 0xB27BE, 0xB2800,
    0xB31BA, 0xB32BE, 0xB32F2,
)

MENU_ALIGNMENT_ROOTS = (0x002FCD, 0x004EC1, 0x006176,
                        0x028199, 0x0282AA, 0x028388)
MENU_SCRIPT_AREA = 0xA00000
CUSTOMIZE_WINDOWS = {0x005828, 0x005AD5, 0x005B13}
STRING_AREA = 0xA80000
FORMAT_STRING_AREA = 0xB00000
MOVABLE_POINTER_TABLES = (
    (0x7A3624, 0x7A3660),
    (0x7A3660, 0x7A3660 + 152 * 4),
    (0x7A38C0, 0x7A38C0 + 105 * 4),
    (0x7EDD54, 0x7EDD54 + 152 * 4),
    (0x7EDFB4, 0x7EDFB4 + 105 * 4),
    (0x7EE170, 0x7EE170 + 808 * 4),
    (0x7EEE38, 0x7EEE38 + 10 * 4),
    (0x7EEE60, 0x7EEE60 + 90 * 4),
    (0x7EEFC8, 0x7EEFC8 + 90 * 4),
    (0x7EF130, 0x7EF130 + 52 * 4),
    (0x7EF200, 0x7EF200 + 52 * 4),
    (0x7EF2D0, 0x7EF2D0 + 40 * 4),
    (0x7EF370, 0x7EF370 + 40 * 4),
    (0x7EF410, 0x7EF410 + 26 * 4),
    (0x7EF478, 0x7EF478 + 26 * 4),
    (0x7EF4E0, 0x7EF4E0 + 44 * 4),
    (0x7EF590, 0x7F26C8),
    (0x7F2DEC, 0x7F2DEC + 20 * 4),
)
COUNTED_NAME_SITES = (
    (0xBBF52, 0xBBF56, 0xBBF58,
     (0xE006, 0x7823, 0xB401, 0x4810, 0x4684, 0xBC01, 0x4760, 0x46C0),
     0xBBFA0),
    (0xE2880, 0xE2884, 0xE2886,
     (0xE006, 0x7823, 0xB401, 0x4811, 0x4684, 0xBC01, 0x4760, 0x46C0),
     0xE28D0),
    (0xE2BA4, 0xE2BA8, 0xE2BAC,
     (0x464D, 0xE780, 0x464B, 0x781B, 0xB401, 0x4806, 0x4684, 0xBC01, 0x4760, 0x46C0),
     0xE2BCC),
)
TITLE_STRING_COPY_SITES = {
    0x103A38: 0x09D0D0,
    0x103A70: 0x09D138,
    0x103AA8: 0x09D1DE,
}
WEAPON_LAYOUT_INSTRUCTIONS = {
    0xAE566: (0x2303, 0x2302),
    0xAE5B4: (0x2301, 0x2300),
    0xAE5C8: (0x2301, 0x2300),
    0xB079A: (0x2303, 0x2302),
    0xB07E8: (0x2301, 0x2300),
    0xB07FC: (0x2301, 0x2300),
}
DECK_RESTRICTION_WINDOW_INSTRUCTIONS = {
    0xC2224: (0x2200, 0x2201),
    0xC222E: (0x2200, 0x2201),
    0xC2238: (0x2200, 0x2201),
}


def align_menu_scripts(rom, original, text_edits):
    handled = set()
    roots = dialogue.menu_roots(original)
    pointers = dialogue.data_pointers(original)
    literals = {root: pointers.get(root, []) for root in roots}
    cursor = MENU_SCRIPT_AREA
    for root in roots:
        source = root
        output = bytearray()
        edited = False
        while True:
            opcode = original[source]
            if opcode == 0:
                source += 1
                output.append(0)
                break
            if opcode in (5, 9):
                text_offset = source + 2
                _, end = dialogue.decode_text(original, text_offset)
                output.extend(original[source:text_offset])
                handled.add(text_offset)
                if text_offset in text_edits:
                    output.extend(text_edits[text_offset])
                    edited = True
                else:
                    output.extend(original[text_offset:end])
                source = end
            else:
                width = dialogue.MENU_LENGTHS.get(opcode, 0)
                if not width:
                    raise ValueError(f'Unsupported menu command at 0x{source:X}.')
                command = (dialogue.menu_window_command(original, source) if opcode == 1
                           else original[source:source + width])
                output.extend(command)
                edited |= command != original[source:source + width]
                if source in CUSTOMIZE_WINDOWS:
                    output[-2] = 12
                    edited = True
                source += width
        if not edited and root not in MENU_ALIGNMENT_ROOTS:
            continue
        if rom[root:source] != original[root:source]:
            raise ValueError(f'Menu script differs at 0x{root:X}.')
        if len(output) <= source - root:
            rom[root:root + len(output)] = output
            rom[root + len(output):source] = b'\xff' * (source - root - len(output))
            continue
        cursor = (cursor + 3) & ~3
        end = cursor + len(output)
        if end > len(rom) or any(value != 0xff for value in rom[cursor:end]):
            raise ValueError(f'No free ROM space for menu script at 0x{root:X}.')
        if not literals[root]:
            raise ValueError(f'No pointer to menu script at 0x{root:X}.')
        rom[cursor:end] = output
        for at in literals[root]:
            if rom[at:at + 4] != original[at:at + 4]:
                raise ValueError(f'Menu pointer differs at 0x{at:X}.')
            struct.pack_into('<I', rom, at, BASE + cursor)
        cursor = end
    return handled

def relocate_strings(rom, original, edits, capacities, area=STRING_AREA):
    profile = dialogue.PROFILE
    wanted = set(edits)
    literals = {offset: [] for offset in wanted}
    for at in range(0, len(original) - 3, 4):
        target = struct.unpack_from('<I', original, at)[0] - BASE
        if target in wanted:
            literals[target].append(at)
    cursor = area
    for offset in sorted(edits):
        replacement = edits[offset]
        _, end = dialogue.decode_text(original, offset)
        if offset in capacities:
            end = offset + capacities[offset]
        else:
            while end % 4 and end < len(original) and original[end] == 0:
                end += 1
        if rom[offset:end] != original[offset:end]:
            raise ValueError(f'String differs at 0x{offset:X}.')
        if len(replacement) <= end - offset:
            rom[offset:offset + len(replacement)] = replacement
            rom[offset + len(replacement):end] = b'\x00' * (end - offset - len(replacement))
            continue
        pointers = literals[offset]
        code_pointers = [at for at in pointers
                         if profile['code_start'] <= at < profile['code_end']]
        table_pointers = [at for at in pointers
                          if any(start <= at < stop for start, stop in MOVABLE_POINTER_TABLES)]
        if table_pointers:
            pointers = code_pointers + table_pointers
        elif len(code_pointers) != len(pointers):
            pointers = []
        if not pointers:
            raise ValueError(f'String at 0x{offset:X} has no movable pointer.')
        cursor = (cursor + 3) & ~3
        stop = cursor + len(replacement)
        if stop > len(rom) or any(value != 0xff for value in rom[cursor:stop]):
            raise ValueError(f'No free ROM space for string at 0x{offset:X}.')
        rom[cursor:stop] = replacement
        for at in pointers:
            if rom[at:at + 4] != original[at:at + 4]:
                raise ValueError(f'String pointer differs at 0x{at:X}.')
            struct.pack_into('<I', rom, at, BASE + cursor)
        cursor = stop


def patch_counted_names(rom, original, target):
    for call, block, entry, after, literal in COUNTED_NAME_SITES:
        if (rom[call:call + 4] != original[call:call + 4] or
                rom[literal:literal + 4] != original[literal:literal + 4]):
            raise ValueError(f'Counted name call differs at 0x{call:X}.')
        rom[call:call + 4] = encode_bl(call, BASE + entry)
        rom[block:block + len(after) * 2] = struct.pack(f'<{len(after)}H', *after)
        struct.pack_into('<I', rom, literal, target | 1)


def patch_title_string_copy_lengths(rom, original, edits):
    for offset, instruction in TITLE_STRING_COPY_SITES.items():
        replacement = edits.get(offset)
        if replacement is None:
            continue
        if len(replacement) > 0xff:
            raise ValueError(f'Title string at 0x{offset:X} exceeds its copy limit.')
        if rom[instruction:instruction + 2] != original[instruction:instruction + 2]:
            raise ValueError(f'Title string copy changed at 0x{instruction:X}.')
        struct.pack_into('<H', rom, instruction, 0x2200 | len(replacement))


def compile_payload(font, packed):
    for tool in ('arm-none-eabi-gcc', 'arm-none-eabi-objcopy', 'arm-none-eabi-nm'):
        if not shutil.which(tool):
            raise ValueError(f'Missing tool: {tool}')
    with tempfile.TemporaryDirectory() as tmp:
        path = Path(tmp)
        (path / 'glyphs.bin').write_bytes(font['glyphs'])
        for name in ('metrics', 'pairs', 'pair_offsets', 'ranges'):
            (path / f'{name}.bin').write_bytes(packed[name])
        compact_indices = [0xffff] * 256
        for index, metric in enumerate(font['metrics']):
            if metric['face'] == 2 and metric['code'] < 256:
                compact_indices[metric['code']] = index
        (path / 'compact_indices.bin').write_bytes(struct.pack(
            '<256H', *compact_indices))
        (path / 'entry.s').write_text(f'''.syntax unified
.cpu arm7tdmi
.thumb
.section .text,"ax"
.global memcpy
.thumb_func
memcpy:
    mov ip, r0
    movs r3, r0
    orrs r3, r1
    lsls r3, #30
    bne 5f
    cmp r2, #4
    blo 2f
1:
    ldmia r1!, {{r3}}
    stmia r0!, {{r3}}
    subs r2, #4
    cmp r2, #4
    bhs 1b
    b 2f
5:
    lsls r3, #1
    bne 2f
    cmp r2, #2
    blo 2f
6:
    ldrh r3, [r1]
    strh r3, [r0]
    adds r1, #2
    adds r0, #2
    subs r2, #2
    cmp r2, #2
    bhs 6b
2:
    cmp r2, #0
    beq 4f
3:
    ldrb r3, [r1]
    strb r3, [r0]
    adds r1, #1
    adds r0, #1
    subs r2, #1
    bne 3b
4:
    mov r0, ip
    bx lr
.section .rodata,"a"
.balign 4
.global runtime_font_data
runtime_font_data:
    .word metrics
    .hword {len(font['metrics'])}
    .hword 0
    .word ranges
    .hword {len(font['ranges'])}
    .hword 0
    .word pairs
    .hword {len(font['pairs'])}
    .hword 0
    .word pair_offsets
    .word glyphs
    .byte 4
    .byte 0
    .hword {font['tall_fallback']}
    .hword {font['compact_fallback']}
    .hword 0
.global ranges
ranges: .incbin "{path / 'ranges.bin'}"
.balign 4
.global metrics
metrics: .incbin "{path / 'metrics.bin'}"
.balign 4
.global pairs
pairs: .incbin "{path / 'pairs.bin'}"
.balign 4
.global pair_offsets
pair_offsets: .incbin "{path / 'pair_offsets.bin'}"
.balign 4
.global glyphs
glyphs: .incbin "{path / 'glyphs.bin'}"
.balign 4
.global compact_indices
compact_indices: .incbin "{path / 'compact_indices.bin'}"
.section .story_choice,"ax"
.balign 4
.global vwf_story_choice
.thumb_func
vwf_story_choice:
    push {{r4, r5, r6, lr}}
    ldr r2, =0x08800040
1:
    ldr r1, [r2]
    cmp r1, #0
    beq 3f
    cmp r1, r0
    beq 2f
    adds r2, #8
    b 1b
2:
    ldr r0, [r2, #4]
3:
    sub sp, #24
    movs r2, r0
    ldr r0, =0x02030564
    ldr r3, =0x0809FBB1
    bx r3
    .ltorg
.balign 4
.global vwf_story_choice_width
.thumb_func
vwf_story_choice_width:
    strb r0, [r4]
    movs r4, #0
    movs r6, #0
1:
    lsls r0, r4, #2
    add r0, sp
    ldr r0, [r0, #8]
    bl vwf_measure_cells
    cmp r6, r0
    bhs 2f
    movs r6, r0
2:
    adds r4, #1
    cmp r4, r5
    blo 1b
    movs r1, #28
    subs r1, r1, r6
    lsls r1, #24
    ldr r3, =0x0809FC15
    bx r3
    .ltorg
''')
        (path / 'link.ld').write_text(f'''SECTIONS {{
 . = {BASE + PAYLOAD};
 .text : {{ *(.text*) *(.rodata*) }}
 .story_choice : {{ *(.story_choice) }}
 /DISCARD/ : {{ *(.comment*) *(.ARM.attributes*) *(.ARM.exidx*) }}
}}
''')
        elf = path / 'vwf.elf'
        subprocess.run(['arm-none-eabi-gcc', '-mcpu=arm7tdmi', '-mthumb', '-Os',
                        '-ffreestanding', '-fno-jump-tables', '-fno-builtin', '-fno-unwind-tables', '-nostdlib',
                        '-Wall', '-Wextra', '-Werror', '-Wl,--build-id=none',
                        '-std=c99', '-DTEXT_RUNTIME_SCRATCH=0x0200DC00',
                        '-I', str(Path(__file__).parent),
                        '-T', str(path / 'link.ld'), str(path / 'entry.s'),
                        str(Path(__file__).with_name('text_core.c')),
                        str(Path(__file__).with_name('dialogue_vwf.c')),
                        '-o', str(elf)], check=True)
        subprocess.run(['arm-none-eabi-objcopy', '-O', 'binary', str(elf), str(path / 'vwf.bin')], check=True)
        symbols = subprocess.check_output(['arm-none-eabi-nm', str(elf)], text=True)
        addresses = {parts[2]: int(parts[0], 16) for line in symbols.splitlines()
                     if len(parts := line.split()) == 3}
        return (path / 'vwf.bin').read_bytes(), addresses


def encode_bl(offset, target):
    displacement = target - (BASE + offset + 4)
    if displacement & 1 or not -(1 << 22) <= displacement < (1 << 22):
        raise ValueError(f'Thumb BL target is out of range at {BASE + offset:08X}.')
    return struct.pack('<HH', 0xf000 | ((displacement >> 12) & 0x7ff),
                       0xf800 | ((displacement >> 1) & 0x7ff))


def long_jump(target):
    return struct.pack('<HHI', 0x4b00, 0x4718, target | 1)


def long_jump_preserve_r3(target):
    return struct.pack('<HHHHI', 0xb508, 0x4b01, 0x9301, 0xbd08, target | 1)


def is_battle_quote(offset):
    return 0x248A0 <= offset <= 0x28182 or 0x91D6E <= offset <= 0x920C1 or offset == 0x108C3C


def check_game_text(checker, entry, source, replacement, template_format):
    kind, right, bottom, left = text_core.TEXT_PROFILE_FULL_DIALOGUE, 512, 512, 0
    wrap, overflow = text_core.TEXT_WRAP_NONE, text_core.TEXT_OVERFLOW_CLIP
    if is_battle_quote(int(entry['offset'], 16)):
        kind, right, bottom, left = text_core.TEXT_PROFILE_LABEL, 176, 16, 2
        wrap, overflow = text_core.TEXT_WRAP_WORD, text_core.TEXT_OVERFLOW_ERROR
    sample = replacement
    if template_format:
        sample = re.sub(rb'%[1-9][0-9]*\$[dsv]|%[ds]', lambda match: dialogue.encode_text(
            '9999999' if match[0][-1:] in (b'd', b'v') else 'G', source,
            compact=entry.get('compact_format', False))[:-1], replacement)
    if entry.get('repeat_format') or entry.get('row_format'):
        sample = sample[1:]
    result = checker.check(sample, kind, right, bottom, left=left, wrap=wrap,
                           overflow=overflow, player=dialogue.encode_text('Zeru'))
    original = {int.from_bytes(unit, 'big') for unit in dialogue.text_units(source, 0) if unit[0] >= 0x20}
    missing = [code for code in result.missing_codes if code not in original]
    if result.status != text_core.TEXT_END or missing:
        codes = ', '.join(f'{code:04X}' for code in missing)
        raise ValueError(f'Game text at {entry["offset"]} does not fit or uses missing glyphs ({codes or "layout status " + str(result.status)}).')


def check_deck_command_names(rom, checker):
    for pointer in struct.unpack_from('<52I', rom, DECK_NAME_TABLE):
        name, end = dialogue.decode_text(rom, pointer - BASE)
        result = checker.check(bytes(rom[pointer - BASE:end]), text_core.TEXT_PROFILE_LABEL, 104, 16,
                               wrap=text_core.TEXT_WRAP_NONE, overflow=text_core.TEXT_OVERFLOW_ERROR,
                               player=b'\0')
        if result.status != text_core.TEXT_END or result.missing_codes or result.lines:
            raise ValueError(f'Deck Command must fit one 104-pixel line: {name}')


def build_rom(original, document, choices, dialogue_document):
    font = text_core.extract_font(original, choices)
    checker = text_core.TextChecker(font)
    scene_offsets = {int(entry['english_offset'], 16) for entry in document['entries']}
    event_edits = {}
    menu_edits = {}
    string_edits = {}
    format_string_edits = {}
    fixed_edits = {}
    string_capacities = {}
    menu_overrides = set()
    for entry in dialogue_document['entries']:
        offset = int(entry['offset'], 16)
        raw = bytes.fromhex(entry['original'])
        source = dialogue.entry_source(entry)
        if original[offset:offset + len(raw)] != raw or dialogue.decode_text(source, 0)[1] != len(source):
            raise ValueError(f'Invalid original text at {entry["offset"]}.')
        if 'capacity' in entry:
            capacity = entry['capacity']
            if not isinstance(capacity, int) or capacity < len(raw) or offset + capacity > len(original):
                raise ValueError(f'Invalid string capacity at {entry["offset"]}.')
            string_capacities[offset] = capacity
        template_format = entry['kind'] == 'menu' or entry.get('template_format', False)
        if 'build_text' in entry:
            if not template_format or dialogue.decode_text(source, 0)[0] != entry['text']:
                raise ValueError(f'Invalid template source at {entry["offset"]}.')
            replacement = dialogue.encode_menu_format(
                entry['build_text'], source, entry.get('compact_format', False))
            if entry['kind'] == 'menu':
                menu_overrides.add(offset)
        else:
            replacement = (dialogue.encode_menu_format(
                               entry['text'], source, entry.get('compact_format', False))
                           if entry['kind'] == 'menu' else
                           dialogue.encode_text(entry['text'], source,
                                                compact=entry.get('compact_format', False)))
        indexed_fields = re.findall(rb'%([1-9][0-9]*)\$([dsv])', replacement)
        if indexed_fields:
            ordinals = [int(number) for number, _ in indexed_fields]
            if (not template_format or
                    (not entry.get('row_format') and
                     sorted(ordinals) != list(range(1, len(ordinals) + 1)))
                    or b'%d' in replacement or b'%s' in replacement):
                raise ValueError(f'Invalid indexed format at {entry["offset"]}.')
        if entry.get('row_format'):
            if (entry['kind'] != 'menu' or not indexed_fields or
                    any(kind != b's' for _, kind in indexed_fields) or
                    max(ordinals) > 20 or entry.get('repeat_format')):
                raise ValueError(f'Invalid row menu format at {entry["offset"]}.')
            replacement = b'^' + replacement
        if entry.get('repeat_format'):
            if entry['kind'] != 'menu' or not indexed_fields and b'%d' not in replacement and b'%s' not in replacement:
                raise ValueError(f'Invalid repeating menu format at {entry["offset"]}.')
            replacement = b'~' + replacement
        if replacement == source:
            continue
        if offset in scene_offsets:
            raise ValueError(f'Edit scene text in scene-translation.json at {entry["offset"]}.')
        check_game_text(checker, entry, source, replacement, template_format)
        if entry['kind'] == 'event':
            event_edits[offset] = replacement
        elif entry['kind'] == 'menu':
            menu_edits[offset] = replacement
        elif entry['kind'] == 'string':
            if entry.get('terminated', True):
                target = format_string_edits if entry.get('template_format') else string_edits
                target[offset] = replacement
            elif len(replacement) != len(source):
                raise ValueError(f'Fixed text at {entry["offset"]} must keep its byte length.')
            else:
                fixed_edits[offset] = replacement[:-1]
        else:
            raise ValueError(f'Unsupported text kind at {entry["offset"]}.')
    rom = bytearray(insert_scenes.build_rom(original, document, checker, event_edits))
    handled = align_menu_scripts(rom, original, menu_edits)
    relocate_strings(rom, original, string_edits, string_capacities)
    relocate_strings(rom, original, format_string_edits, string_capacities,
                     FORMAT_STRING_AREA)
    patch_title_string_copy_lengths(rom, original, string_edits)
    for offset, replacement in fixed_edits.items():
        rom[offset:offset + len(replacement)] = replacement
    if menu_overrides - handled:
        offset = min(menu_overrides - handled)
        raise ValueError(f'Menu override has no supported script at 0x{offset:X}.')
    if set(menu_edits) - handled:
        offset = min(set(menu_edits) - handled)
        raise ValueError(f'Menu text has no supported script at 0x{offset:X}.')
    check_deck_command_names(rom, checker)
    payload, symbols = compile_payload(font, checker.packed)
    if PAYLOAD + len(payload) > len(rom) or any(b != 255 for b in rom[PAYLOAD:PAYLOAD + len(payload)]):
        raise ValueError('Renderer payload overlaps existing data.')
    rom[PAYLOAD:PAYLOAD + len(payload)] = payload
    rom[REFRESH_HOOK:REFRESH_HOOK + 8] = long_jump(symbols['vwf_refresh'])
    rom[HOOK:HOOK + 8] = long_jump(symbols['vwf_render'])
    rom[STORY_CHOICE_HOOK:STORY_CHOICE_HOOK + 8] = long_jump(symbols['vwf_story_choice'])
    rom[STORY_CHOICE_WIDTH_HOOK:STORY_CHOICE_WIDTH_HOOK + 8] = long_jump(symbols['vwf_story_choice_width'])
    rom[DESCRIPTION_HOOK:DESCRIPTION_HOOK + 8] = long_jump(symbols['vwf_description'])
    rom[NUMBER_HOOK:NUMBER_HOOK + 12] = long_jump_preserve_r3(symbols['vwf_format_number'])
    rom[NUMBER_AT_HOOK:NUMBER_AT_HOOK + 12] = long_jump_preserve_r3(symbols['vwf_number_at'])
    rom[NUMBER_CURRENT_HOOK:NUMBER_CURRENT_HOOK + 12] = long_jump_preserve_r3(
        symbols['vwf_number_current'])
    rom[CURSOR_SET_HOOK:CURSOR_SET_HOOK + 8] = long_jump(symbols['vwf_cursor_set'])
    rom[FIELD_AT_HOOK:FIELD_AT_HOOK + 12] = long_jump_preserve_r3(
        symbols['vwf_field_at'])
    rom[EDITOR_FIELD_HOOK:EDITOR_FIELD_HOOK + 8] = long_jump(symbols['vwf_editor_text'])
    rom[FIELD_CURRENT_HOOK:FIELD_CURRENT_HOOK + 8] = long_jump(
        symbols['vwf_field_current'])
    patch_counted_names(rom, original, symbols['vwf_field_counted'])
    rom[CLEAR_HOOK:CLEAR_HOOK + 8] = long_jump(symbols['vwf_clear_window'])
    rom[CLEAR_ALL_HOOK:CLEAR_ALL_HOOK + 8] = long_jump(symbols['vwf_clear_window_all'])
    rom[WINDOW_CREATE_HOOK:WINDOW_CREATE_HOOK + 8] = long_jump(symbols['vwf_window_created'])
    rom[LIST_RENDER_HOOK:LIST_RENDER_HOOK + 8] = long_jump(
        symbols['vwf_list_render_next'])
    rom[LIST_ADD_HOOK:LIST_ADD_HOOK + 8] = long_jump(symbols['vwf_list_add'])
    rom[LIST_REPLACE_HOOK:LIST_REPLACE_HOOK + 8] = long_jump(
        symbols['vwf_list_replace'])
    for offset, (expected, replacement) in WEAPON_LAYOUT_INSTRUCTIONS.items():
        if struct.unpack_from('<H', rom, offset)[0] != expected:
            raise ValueError(f'Weapon layout instruction differs at 0x{offset:X}.')
        struct.pack_into('<H', rom, offset, replacement)
    for offset, (expected, replacement) in DECK_RESTRICTION_WINDOW_INSTRUCTIONS.items():
        if struct.unpack_from('<H', rom, offset)[0] != expected:
            raise ValueError(f'Deck restriction window instruction differs at 0x{offset:X}.')
        struct.pack_into('<H', rom, offset, replacement)
    rom[MENU_COMMAND_HOOK:MENU_COMMAND_HOOK + 12] = (
        struct.pack('<HH', 0x1c38, 0x4641) +
        encode_bl(MENU_COMMAND_HOOK + 4, BASE + MENU_COMMAND_VENEER) +
        struct.pack('<HH', 0x4680, 0xe188))
    rom[MENU_COMMAND_VENEER:MENU_COMMAND_VENEER + 8] = long_jump(
        symbols['vwf_menu_command'])
    rom[COPY_HOOK:COPY_HOOK + 8] = long_jump(symbols['vwf_bounded_copy'])
    rom[CONCAT_HOOK:CONCAT_HOOK + 8] = long_jump(symbols['vwf_bounded_concat'])
    rom[PREFIX_HOOK:PREFIX_HOOK + 8] = long_jump(symbols['vwf_menu_prefix'])
    rom[TALL_CONVERTER_VENEER:TALL_CONVERTER_VENEER + 8] = long_jump(
        symbols['direct_tall_tiles'])
    rom[STARTUP_GLYPH_VENEER:STARTUP_GLYPH_VENEER + 8] = long_jump(
        symbols['compact_glyph_tile'])
    rom[COMPACT_CONVERTER_VENEER:COMPACT_CONVERTER_VENEER + 8] = long_jump(
        symbols['direct_compact_tiles'])
    rom[MEASURE_VENEER:MEASURE_VENEER + 8] = long_jump(symbols['vwf_measure_cells'])
    rom[STORAGE_VENEER:STORAGE_VENEER + 8] = long_jump(symbols['vwf_storage_cells'])
    for call in CONSTRUCTION_POSITION_CALLS:
        rom[call:call + 4] = encode_bl(call, BASE + STORAGE_VENEER)
    for call in VISUAL_MEASURE_CALLS:
        rom[call:call + 4] = encode_bl(call, BASE + MEASURE_VENEER)
    rom[STARTUP_CALL:STARTUP_CALL + 4] = encode_bl(
        STARTUP_CALL, BASE + STARTUP_GLYPH_VENEER)
    rom[CREDITS_HOOK:CREDITS_HOOK + CREDITS_LENGTH] = struct.pack(
        '<10H4x2I', 0x4640, 0x4629, 0xAA09, 0x4B04, 0x469E, 0x4B04, 0x4718, 0x4680,
        0x7802, 0xE016, CREDITS_RESUME, symbols['credits_line'] | 1)
    return bytes(rom)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom', type=Path, default=ROOT / 'Zoids Legacy (USA).gba')
    parser.add_argument('--draft', type=Path, default=ROOT / 'scene-translation.json')
    parser.add_argument('--kerning', type=Path, default=ROOT / 'kerning-choices.json')
    parser.add_argument('--dialogue', type=Path, default=ROOT / 'dialogue-en.json')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    try:
        document = dialogue.load_scenes(args.draft, args.rom)
        choices = json.loads(args.kerning.read_text())
        text_data = dialogue.load_dialogue(args.dialogue, args.rom)
        rom = build_rom(dialogue.read_rom(args.rom), document, choices, text_data)
        args.output.write_bytes(rom)
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        parser.exit(1, f'Error: {error}\n')
    print(f'Wrote {args.output}.')


if __name__ == '__main__':
    main()
