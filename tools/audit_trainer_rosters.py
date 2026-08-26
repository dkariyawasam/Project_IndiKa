#!/usr/bin/env python3
"""Audit trainer rosters for species that should not appear in trainer teams."""

from __future__ import annotations

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPECIES_PATH = ROOT / "include/constants/species.h"
PARTIES_PATH = ROOT / "src/data/trainer_parties.h"
TRAINERS_PATH = ROOT / "src/data/trainers.h"


APEX_SPECIES = {
    "SPECIES_AMBIPOM",
    "SPECIES_ANNIHILAPE",
    "SPECIES_ARTICUNO",
    "SPECIES_LICKILICKY",
    "SPECIES_MEWTWO",
    "SPECIES_MIME_SR",
    "SPECIES_MOLTRES",
    "SPECIES_OSSCYTHE",
    "SPECIES_TANGROWTH",
    "SPECIES_ZAPDOS",
}

REMOVED_SPECIES = {
    "SPECIES_AGGRON",
    "SPECIES_ARON",
    "SPECIES_BLAZIKEN",
    "SPECIES_BONSLY",
    "SPECIES_COMBUSKEN",
    "SPECIES_DEOXYS",
    "SPECIES_GROUDON",
    "SPECIES_GROVYLE",
    "SPECIES_JIRACHI",
    "SPECIES_KYOGRE",
    "SPECIES_LAIRON",
    "SPECIES_LATIAS",
    "SPECIES_LATIOS",
    "SPECIES_MARSHTOMP",
    "SPECIES_MUDKIP",
    "SPECIES_MUNCHLAX",
    "SPECIES_RAYQUAZA",
    "SPECIES_SCEPTILE",
    "SPECIES_SUDOWOODO",
    "SPECIES_SWAMPERT",
    "SPECIES_TORCHIC",
    "SPECIES_TREECKO",
}


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def parse_species_constants(text: str) -> set[str]:
    return set(re.findall(r"^#define\s+(SPECIES_[A-Z0-9_]+)\b", text, re.MULTILINE))


def parse_parties(text: str) -> dict[str, list[str]]:
    parties: dict[str, list[str]] = {}
    party_start_re = re.compile(r"static\s+const\s+struct\s+\w+\s+(sParty_\w+)\[\]\s*=\s*\{")
    lines = text.splitlines()

    i = 0
    while i < len(lines):
        match = party_start_re.search(lines[i])
        if not match:
            i += 1
            continue

        party_name = match.group(1)
        body_lines = [lines[i]]
        while "};" not in lines[i] and i + 1 < len(lines):
            i += 1
            body_lines.append(lines[i])

        body = "\n".join(body_lines)
        parties[party_name] = re.findall(r"\bSPECIES_[A-Z0-9_]+\b", body)
        i += 1

    return parties


def parse_trainers(text: str) -> list[dict[str, str]]:
    trainers: list[dict[str, str]] = []
    trainer_re = re.compile(
        r"\[(TRAINER_[A-Z0-9_]+)\]\s*=\s*\{(.*?)(?=\n\s*\[TRAINER_[A-Z0-9_]+\]|\n\};)",
        re.DOTALL,
    )

    for trainer_id, body in trainer_re.findall(text):
        party_match = re.search(r"\.party\s*=\s*[^;]*\b(sParty_\w+)\b", body, re.DOTALL)
        if not party_match:
            continue

        name_match = re.search(r"\.trainerName\s*=\s*_\(\"([^\"]*)\"\)", body)
        class_match = re.search(r"\.trainerClass\s*=\s*(TRAINER_CLASS_[A-Z0-9_]+)", body)

        trainers.append(
            {
                "id": trainer_id,
                "name": name_match.group(1) if name_match else "?",
                "class": class_match.group(1) if class_match else "?",
                "party": party_match.group(1),
            }
        )

    return trainers


def trainer_label(trainer: dict[str, str]) -> str:
    return f"{trainer['id']} ({trainer['class']} {trainer['name']})"


def main() -> int:
    species_constants = parse_species_constants(read_text(SPECIES_PATH))
    parties = parse_parties(read_text(PARTIES_PATH))
    trainers = parse_trainers(read_text(TRAINERS_PATH))

    issues: list[str] = []
    total_pokemon = 0

    for trainer in trainers:
        party_name = trainer["party"]
        species = parties.get(party_name)
        if species is None:
            issues.append(f"{trainer_label(trainer)} references missing party {party_name}")
            continue

        total_pokemon += len(species)

        for mon in species:
            if mon not in species_constants:
                issues.append(f"{trainer_label(trainer)} uses unknown {mon} in {party_name}")
            if mon in APEX_SPECIES:
                issues.append(f"{trainer_label(trainer)} uses Apex {mon} in {party_name}")
            if mon in REMOVED_SPECIES:
                issues.append(f"{trainer_label(trainer)} uses removed {mon} in {party_name}")

    orphan_parties = sorted(set(parties) - {trainer["party"] for trainer in trainers})

    if issues:
        print("Trainer roster audit failed:")
        for issue in issues:
            print(f"- {issue}")
        print()
        print(f"Scanned {len(trainers)} trainers, {len(parties)} party blocks, {total_pokemon} Pokemon entries.")
        return 1

    print("Trainer roster audit passed.")
    print(f"- {len(trainers)} trainers with parties checked")
    print(f"- {len(parties)} party blocks parsed")
    print(f"- {total_pokemon} Pokemon entries scanned")
    print("- No Apex, removed, or unknown species in trainer rosters")
    if orphan_parties:
        print(f"- {len(orphan_parties)} unused party blocks present")
    return 0


if __name__ == "__main__":
    sys.exit(main())
