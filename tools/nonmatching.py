#!/usr/bin/env python3
"""This program compiles the non-matching C listed in nonmatching.txt and compares it with a user ROM."""

from __future__ import annotations

import argparse
import difflib
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

import linklib
from verify import EXPECTED_SHA1, ROM_SIZE, ROOT, compiler_flags, regions, run, sha1

BASE = 0x08000000
RE_LINE = re.compile(r"^\s*([0-9a-f]+):\s+([0-9a-f]{4})(?: ([0-9a-f]{4}))?\s+(\S+)\s*(.*)$")
RE_PC_LOAD = re.compile(r"\[pc, #(\d+)\]")
RE_BRANCH = re.compile(r"^b(?:eq|ne|cs|cc|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?(?:\.n|\.w)?$")


def compile_entry(entry, work: Path, agbcc: Path, old_agbcc: Path) -> bytes:
    name, start, _, sources, mode = entry
    compiler = old_agbcc if mode == "old" else agbcc
    flags = compiler_flags(mode)
    objects = []
    for index, source_name in enumerate(sources):
        source = ROOT / "src" / source_name
        preprocessed = work / f"{index}.i"
        assembly = work / f"{index}.s"
        object_file = work / f"{index}.o"
        run(["cc", "-E", "-P", "-I", "tools", str(source), "-o", str(preprocessed)], name)
        run([str(compiler), *flags, "-o", str(assembly), str(preprocessed)], name)
        with assembly.open("a") as output:
            output.write("\t.align\t2, 0\n")
        run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", str(object_file), str(assembly)], name)
        objects.append(str(object_file))
    return linklib.link_text(objects, BASE + start, str(work / "entry.bin"), work=str(work), ld_flags=("--no-check-sections",))


def disassemble(data: bytes, vma: int, work: Path, label: str, rom: bytes | None = None) -> list[str]:
    blob = work / f"{label}.bin"
    blob.write_bytes(data)
    output = subprocess.run(
        ["arm-none-eabi-objdump", "-D", "-b", "binary", "-marm", "-Mforce-thumb", f"--adjust-vma=0x{vma:X}", str(blob)],
        capture_output=True, text=True, check=True,
    ).stdout
    decoded = []
    for line in output.splitlines():
        match = RE_LINE.match(line)
        if match:
            decoded.append((int(match.group(1), 16), match.group(4), match.group(5).split("@")[0].strip()))
    pool = set()
    for address, mnemonic, operands in decoded:
        load = RE_PC_LOAD.search(operands)
        if mnemonic == "ldr" and load:
            target = ((address + 4) & ~3) + int(load.group(1))
            pool.update((target, target + 2))
    end = vma + len(data)
    listing = []
    for address, mnemonic, operands in decoded:
        if address in pool:
            continue
        load = RE_PC_LOAD.search(operands)
        if mnemonic == "ldr" and load:
            target = ((address + 4) & ~3) + int(load.group(1))
            offset = target - vma
            if offset + 4 <= len(data):
                value = int.from_bytes(data[offset:offset + 4], "little")
            elif rom is not None:
                value = int.from_bytes(rom[target - BASE:target - BASE + 4], "little")
            else:
                value = 0
            operands = RE_PC_LOAD.sub(f"=0x{value:08X}", operands)
        elif RE_BRANCH.match(mnemonic):
            target = int(operands.split()[0], 16)
            operands = "<local>" if vma <= target < end else operands.split()[0]
        listing.append(f"{mnemonic} {operands}".strip())
    return listing


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--rom", type=Path, required=True)
    parser.add_argument("--agbcc", type=Path, default=Path(os.environ.get("AGBCC", ROOT.parent / "agbcc" / "agbcc")))
    parser.add_argument("--old-agbcc", type=Path, default=Path(os.environ.get("OLD_AGBCC", ROOT.parent / "agbcc" / "old_agbcc")))
    parser.add_argument("--manifest", type=Path, default=ROOT / "nonmatching.txt")
    parser.add_argument("--entry")
    parser.add_argument("--diff", action="store_true", help="print the instruction diff for each entry")
    args = parser.parse_args()

    if not args.rom.is_file() or args.rom.stat().st_size != ROM_SIZE or sha1(args.rom) != EXPECTED_SHA1:
        print("ROM is unavailable or does not match Zoids Legacy (USA).", file=sys.stderr)
        return 2
    selected = [entry for entry in regions(args.manifest) if args.entry in {None, entry[0]}]
    if not selected:
        print("No entry matches the selection.", file=sys.stderr)
        return 2
    rom = args.rom.read_bytes()
    total_reference = total_equal = 0
    with tempfile.TemporaryDirectory(prefix="zoidsq-nonmatching-") as temporary:
        for entry in selected:
            name, start, end, _, _ = entry
            work = Path(temporary) / name
            work.mkdir()
            try:
                data = compile_entry(entry, work, args.agbcc, args.old_agbcc)
            except (RuntimeError, subprocess.CalledProcessError) as error:
                print(f"FAIL {name}: {error}", file=sys.stderr)
                continue
            reference = disassemble(rom[start:end], BASE + start, work, "reference", rom)
            compiled = disassemble(data, BASE + start, work, "compiled")
            matcher = difflib.SequenceMatcher(None, reference, compiled, autojunk=False)
            equal = sum(block.size for block in matcher.get_matching_blocks())
            total_reference += len(reference)
            total_equal += equal
            exact = "exact" if data == rom[start:end] else f"{len(data)}/{end - start} bytes"
            print(f"{name:24} {equal:4}/{len(reference):<4} instructions equal ({100 * equal / len(reference):5.1f}%), "
                  f"{len(compiled)} compiled, {exact}")
            if args.diff:
                for line in difflib.unified_diff(reference, compiled, "rom", "c", n=2, lineterm=""):
                    print(f"    {line}")
    if total_reference:
        print(f"Total: {total_equal}/{total_reference} instructions equal ({100 * total_equal / total_reference:.1f}%).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
