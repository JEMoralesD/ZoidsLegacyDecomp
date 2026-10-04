#!/usr/bin/env python3
"""Build the translated ROM and write the site's BPS patch and version from VERSION."""

import argparse
import binascii
import json
from pathlib import Path
import struct
import subprocess

import dialogue
import insert_vwf

ROOT = Path(__file__).resolve().parent.parent
SITE = ROOT / 'site'


def encode_number(value):
    output = bytearray()
    while True:
        byte = value & 127
        value >>= 7
        if not value:
            output.append(byte | 128)
            return output
        output.append(byte)
        value -= 1


def create_bps(source, target):
    patch = bytearray(b'BPS1')
    for value in (len(source), len(target), 0):
        patch.extend(encode_number(value))
    cursor = 0
    target_relative = 0
    while cursor < len(target):
        start = cursor
        if cursor < len(source) and target[cursor] == source[cursor]:
            while cursor < min(len(source), len(target)) and target[cursor] == source[cursor]:
                cursor += 1
            patch.extend(encode_number((cursor - start - 1) * 4))
            continue
        if cursor and target[cursor] == target[cursor - 1]:
            stop = cursor + 1
            while stop < len(target) and target[stop] == target[cursor - 1]:
                stop += 1
            if stop - cursor >= 16:
                patch.extend(encode_number((stop - cursor - 1) * 4 + 3))
                offset = cursor - 1 - target_relative
                patch.extend(encode_number(abs(offset) * 2 + (offset < 0)))
                target_relative = cursor - 1 + stop - cursor
                cursor = stop
                continue
        cursor += 1
        while cursor < len(target):
            if cursor < len(source) and target[cursor] == source[cursor]:
                break
            if target[cursor] == target[cursor - 1] and target[cursor:cursor + 16] == bytes([target[cursor]]) * 16:
                break
            cursor += 1
        patch.extend(encode_number((cursor - start - 1) * 4 + 1))
        patch.extend(target[start:cursor])
    patch.extend(struct.pack('<II', binascii.crc32(source), binascii.crc32(target)))
    patch.extend(struct.pack('<I', binascii.crc32(patch)))
    return bytes(patch)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom', type=Path, default=ROOT / 'Zoids Legacy (USA).gba')
    args = parser.parse_args()
    try:
        version = insert_vwf.release_version()
        source = dialogue.read_rom(args.rom)
        target = insert_vwf.build_rom(
            source, dialogue.load_scenes(ROOT / 'scene-translation.json', args.rom),
            json.loads((ROOT / 'kerning-choices.json').read_text()),
            dialogue.load_dialogue(ROOT / 'dialogue-en.json', args.rom))
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        parser.exit(1, f'Error: {error}\n')
    (SITE / 'patch.bps').write_bytes(create_bps(source, target))
    (SITE / 'version.mjs').write_text(f"export const PATCH_VERSION = '{version}';\n")
    print(f'Wrote the version {version} patch to {SITE / "patch.bps"}.')


if __name__ == '__main__':
    main()
