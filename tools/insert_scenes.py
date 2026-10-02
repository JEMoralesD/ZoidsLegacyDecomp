"""Add the lookup table for edited event text."""

import re
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile

import dialogue
import text_core


HOOK_OFFSET = 0xA150C
PAYLOAD_OFFSET = 0x800000
TABLE_OFFSET = PAYLOAD_OFFSET + 0x40
OUTPUT_SIZE = 0x1000000


def assemble_hook():
    assembler = shutil.which('arm-none-eabi-as')
    objcopy = shutil.which('arm-none-eabi-objcopy')
    if not assembler or not objcopy:
        raise ValueError('Install GNU Arm binutils: arm-none-eabi-as and arm-none-eabi-objcopy.')
    with tempfile.TemporaryDirectory() as directory:
        obj = Path(directory) / 'hook.o'
        binary = Path(directory) / 'hook.bin'
        subprocess.run([assembler, '-o', str(obj), str(Path(__file__).with_name('scene_text_hook.s'))], check=True)
        subprocess.run([objcopy, '-O', 'binary', '-j', '.text', str(obj), str(binary)], check=True)
        code = binary.read_bytes()
    return code


SPEAKER = re.compile(rb'^(\x01\x02[^\n]+)\n(.*)$', re.S)
SCENE_PROFILES = {20: (text_core.TEXT_PROFILE_PORTRAIT_DIALOGUE, 160, 2, text_core.TEXT_WRAP_WORD),
                  28: (text_core.TEXT_PROFILE_FULL_DIALOGUE, 224, 2, text_core.TEXT_WRAP_WORD),
                  9: (text_core.TEXT_PROFILE_BATTLE_CHOICE, 72, 0, text_core.TEXT_WRAP_NONE)}


def scene_text(checker, entry):
    offset = entry['english_offset']
    encoded = dialogue.encode_text(entry['english_draft'])
    old = bytes.fromhex(entry['english_original_hex'])
    if dialogue.control_units(old).count(b'\x03') != dialogue.control_units(encoded).count(b'\x03'):
        raise ValueError(f'Changed player control tokens at {offset}.')
    if entry.get('text_columns') not in SCENE_PROFILES:
        raise ValueError(f'Missing layout profile at {offset}.')
    kind, right, left, wrap = SCENE_PROFILES[entry['text_columns']]
    if kind == text_core.TEXT_PROFILE_BATTLE_CHOICE and len(entry['english_draft'].splitlines()) != 2:
        raise ValueError(f'The battle menu must keep two choices at {offset}.')
    body, start = encoded, None
    match = SPEAKER.match(encoded[:-1])
    if match:
        speaker = checker.check(match[1] + b'\0', text_core.TEXT_PROFILE_SPEAKER, right, 16, left=left,
                                wrap=text_core.TEXT_WRAP_NONE, overflow=text_core.TEXT_OVERFLOW_ERROR)
        if speaker.status != text_core.TEXT_END or speaker.missing_codes:
            raise ValueError(f'Speaker does not fit at {offset}.')
        body, start = match[2] + b'\0', (left, 16, speaker.color)
    result = checker.check(body, kind, right, 512, wrap=wrap,
                           overflow=text_core.TEXT_OVERFLOW_ERROR, start=start)
    if result.status != text_core.TEXT_END or result.missing_codes:
        raise ValueError(f'Text does not fit or has missing glyphs at {offset}.')
    return encoded


def build_rom(rom, document, checker, event_edits):
    records = [(int(entry['english_offset'], 16), scene_text(checker, entry))
               for entry in document['entries']]
    records += event_edits.items()
    records.sort()
    if len({offset for offset, _ in records}) != len(records):
        raise ValueError('Duplicate event text offset.')
    strings = bytearray()
    table = bytearray()
    text_start = TABLE_OFFSET + (len(records) + 1) * 8
    for offset, encoded in records:
        if rom[offset - 1] != 0x20:
            raise ValueError(f'No event text command at 0x{offset:06X}.')
        table.extend(struct.pack('<II', dialogue.BASE + offset, dialogue.BASE + text_start + len(strings)))
        strings.extend(encoded)
    table.extend(bytes(8))
    payload = assemble_hook() + table + strings
    if PAYLOAD_OFFSET + len(payload) > OUTPUT_SIZE:
        raise ValueError('Edited text exceeds the expanded ROM size.')
    result = bytearray(rom)
    result.extend(b'\xff' * (OUTPUT_SIZE - len(result)))
    result[PAYLOAD_OFFSET:PAYLOAD_OFFSET + len(payload)] = payload
    # This aligned Thumb jump replaces only the display-pointer assignment.
    result[HOOK_OFFSET:HOOK_OFFSET + 8] = struct.pack('<HHI', 0x4800, 0x4700,
                                                  dialogue.BASE + PAYLOAD_OFFSET + 1)
    return bytes(result)
