"""Extract the ROM font and call the shared text core from the host."""

from __future__ import annotations

import ctypes
import hashlib
import os
from pathlib import Path
import struct
import subprocess
import tempfile

import dialogue

SOURCE = Path(__file__).with_suffix('.c')
HEADER = Path(__file__).with_suffix('.h')
TALL_TABLE = dialogue.PROFILE['glyphs']
COMPACT_TABLE = 0x7A0B48

TEXT_FACE_TALL = 1
TEXT_FACE_COMPACT = 2
TEXT_END = 1
TEXT_WRAP_NONE = 0
TEXT_WRAP_WORD = 1
TEXT_OVERFLOW_ERROR = 0
TEXT_OVERFLOW_CLIP = 1
TEXT_PROFILE_PORTRAIT_DIALOGUE = 0
TEXT_PROFILE_FULL_DIALOGUE = 1
TEXT_PROFILE_LABEL = 2
TEXT_PROFILE_BATTLE_CHOICE = 7
TEXT_PROFILE_SPEAKER = 8


class CFont(ctypes.Structure):
    _fields_ = [('metrics', ctypes.c_char_p), ('metric_count', ctypes.c_uint16),
                ('ranges', ctypes.c_char_p), ('range_count', ctypes.c_uint16),
                ('pairs', ctypes.c_char_p), ('pair_count', ctypes.c_uint16),
                ('pair_offsets', ctypes.c_char_p), ('glyphs', ctypes.c_char_p),
                ('glyph_stride', ctypes.c_uint8), ('tall_fallback', ctypes.c_uint16),
                ('compact_fallback', ctypes.c_uint16)]


class CCheck(ctypes.Structure):
    _fields_ = [('text', ctypes.c_char_p), ('player', ctypes.c_char_p),
                ('text_length', ctypes.c_uint16), ('player_length', ctypes.c_uint16),
                ('left', ctypes.c_int16), ('right', ctypes.c_int16),
                ('bottom', ctypes.c_int16), ('x', ctypes.c_int16), ('y', ctypes.c_int16),
                ('kind', ctypes.c_uint8), ('wrap', ctypes.c_uint8),
                ('overflow', ctypes.c_uint8), ('resume', ctypes.c_uint8),
                ('color', ctypes.c_uint8), ('lines', ctypes.c_uint8),
                ('missing_count', ctypes.c_uint8), ('missing', ctypes.c_uint16 * 128)]


def _library_path() -> Path:
    digest = hashlib.sha256(SOURCE.read_bytes() + HEADER.read_bytes()).hexdigest()[:16]
    suffix = '.dylib' if os.uname().sysname == 'Darwin' else '.so'
    directory = Path(tempfile.gettempdir()) / f'zoids-text-core-{os.getuid()}'
    directory.mkdir(mode=0o700, exist_ok=True)
    output = directory / f'{digest}{suffix}'
    if output.exists():
        return output
    temporary = output.with_suffix(output.suffix + '.tmp')
    command = ['cc', '-std=c99', '-O2', '-fPIC', '-Wall', '-Wextra', '-Werror']
    command += ['-dynamiclib' if suffix == '.dylib' else '-shared',
                str(SOURCE), '-o', str(temporary)]
    result = subprocess.run(command, capture_output=True, text=True)
    if result.returncode:
        raise ValueError(f'Host build of text_core.c failed:\n{result.stderr}')
    temporary.replace(output)
    return output


def _load_library():
    library = ctypes.CDLL(str(_library_path()))
    library.text_check.argtypes = [ctypes.POINTER(CFont), ctypes.POINTER(CCheck)]
    library.text_check.restype = ctypes.c_int
    return library


LIBRARY = None


def library():
    global LIBRARY
    if LIBRARY is None:
        LIBRARY = _load_library()
    return LIBRARY


def _unpack_rows(raw: bytes, height: int) -> list[list[int]]:
    rows = [[(raw[y * 2 + x // 4] >> ((x % 4) * 2)) & 3
             for x in range(8)] for y in range(height)]
    return rows + [[0] * 8 for _ in range(16 - height)]


def _pack_rows(rows: list[list[int]]) -> bytes:
    return bytes(sum((rows[y][x + offset] if x + offset < len(rows[y]) else 0)
                     << (2 * offset) for offset in range(4))
                 for y in range(16) for x in range(0, 16, 4))


def _ranges(metrics: list[dict]) -> list[tuple[int, int, int, int]]:
    result = []
    for index, metric in enumerate(metrics):
        if (result and metric['face'] == result[-1][3] and
                metric['code'] == result[-1][0] + result[-1][1] and
                index == result[-1][2] + result[-1][1]):
            first, count, base, face = result[-1]
            result[-1] = first, count + 1, base, face
        else:
            result.append((metric['code'], 1, index, metric['face']))
    return result


def extract_font(rom: bytes, choices: dict) -> dict:
    metrics = []
    advances = choices.get('advances', {})
    letter_spacing = choices.get('letter_spacing', 0)
    proportional = choices.get('spacing') == 'proportional'
    for face, table, table_format, height in (
            (TEXT_FACE_TALL, TALL_TABLE, '<HBBI', 16),
            (TEXT_FACE_COMPACT, COMPACT_TABLE, '<BBHI', 8)):
        baseline = 14 if face == TEXT_FACE_TALL else 7
        glyph_size = 32 if face == TEXT_FACE_TALL else 16
        for table_index in range(22):
            first, count, _, pointer = struct.unpack_from(table_format, rom, table + table_index * 8)
            start = pointer - dialogue.BASE
            for offset, code in enumerate(range(first, first + count)):
                raw = rom[start + offset * glyph_size:start + (offset + 1) * glyph_size]
                rows = _unpack_rows(raw, height)
                columns = [x for x in range(8) if any(rows[y][x] for y in range(height))]
                ink_rows = [y for y in range(height) if any(rows[y])]
                if columns:
                    left, right = min(columns), max(columns) + 1
                    top, bottom = min(ink_rows), max(ink_rows) + 1
                else:
                    left = right = top = bottom = 0
                advance = 8
                if face == TEXT_FACE_TALL:
                    default = min(right - left + 1, 8) if columns else 4
                    advance = advances.get(f'{code:04X}', default) + letter_spacing
                if not 1 <= advance <= 16:
                    raise ValueError(f'Invalid advance for glyph {code:04X}.')
                width = right - left
                ink_height = bottom - top
                bitmap = [[0] * 16 for _ in range(16)]
                for y in range(ink_height):
                    bitmap[y][:width] = rows[top + y][left:right]
                metrics.append(dict(code=code,
                                    bearing_x=0 if face == TEXT_FACE_TALL and proportional else left,
                                    bearing_y=top - baseline if ink_height else 0,
                                    width=width, height=ink_height, advance=advance,
                                    face=face, line_height=height, baseline=baseline,
                                    bitmap=bitmap))
    indices = {(metric['face'], metric['code']): index
               for index, metric in enumerate(metrics)}
    pairs = []
    for key, adjust in choices.get('pairs', {}).items():
        try:
            left, right = (indices[(TEXT_FACE_TALL, int(part, 16))] for part in key.split(':'))
        except (KeyError, ValueError) as error:
            raise ValueError(f'Unknown glyph in pair {key}.') from error
        if not -128 <= adjust <= 127:
            raise ValueError(f'Invalid pair adjustment for {key}.')
        pairs.append((left, right, adjust))
    pairs.sort()
    return dict(metrics=metrics, ranges=_ranges(metrics), pairs=pairs,
                glyphs=b''.join(_pack_rows(metric['bitmap']) for metric in metrics),
                tall_fallback=indices[(TEXT_FACE_TALL, 0x8148)],
                compact_fallback=indices[(TEXT_FACE_COMPACT, ord('?'))])


def pack_font(font: dict) -> dict:
    metrics = b''.join(struct.pack(
        '<HbbBBbBBB', metric['code'], metric['bearing_x'], metric['bearing_y'],
        metric['width'], metric['height'], metric['advance'], metric['face'],
        metric['line_height'], metric['baseline'])
        for metric in font['metrics'])
    pair_offsets = []
    position = 0
    for left in range(len(font['metrics']) + 1):
        while position < len(font['pairs']) and font['pairs'][position][0] < left:
            position += 1
        pair_offsets.append(position)
    return dict(
        metrics=metrics,
        pairs=b''.join(struct.pack('<HHbB', left, right, adjust, 0)
                       for left, right, adjust in font['pairs']),
        pair_offsets=struct.pack(f'<{len(pair_offsets)}H', *pair_offsets),
        ranges=b''.join(struct.pack('<HHHBB', first, count, base, face, 0)
                        for first, count, base, face in font['ranges']))


class TextChecker:
    def __init__(self, font: dict):
        self.packed = pack_font(font)
        self.glyphs = font['glyphs']
        self.font = CFont(self.packed['metrics'], len(font['metrics']),
                          self.packed['ranges'], len(font['ranges']),
                          self.packed['pairs'], len(font['pairs']),
                          self.packed['pair_offsets'], self.glyphs, 4,
                          font['tall_fallback'], font['compact_fallback'])
        tall = max((metric for metric in font['metrics']
                    if metric['face'] == TEXT_FACE_TALL and metric['width']),
                   key=lambda metric: (metric['advance'], metric['width'], metric['code']))
        self.widest_player = tall['code'].to_bytes(2, 'big') * 8 + b'\0'

    def check(self, text: bytes, kind: int, right: int, bottom: int, *,
              wrap: int, overflow: int, left: int = -1, player: bytes | None = None,
              start: tuple[int, int, int] | None = None) -> CCheck:
        player = self.widest_player if player is None else player
        check = CCheck(text, player, len(text), len(player), left, right, bottom,
                       *(start[:2] if start else (0, 0)), kind, wrap, overflow,
                       int(start is not None), start[2] if start else 0)
        check.status = library().text_check(ctypes.byref(self.font), ctypes.byref(check))
        check.missing_codes = list(check.missing[:check.missing_count])
        return check
