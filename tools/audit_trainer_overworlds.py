#!/usr/bin/env python3
"""Audit placed trainer objects against their expected overworld graphics."""

from __future__ import annotations

import json
import re
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TRAINERS_H = ROOT / "src/data/trainers.h"
MAPS_DIR = ROOT / "data/maps"
GLOBAL_TRAINERS_INC = ROOT / "data/scripts/trainers.inc"
OUT_JSON = ROOT / "docs/trainer-overworld-audit.json"
OUT_MD = ROOT / "docs/trainer-overworld-audit.md"


ALIASES = {
    "FISHERMAN": {"FISHER", "FISHERMAN"},
    "FISHER": {"FISHER", "FISHERMAN"},
    "POKEMANIAC": {"POKEMANIAC", "POKE_MANIAC", "POKEFAN", "POKEFAN_M", "POKEFAN_F"},
    "POKE_MANIAC": {"POKEMANIAC", "POKE_MANIAC", "POKEFAN", "POKEFAN_M", "POKEFAN_F"},
    "POKEFAN": {"POKEMANIAC", "POKE_MANIAC", "POKEFAN", "POKEFAN_M", "POKEFAN_F"},
    "POKEFAN_M": {"POKEMANIAC", "POKE_MANIAC", "POKEFAN", "POKEFAN_M"},
    "POKEFAN_F": {"POKEMANIAC", "POKE_MANIAC", "POKEFAN", "POKEFAN_F"},
    "BLACK_BELT": {"BLACK_BELT", "BLACK_BELT_M"},
    "BLACK_BELT_M": {"BLACK_BELT", "BLACK_BELT_M"},
    "BLACK_BELT_F": {"BLACK_BELT_F", "CRUSH_GIRL"},
    "CRUSH_GIRL": {"BLACK_BELT_F", "CRUSH_GIRL"},
    "ROUGHNECK": {"ROUGHNECK", "CUE_BALL"},
    "CUE_BALL": {"ROUGHNECK", "CUE_BALL"},
    "ROCKET_GRUNT_M": {"ROCKET_GRUNT_M", "TEAM_ROCKET_M", "ROCKET_M", "ROCKET"},
    "ROCKET_GRUNT_F": {"ROCKET_GRUNT_F", "TEAM_ROCKET_F", "ROCKET_F", "ROCKET"},
    "TEAM_ROCKET_M": {"TEAM_ROCKET_M", "ROCKET_GRUNT_M", "ROCKET_M", "ROCKET"},
    "TEAM_ROCKET_F": {"TEAM_ROCKET_F", "ROCKET_GRUNT_F", "ROCKET_F", "ROCKET"},
    "ROCKET_M": {"TEAM_ROCKET_M", "ROCKET_GRUNT_M", "ROCKET_M", "ROCKET"},
    "ROCKET_F": {"TEAM_ROCKET_F", "ROCKET_GRUNT_F", "ROCKET_F", "ROCKET"},
    "ROCKET": {"TEAM_ROCKET_M", "TEAM_ROCKET_F", "ROCKET_GRUNT_M", "ROCKET_GRUNT_F", "ROCKET_M", "ROCKET_F", "ROCKET"},
    "ARIANA": {"ROCKET_ARIANA", "ARIANA"},
    "ROCKET_ARIANA": {"ROCKET_ARIANA", "ARIANA"},
    "PETREL": {"ROCKET_PETREL", "PETREL"},
    "ROCKET_PETREL": {"ROCKET_PETREL", "PETREL"},
    "PROTON": {"ROCKET_PROTON", "PROTON"},
    "ROCKET_PROTON": {"ROCKET_PROTON", "PROTON"},
    "ARCHER": {"ROCKET_ARCHER", "ARCHER"},
    "ROCKET_ARCHER": {"ROCKET_ARCHER", "ARCHER"},
    "PKMN_BREEDER": {"PKMN_BREEDER", "POKEMON_BREEDER", "BREEDER"},
    "POKEMON_BREEDER": {"PKMN_BREEDER", "POKEMON_BREEDER", "BREEDER"},
    "BREEDER": {"PKMN_BREEDER", "POKEMON_BREEDER", "BREEDER"},
    "ELECTRICIAN": {"ELECTRICIAN"},
    "RS_PSYCHIC": {"CHANNELER", "PSYCHIC_M", "PSYCHIC_F"},
    "PSYCHIC_M": {"CHANNELER", "PSYCHIC_M"},
    "PSYCHIC_F": {"CHANNELER", "PSYCHIC_F"},
    "TUBER_F": {"TUBER_F", "TUBER_F_WATER", "TUBER_F_LAND"},
    "TUBER_M": {"TUBER_M", "TUBER_M_WATER", "TUBER_M_LAND"},
    "RS_TUBER_F": {"RS_TUBER_F", "TUBER_F", "TUBER_F_WATER", "TUBER_F_LAND"},
    "RS_TUBER_M": {"RS_TUBER_M", "TUBER_M", "TUBER_M_WATER", "TUBER_M_LAND"},
    "TUBER_F_WATER": {"TUBER_F_WATER", "TUBER_F", "TUBER_F_LAND"},
    "TUBER_M_WATER": {"TUBER_M_WATER", "TUBER_M", "TUBER_M_LAND"},
    "TUBER_F_LAND": {"TUBER_F_LAND", "TUBER_F", "TUBER_F_WATER"},
    "TUBER_M_LAND": {"TUBER_M_LAND", "TUBER_M", "TUBER_M_WATER"},
    "SWIMMER_F": {"SWIMMER_F", "SWIMMER_F_WATER", "SWIMMER_F_LAND"},
    "SWIMMER_M": {"SWIMMER_M", "SWIMMER_M_WATER", "SWIMMER_M_LAND"},
    "SWIMMER_F_WATER": {"SWIMMER_F_WATER", "SWIMMER_F", "SWIMMER_F_LAND"},
    "SWIMMER_M_WATER": {"SWIMMER_M_WATER", "SWIMMER_M", "SWIMMER_M_LAND"},
    "SWIMMER_F_LAND": {"SWIMMER_F_LAND", "SWIMMER_F", "SWIMMER_F_WATER"},
    "SWIMMER_M_LAND": {"SWIMMER_M_LAND", "SWIMMER_M", "SWIMMER_M_WATER"},
    "TRIATHLETE_F_WATER": {"TRIATHLETE_F_WATER", "TRIATHLETE_F"},
    "TRIATHLETE_M_WATER": {"TRIATHLETE_M_WATER", "TRIATHLETE_M"},
    "YOUNG_COUPLE": {"YOUNG_COUPLE", "BEAUTY", "RICH_BOY"},
    "SIS_AND_BRO": {"SIS_AND_BRO", "SWIMMER_F_WATER", "SWIMMER_F_LAND", "TUBER_M_WATER", "TUBER_M_LAND"},
    "COOL_COUPLE": {"COOL_COUPLE", "ACE_TRAINER_M", "ACE_TRAINER_F"},
    "LEADER_GIOVANNI": {"LEADER_GIOVANNI", "GIOVANNI"},
    "GIOVANNI": {"LEADER_GIOVANNI", "GIOVANNI"},
}


def strip_prefix(value: str, prefix: str) -> str:
    return value[len(prefix) :] if value.startswith(prefix) else value


def parse_trainers() -> dict[str, dict[str, str]]:
    trainers: dict[str, dict[str, str]] = {}
    current_id: str | None = None
    block: list[str] = []

    for line in TRAINERS_H.read_text().splitlines():
        match = re.match(r"\s*\[(TRAINER_[A-Z0-9_]+)\]\s*=\s*\{", line)
        if match:
            current_id = match.group(1)
            block = [line]
            continue

        if current_id is None:
            continue

        block.append(line)
        if re.match(r"\s*\},?", line):
            block_text = "\n".join(block)
            trainers[current_id] = {
                "class": find_field(block_text, r"\.trainerClass\s*=\s*(TRAINER_CLASS_[A-Z0-9_]+)"),
                "pic": find_field(block_text, r"\.trainerPic\s*=\s*(TRAINER_PIC_[A-Z0-9_]+)"),
                "name": find_field(block_text, r'\.trainerName\s*=\s*_\("([^"]*)"\)'),
            }
            current_id = None
            block = []

    return trainers


def find_field(text: str, pattern: str) -> str:
    match = re.search(pattern, text)
    return match.group(1) if match else ""


def parse_script_labels(paths: list[Path], known_trainers: set[str]) -> dict[str, dict[str, object]]:
    labels: dict[str, dict[str, object]] = {}
    current_label: str | None = None
    body: list[str] = []

    def finish_label() -> None:
        nonlocal current_label, body
        if current_label is None:
            return

        body_text = "\n".join(body)
        trainer_ids = sorted(set(re.findall(r"\b(TRAINER_[A-Z0-9_]+)\b", body_text)) & known_trainers)
        if trainer_ids:
            labels[current_label] = {
                "trainer_ids": trainer_ids,
                "has_trainerbattle": bool(re.search(r"\btrainerbattle", body_text)),
            }

    for path in paths:
        if not path.exists():
            continue
        current_label = None
        body = []
        for line in path.read_text().splitlines():
            match = re.match(r"^([A-Za-z0-9_]+)::\s*$", line)
            if match:
                finish_label()
                current_label = match.group(1)
                body = []
            elif current_label is not None:
                body.append(line)
        finish_label()

    return labels


def expected_suffixes(trainer: dict[str, str]) -> set[str]:
    seeds = {
        strip_prefix(trainer["class"], "TRAINER_CLASS_"),
        strip_prefix(trainer["pic"], "TRAINER_PIC_"),
    }
    seeds.discard("")

    expected: set[str] = set()
    for seed in seeds:
        expected.add(seed)
        expected.update(ALIASES.get(seed, set()))
    return expected


def classify_object(graphics_id: str, trainer: dict[str, str]) -> tuple[bool, list[str], str]:
    actual = strip_prefix(graphics_id, "OBJ_EVENT_GFX_")
    expected = sorted(expected_suffixes(trainer))
    return actual in expected, expected, actual


def audit() -> dict[str, object]:
    trainers = parse_trainers()
    script_paths = [GLOBAL_TRAINERS_INC, *sorted(MAPS_DIR.glob("*/scripts.inc"))]
    script_labels = parse_script_labels(script_paths, set(trainers))
    mismatches: list[dict[str, object]] = []
    skipped_multi: list[dict[str, object]] = []
    skipped_dynamic: list[dict[str, object]] = []
    summary = Counter()

    for map_json in sorted(MAPS_DIR.glob("*/map.json")):
        data = json.loads(map_json.read_text())
        map_name = data.get("name") or map_json.parent.name
        summary["maps_scanned"] += 1

        for index, obj in enumerate(data.get("object_events", []), start=1):
            script = obj.get("script", "")
            script_info = script_labels.get(script)
            if not script_info:
                continue

            trainer_ids = script_info["trainer_ids"]
            if len(trainer_ids) != 1:
                summary["skipped_multi_trainer_scripts"] += 1
                skipped_multi.append({"map": map_name, "object": index, "script": script, "trainers": trainer_ids})
                continue

            graphics_id = obj.get("graphics_id", "")
            if graphics_id.startswith("OBJ_EVENT_GFX_VAR_"):
                summary["skipped_dynamic_graphics"] += 1
                skipped_dynamic.append({"map": map_name, "object": index, "script": script, "trainer": trainer_ids[0]})
                continue

            summary["trainer_object_events"] += 1
            trainer = trainers[trainer_ids[0]]
            matches, expected, actual = classify_object(graphics_id, trainer)

            if matches:
                summary["matched"] += 1
            else:
                summary["mismatches"] += 1
                mismatches.append(
                    {
                        "map": map_name,
                        "object": index,
                        "x": obj.get("x"),
                        "y": obj.get("y"),
                        "script": script,
                        "trainer": trainer_ids[0],
                        "trainer_name": trainer["name"],
                        "trainer_class": trainer["class"],
                        "trainer_pic": trainer["pic"],
                        "graphics_id": graphics_id,
                        "actual_suffix": actual,
                        "expected_suffixes": expected,
                    }
                )

    return {
        "summary": dict(summary),
        "mismatches": mismatches,
        "skipped_multi_trainer_scripts": skipped_multi,
        "skipped_dynamic_graphics": skipped_dynamic,
        "aliases": {key: sorted(value) for key, value in sorted(ALIASES.items())},
    }


def write_markdown(report: dict[str, object]) -> None:
    summary = report["summary"]
    lines = [
        "# Trainer Overworld Audit",
        "",
        "Generated by `tools/audit_trainer_overworlds.py`.",
        "",
        "## Summary",
        "",
        f"- Maps scanned: {summary.get('maps_scanned', 0)}",
        f"- Trainer object events checked: {summary.get('trainer_object_events', 0)}",
        f"- Matched: {summary.get('matched', 0)}",
        f"- Mismatches: {summary.get('mismatches', 0)}",
        f"- Skipped dynamic graphics: {summary.get('skipped_dynamic_graphics', 0)}",
        f"- Skipped multi-trainer scripts: {summary.get('skipped_multi_trainer_scripts', 0)}",
        "",
        "## Intentional Aliases Suppressed",
        "",
        "These names are treated as compatible when a trainer class/front-pic and overworld use different intentional names.",
        "",
        "| Seed | Accepted overworld suffixes |",
        "| --- | --- |",
    ]

    aliases = report["aliases"]
    for key, value in aliases.items():
        lines.append(f"| `{key}` | {', '.join(f'`{item}`' for item in value)} |")

    lines.extend(["", "## Mismatches", ""])
    mismatches = report["mismatches"]
    if mismatches:
        lines.extend(
            [
                "| Map | Obj | Coord | Trainer | Class | Pic | Graphics | Expected |",
                "| --- | ---: | --- | --- | --- | --- | --- | --- |",
            ]
        )
        for row in mismatches:
            expected = ", ".join(f"`{item}`" for item in row["expected_suffixes"])
            lines.append(
                "| {map} | {object} | {x},{y} | `{trainer}` ({trainer_name}) | `{trainer_class}` | `{trainer_pic}` | `{graphics_id}` | {expected} |".format(
                    **row, expected=expected
                )
            )
    else:
        lines.append("No real trainer overworld mismatches found.")

    if report["skipped_multi_trainer_scripts"]:
        lines.extend(["", "## Skipped Multi-Trainer Scripts", ""])
        lines.extend(["| Map | Obj | Script | Trainers |", "| --- | ---: | --- | --- |"])
        for row in report["skipped_multi_trainer_scripts"]:
            lines.append(
                f"| {row['map']} | {row['object']} | `{row['script']}` | "
                + ", ".join(f"`{trainer}`" for trainer in row["trainers"])
                + " |"
            )

    OUT_MD.write_text("\n".join(lines) + "\n")


def main() -> None:
    report = audit()
    OUT_JSON.write_text(json.dumps(report, indent=2) + "\n")
    write_markdown(report)
    summary = report["summary"]
    print(
        "checked {checked} trainer objects; {matched} matched; {mismatches} mismatches".format(
            checked=summary.get("trainer_object_events", 0),
            matched=summary.get("matched", 0),
            mismatches=summary.get("mismatches", 0),
        )
    )


if __name__ == "__main__":
    main()
