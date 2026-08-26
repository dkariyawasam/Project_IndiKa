#!/usr/bin/env python3
"""Audit trainer table/script wiring without touching trainer balance."""

from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OPPONENTS_PATH = ROOT / "include/constants/opponents.h"
TRAINERS_PATH = ROOT / "src/data/trainers.h"
PARTIES_PATH = ROOT / "src/data/trainer_parties.h"
LEGACY_TRAINER_SCRIPTS_PATH = ROOT / "data/scripts/trainers.inc"

SCAN_DIRS = (
    ROOT / "data",
    ROOT / "include",
    ROOT / "src",
)

EXCLUDED_ACTIVE_REF_PATHS = {
    OPPONENTS_PATH,
    TRAINERS_PATH,
    PARTIES_PATH,
    LEGACY_TRAINER_SCRIPTS_PATH,
    ROOT / "data/text/trainers.inc",
    ROOT / "src/vs_seeker.c",
}

TEXT_SUFFIXES = {
    ".c",
    ".h",
    ".inc",
    ".json",
    ".md",
    ".s",
    ".txt",
}

IGNORED_TRAINER_REF_PREFIXES = (
    "TRAINER_BATTLE",
    "TRAINER_BACK_PIC",
    "TRAINER_CARD",
    "TRAINER_CLASS",
    "TRAINER_ENCOUNTER",
    "TRAINER_FLAG",
    "TRAINER_FRONT",
    "TRAINER_GENDER",
    "TRAINER_HILL",
    "TRAINER_ID",
    "TRAINER_LINK",
    "TRAINER_MON",
    "TRAINER_PARAM",
    "TRAINER_PIC",
    "TRAINER_REMATCH",
    "TRAINER_SECRET",
    "TRAINER_SLIDE",
    "TRAINER_TYPE",
    "TRAINER_UNION",
)

IGNORED_TRAINER_REFS = {
    "TRAINER_FAN_CLUB",
    "TRAINER_PAL",
    "TRAINER_SPRITE",
}


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def rel(path: Path) -> str:
    return path.relative_to(ROOT).as_posix()


def parse_constant_defs(text: str) -> dict[str, str | None]:
    defs: dict[str, str | None] = {}
    for line in text.splitlines():
        line = line.split("//", 1)[0].strip()
        if not line.startswith("#define"):
            continue

        match = re.match(r"#define\s+(TRAINER_[A-Z0-9_]+)(?:\s+(.+))?$", line)
        if not match:
            continue

        name = match.group(1)
        value = match.group(2).strip() if match.group(2) else None
        defs[name] = value.split()[0] if value else None
    return defs


def resolve_constant_alias(name: str, constant_defs: dict[str, str | None]) -> str:
    seen: set[str] = set()
    while name not in seen:
        seen.add(name)
        value = constant_defs.get(name)
        if not value or not value.startswith("TRAINER_"):
            return name
        if value not in constant_defs:
            return value
        name = value
    return name


def parse_trainer_entries(text: str) -> set[str]:
    return set(re.findall(r"^\s*\[(TRAINER_[A-Z0-9_]+)\]\s*=", text, re.MULTILINE))


def parse_party_defs(text: str) -> set[str]:
    return set(re.findall(r"\b(sParty_[A-Za-z0-9_]+)\s*\[\]\s*=", text))


def parse_entry_party_refs(text: str) -> dict[str, str]:
    refs: dict[str, str] = {}
    trainer_re = re.compile(
        r"\[(TRAINER_[A-Z0-9_]+)\]\s*=\s*\{(.*?)(?=\n\s*\[TRAINER_[A-Z0-9_]+\]|\n\};)",
        re.DOTALL,
    )

    for trainer_id, body in trainer_re.findall(text):
        party_match = re.search(r"\.party\s*=\s*[^;]*\b(sParty_[A-Za-z0-9_]+)\b", body, re.DOTALL)
        if party_match:
            refs[trainer_id] = party_match.group(1)

    return refs


def build_resolved_entry_lookup(
    entries: set[str],
    constant_defs: dict[str, str | None],
) -> dict[str, set[str]]:
    lookup: dict[str, set[str]] = {}
    for entry in entries:
        lookup.setdefault(resolve_constant_alias(entry, constant_defs), set()).add(entry)
    return lookup


def find_trainer_entry(
    ref: str,
    entries: set[str],
    resolved_entry_lookup: dict[str, set[str]],
    constant_defs: dict[str, str | None],
) -> str | None:
    if ref in entries:
        return ref

    resolved_ref = resolve_constant_alias(ref, constant_defs)
    if resolved_ref in entries:
        return resolved_ref

    resolved_entries = resolved_entry_lookup.get(resolved_ref)
    if resolved_entries:
        return sorted(resolved_entries)[0]

    return None


def is_probable_opponent_ref(ref: str) -> bool:
    if ref == "TRAINER_NONE":
        return False
    if ref in IGNORED_TRAINER_REFS:
        return False
    return not ref.startswith(IGNORED_TRAINER_REF_PREFIXES)


def trainer_refs_in(text: str) -> set[str]:
    return {
        ref
        for ref in re.findall(r"\bTRAINER_[A-Z0-9_]+\b", text)
        if is_probable_opponent_ref(ref)
    }


def iter_text_files() -> list[Path]:
    paths: list[Path] = []
    for scan_dir in SCAN_DIRS:
        for path in scan_dir.rglob("*"):
            if not path.is_file():
                continue
            if "build" in path.parts:
                continue
            if path in EXCLUDED_ACTIVE_REF_PATHS:
                continue
            if path.suffix not in TEXT_SUFFIXES:
                continue
            paths.append(path)
    return sorted(paths)


def scan_active_refs() -> dict[str, set[str]]:
    refs: dict[str, set[str]] = {}
    for path in iter_text_files():
        for ref in trainer_refs_in(read_text(path)):
            refs.setdefault(ref, set()).add(rel(path))
    return refs


def print_ref_group(title: str, refs: dict[str, set[str]]) -> None:
    print(title)
    for ref in sorted(refs):
        locations = ", ".join(sorted(refs[ref]))
        print(f"- {ref}: {locations}")


def main() -> int:
    constant_defs = parse_constant_defs(read_text(OPPONENTS_PATH))
    constants = set(constant_defs)
    entries = parse_trainer_entries(read_text(TRAINERS_PATH))
    resolved_entry_lookup = build_resolved_entry_lookup(entries, constant_defs)
    party_defs = parse_party_defs(read_text(PARTIES_PATH))
    entry_party_refs = parse_entry_party_refs(read_text(TRAINERS_PATH))
    active_refs = scan_active_refs()
    legacy_refs = trainer_refs_in(read_text(LEGACY_TRAINER_SCRIPTS_PATH))

    active_unknown_constants = {
        ref: paths for ref, paths in active_refs.items() if ref not in constants
    }
    active_missing_entries = {
        ref: paths
        for ref, paths in active_refs.items()
        if ref in constants
        and find_trainer_entry(ref, entries, resolved_entry_lookup, constant_defs) is None
    }
    stale_legacy_only_refs = sorted(
        ref
        for ref in legacy_refs
        if ref in constants
        and find_trainer_entry(ref, entries, resolved_entry_lookup, constant_defs) is None
        and ref not in active_refs
    )

    missing_party_defs = {
        trainer: party
        for trainer, party in entry_party_refs.items()
        if party not in party_defs
    }
    unreferenced_party_defs = sorted(party_defs - set(entry_party_refs.values()))

    failed = False
    if active_unknown_constants:
        print_ref_group("Active trainer refs with no constant:", active_unknown_constants)
        failed = True
    if active_missing_entries:
        print_ref_group("Active trainer refs with no trainer table entry:", active_missing_entries)
        failed = True
    if missing_party_defs:
        print("Trainer entries with missing party definitions:")
        for trainer, party in sorted(missing_party_defs.items()):
            print(f"- {trainer}: {party}")
        failed = True
    if unreferenced_party_defs:
        print("Unreferenced party definitions:")
        for party in unreferenced_party_defs:
            print(f"- {party}")
        failed = True

    if failed:
        print()
        print("Trainer wiring audit failed.")
        return 1

    print("Trainer wiring audit passed.")
    print(f"- {len(entries)} trainer table entries parsed")
    print(f"- {len(constants)} trainer constants parsed")
    print(f"- {len(active_refs)} active trainer refs scanned")
    print(f"- {len(party_defs)} party definitions parsed")
    print("- No active refs point to missing trainer entries")
    print("- No trainer entries point to missing party definitions")
    print("- No unreferenced party definitions")
    if stale_legacy_only_refs:
        print(f"- {len(stale_legacy_only_refs)} stale legacy-only trainer aliases isolated to {rel(LEGACY_TRAINER_SCRIPTS_PATH)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
