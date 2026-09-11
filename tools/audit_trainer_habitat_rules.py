#!/usr/bin/env python3
from __future__ import annotations

import json
import re
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TRAINERS_PATH = ROOT / "src/data/trainers.h"
PARTIES_PATH = ROOT / "src/data/trainer_parties.h"
OPPONENTS_PATH = ROOT / "include/constants/opponents.h"
MAPS_DIR = ROOT / "data/maps"
SHARED_TRAINER_SCRIPTS_PATH = ROOT / "data/scripts/trainers.inc"
REPORT_PATH = ROOT / "docs/trainer-habitat-ruleset-audit.md"
TRAINER_ID_RE = re.compile(r"\bTRAINER_[A-Z0-9_]+\b")
TRAINER_BATTLE_RE = re.compile(r"^\s*trainerbattle\w*\b")
TRAINER_ALIAS_RE = re.compile(r"^\s*#define\s+(TRAINER_[A-Z0-9_]+)\s+(TRAINER_[A-Z0-9_]+)\b")
TRAINER_VALUE_RE = re.compile(r"^\s*#define\s+(TRAINER_[A-Z0-9_]+)\s+(\d+)\b")
SCRIPT_LABEL_RE = re.compile(r"^([A-Za-z0-9_]+)::")


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
    "SPECIES_SNORLAX",
    "SPECIES_SUDOWOODO",
    "SPECIES_SWAMPERT",
    "SPECIES_TORCHIC",
    "SPECIES_TREECKO",
}

DEPRECATED_CLASSES = {
    "TRAINER_CLASS_BATTLE_GIRL",
    "TRAINER_CLASS_CAMPER",
    "TRAINER_CLASS_COOLTRAINER",
    "TRAINER_CLASS_CRUSH_GIRL",
    "TRAINER_CLASS_PICKNICKER",
    "TRAINER_CLASS_ROCKER",
}

HABITAT_ALLOWED_CLASS_PARTS = {
    "Field/Route": {
        "AROMA_LADY",
        "BIKER",
        "TRENDSETTER_F",
        "BIRD_KEEPER",
        "BLACK_BELT",
        "BUG_CATCHER",
        "BUG_MANIAC",
        "CHANNELER",
        "ELECTRICIAN",
        "EXPERT",
        "FISHERMAN",
        "GENTLEMAN",
        "HIKER",
        "LADY",
        "YOUNGSTER_F",
        "POKEFAN",
        "POKEMANIAC",
        "PKMN_BREEDER",
        "PSYCHIC",
        "RANGER",
        "TRENDSETTER_M",
        "ROUGHNECK",
        "RUIN_MANIAC",
        "SCOUT",
        "SAILOR",
        "SWIMMER",
        "TRIATHLETE",
        "TUBER",
        "YOUNGSTER_M",
    },
    "Cave/Ruins": {
        "BLACK_BELT",
        "BUG_CATCHER",
        "BURGLAR",
        "CHANNELER",
        "EXPERT",
        "HIKER",
        "KINDLER",
        "POKEMANIAC",
        "PSYCHIC",
        "ROCKET",
        "RUIN_MANIAC",
        "SCOUT",
        "SCIENTIST",
    },
    "Water/Shore": {
        "TRENDSETTER_F",
        "BIRD_KEEPER",
        "FISHERMAN",
        "SAILOR",
        "SIS_AND_BRO",
        "SWIMMER",
        "TRIATHLETE",
        "TUBER",
    },
    "Victory Approach": {
        "ACE",
        "BIRD_KEEPER",
        "BLACK_BELT",
        "HIKER",
        "POKEMANIAC",
        "RUIN_MANIAC",
        "TRIATHLETE",
    },
    "Urban": {
        "TRENDSETTER_F",
        "BIKER",
        "BURGLAR",
        "ELECTRICIAN",
        "ENGINEER",
        "GENTLEMAN",
        "LADY",
        "YOUNGSTER_F",
        "POKEFAN",
        "POKEMANIAC",
        "PSYCHIC",
        "TRENDSETTER_M",
        "ROCKET",
        "ROUGHNECK",
        "SCIENTIST",
        "YOUNGSTER_M",
    },
    "Facility": {
        "ACE",
        "ADMIN",
        "BLACK_BELT",
        "BOSS",
        "BURGLAR",
        "CHANNELER",
        "ELECTRICIAN",
        "ENGINEER",
        "EXPERT",
        "GENTLEMAN",
        "MEDIUM",
        "POKEMANIAC",
        "RIVAL",
        "ROCKET",
        "SCIENTIST",
    },
    "Volcano": {
        "BIKER",
        "BLACK_BELT",
        "BURGLAR",
        "ELECTRICIAN",
        "ENGINEER",
        "EXPERT",
        "HIKER",
        "KINDLER",
        "POKEMANIAC",
        "ROCKET",
        "ROUGHNECK",
        "RUIN_MANIAC",
        "SCOUT",
        "SCIENTIST",
    },
    "Cycling": {
        "BIKER",
        "ROUGHNECK",
        "TRIATHLETE",
        "TRENDSETTERS",
    },
}

STORY_EXEMPT_PARTS = {
    "ADMIN",
    "AGATHA",
    "AIDE",
    "BLAINE",
    "BOSS",
    "BROCK",
    "BRUNO",
    "CHAMPION",
    "ELITE",
    "ERIKA",
    "GIOVANNI",
    "GYM",
    "KOGA",
    "LANCE",
    "LEADER",
    "LORELEI",
    "MISTY",
    "OAK",
    "PROF",
    "RIVAL",
    "ROCKET",
    "SABRINA",
    "SURGE",
}

STORY_EXEMPT_MAP_PARTS = {
    "Dojo",
    "Gym",
    "PokemonLeague",
    "RocketLeague",
    "SilphCo",
    "SSAnne",
}


@dataclass(frozen=True)
class Trainer:
    trainer_id: str
    trainer_class: str
    name: str
    party: str


@dataclass(frozen=True)
class TrainerUse:
    trainer_id: str
    map_name: str
    line: int
    file: Path


@dataclass(frozen=True)
class HabitatWarning:
    use_id: str
    resolved_id: str
    trainer_class: str
    trainer_name: str
    map_name: str
    habitat: str
    source: str


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def parse_c_blocks(path: Path, start_pattern: str) -> dict[str, str]:
    lines = read_text(path).splitlines()
    start_re = re.compile(start_pattern)
    blocks: dict[str, str] = {}
    i = 0
    while i < len(lines):
        match = start_re.search(lines[i])
        if match is None:
            i += 1
            continue

        name = match.group(1)
        block_lines = []
        depth = 0
        started = False
        while i < len(lines):
            line = lines[i]
            block_lines.append(line)
            depth += line.count("{") - line.count("}")
            started = started or "{" in line
            if started and depth <= 0:
                break
            i += 1

        blocks[name] = "\n".join(block_lines)
        i += 1

    return blocks


def parse_trainers() -> dict[str, Trainer]:
    blocks = parse_c_blocks(TRAINERS_PATH, r"^\s*\[(TRAINER_[A-Z0-9_]+)\]\s*=\s*\{")
    trainers: dict[str, Trainer] = {}
    for trainer_id, block in blocks.items():
        class_match = re.search(r"\.trainerClass\s*=\s*(TRAINER_CLASS_[A-Z0-9_]+)", block)
        name_match = re.search(r'\.trainerName\s*=\s*_\("([^"]*)"\)', block)
        party_match = re.search(r"sParty_[A-Za-z0-9_]+", block)
        trainers[trainer_id] = Trainer(
            trainer_id=trainer_id,
            trainer_class=class_match.group(1) if class_match else "UNKNOWN_CLASS",
            name=name_match.group(1) if name_match else "UNKNOWN",
            party=party_match.group(0) if party_match else "",
        )
    return trainers


def parse_parties() -> dict[str, set[str]]:
    blocks = parse_c_blocks(
        PARTIES_PATH,
        r"^\s*static const struct TrainerMon[A-Za-z0-9_]*\s+(sParty_[A-Za-z0-9_]+)\[\]\s*=\s*\{",
    )
    return {
        party_name: set(re.findall(r"SPECIES_[A-Z0-9_]+", block))
        for party_name, block in blocks.items()
    }


def parse_active_trainer_uses() -> list[TrainerUse]:
    uses: list[TrainerUse] = []
    for script in sorted(MAPS_DIR.glob("*/scripts.inc")):
        lines = read_text(script).splitlines()
        for index, line in enumerate(lines):
            if not TRAINER_BATTLE_RE.match(line):
                continue
            match = TRAINER_ID_RE.search(line)
            if match and match.group(0) != "TRAINER_NONE":
                uses.append(TrainerUse(match.group(0), script.parent.name, index + 1, script))
    uses.extend(parse_shared_object_trainer_uses())
    return uses


def parse_shared_object_trainer_uses() -> list[TrainerUse]:
    script_to_maps: dict[str, set[str]] = defaultdict(set)
    for map_json_path in sorted(MAPS_DIR.glob("*/map.json")):
        map_data = json.loads(read_text(map_json_path))
        map_name = map_json_path.parent.name
        for obj_event in map_data.get("object_events", []):
            if obj_event.get("trainer_type") in (None, "TRAINER_TYPE_NONE"):
                continue
            script = obj_event.get("script")
            if script:
                script_to_maps[script].add(map_name)

    if not SHARED_TRAINER_SCRIPTS_PATH.exists():
        return []

    trainer_script_uses: dict[str, tuple[str, int]] = {}
    current_label = ""
    lines = read_text(SHARED_TRAINER_SCRIPTS_PATH).splitlines()
    for index, line in enumerate(lines):
        label_match = SCRIPT_LABEL_RE.match(line)
        if label_match:
            current_label = label_match.group(1)
            continue

        if not current_label or not TRAINER_BATTLE_RE.match(line):
            continue

        trainer_match = TRAINER_ID_RE.search(line)
        if trainer_match and trainer_match.group(0) != "TRAINER_NONE":
            trainer_script_uses[current_label] = (trainer_match.group(0), index + 1)

    uses: list[TrainerUse] = []
    for script, map_names in script_to_maps.items():
        if script not in trainer_script_uses:
            continue
        trainer_id, line_no = trainer_script_uses[script]
        for map_name in sorted(map_names):
            uses.append(TrainerUse(trainer_id, map_name, line_no, SHARED_TRAINER_SCRIPTS_PATH))

    return uses


def parse_trainer_aliases(trainers: dict[str, Trainer]) -> dict[str, str]:
    aliases: dict[str, str] = {}
    numeric_values: dict[str, int] = {}
    for line in read_text(OPPONENTS_PATH).splitlines():
        match = TRAINER_ALIAS_RE.match(line)
        if match:
            aliases[match.group(1)] = match.group(2)
            continue

        value_match = TRAINER_VALUE_RE.match(line)
        if value_match:
            numeric_values[value_match.group(1)] = int(value_match.group(2))

    def resolve_numeric_value(trainer_id: str) -> int | None:
        current = trainer_id
        seen: set[str] = set()
        while current not in seen:
            seen.add(current)
            if current in numeric_values:
                return numeric_values[current]
            if current not in aliases:
                return None
            current = aliases[current]
        return None

    trainer_by_value: dict[int, str] = {}
    for trainer_id in trainers:
        value = resolve_numeric_value(trainer_id)
        if value is not None:
            trainer_by_value[value] = trainer_id

    for trainer_id in set(numeric_values) | set(aliases):
        if trainer_id in trainers:
            continue
        value = resolve_numeric_value(trainer_id)
        if value is not None and value in trainer_by_value:
            aliases[trainer_id] = trainer_by_value[value]

    return aliases


def resolve_trainer_id(trainer_id: str, trainers: dict[str, Trainer], aliases: dict[str, str]) -> str | None:
    if trainer_id in trainers:
        return trainer_id

    current = trainer_id
    seen: set[str] = set()
    while current in aliases and current not in seen:
        seen.add(current)
        current = aliases[current]
        if current in trainers:
            return current
    return None


def classify_map(map_name: str) -> str:
    if "CinnabarVolcano" in map_name:
        return "Volcano"
    if "Gym" in map_name or "Dojo" in map_name:
        return "Gym/Dojo"
    if any(part in map_name for part in (
        "MtMoon",
        "RockTunnel",
        "DiglettsCave",
        "SeafoamIslands",
        "PokemonTower",
        "PokemonMansion",
        "CeladonCave",
        "CeruleanCave",
    )):
        return "Cave/Ruins"
    if any(part in map_name for part in (
        "GameCorner",
        "PokemonLab",
        "PokemonLeague",
        "PowerPlant",
        "Rocket",
        "SilphCo",
        "SSAnne",
        "SSTidal",
    )):
        return "Facility"
    if map_name in {"Route12", "Route19", "Route20", "Route21", "Route21_North", "Route21_South", "VermilionHarbor"}:
        return "Water/Shore"
    if map_name == "Route23":
        return "Victory Approach"
    if map_name in {"Route16", "Route17", "Route18"}:
        return "Cycling"
    if "Forest" in map_name or map_name.startswith("Route"):
        return "Field/Route"
    if "City" in map_name or "Town" in map_name:
        return "Urban"
    return "Other"


def class_label(trainer_class: str) -> str:
    return trainer_class.removeprefix("TRAINER_CLASS_")


def is_story_exempt(trainer: Trainer, map_name: str) -> bool:
    text = " ".join((trainer.trainer_id, trainer.trainer_class, trainer.name, map_name))
    return any(part in text for part in STORY_EXEMPT_PARTS) or any(part in map_name for part in STORY_EXEMPT_MAP_PARTS)


def habitat_allows(trainer_class: str, habitat: str) -> bool:
    if habitat in {"Gym/Dojo", "Other"}:
        return True
    label = class_label(trainer_class)
    return any(part in label for part in HABITAT_ALLOWED_CLASS_PARTS.get(habitat, set()))


def trainer_summary(trainer: Trainer) -> str:
    return f"{class_label(trainer.trainer_class)} {trainer.name} ({trainer.trainer_id})"


def trainer_summary_for_use(use_id: str, trainer: Trainer) -> str:
    summary = trainer_summary(trainer)
    if use_id != trainer.trainer_id:
        return f"{summary}, via `{use_id}`"
    return summary


def source_link(use: TrainerUse) -> str:
    rel = use.file.relative_to(ROOT)
    return f"`{rel}:{use.line}`"


def make_bullets(rows: list[str]) -> str:
    if not rows:
        return "- None.\n"
    return "\n".join(f"- {row}" for row in rows) + "\n"


def collapse_habitat_warnings(warnings: list[HabitatWarning]) -> list[str]:
    grouped: dict[tuple[str, str, str], list[HabitatWarning]] = defaultdict(list)
    order: list[tuple[str, str, str]] = []

    for warning in warnings:
        key = (warning.resolved_id, warning.map_name, warning.habitat)
        if key not in grouped:
            order.append(key)
        grouped[key].append(warning)

    rows = []
    for key in order:
        entries = grouped[key]
        first = entries[0]
        use_ids = sorted({entry.use_id for entry in entries})
        sources = []
        for entry in entries:
            if entry.source not in sources:
                sources.append(entry.source)
        alias_ids = [use_id for use_id in use_ids if use_id != first.resolved_id]
        alias_note = ""
        if alias_ids:
            alias_note = f" Alias refs: {', '.join(f'`{alias_id}`' for alias_id in alias_ids)}."
        rows.append(
            f"`{class_label(first.trainer_class)} {first.trainer_name}` (`{first.resolved_id}`) "
            f"appears in `{first.map_name}` ({first.habitat}). "
            f"Sources: {', '.join(sources)}.{alias_note}"
        )

    return rows


def main() -> None:
    trainers = parse_trainers()
    parties = parse_parties()
    aliases = parse_trainer_aliases(trainers)
    uses = parse_active_trainer_uses()
    active_use_ids = sorted({use.trainer_id for use in uses})

    uses_by_trainer: dict[str, list[TrainerUse]] = defaultdict(list)
    resolved_ids_by_use_id: dict[str, str] = {}
    for use in uses:
        resolved_id = resolve_trainer_id(use.trainer_id, trainers, aliases)
        if resolved_id is not None:
            resolved_ids_by_use_id[use.trainer_id] = resolved_id
            uses_by_trainer[resolved_id].append(use)

    active_resolved_ids = sorted(set(resolved_ids_by_use_id.values()))

    trainers_by_identity: dict[tuple[str, str], list[str]] = defaultdict(list)
    for trainer_id, trainer in trainers.items():
        trainers_by_identity[(trainer.trainer_class, trainer.name)].append(trainer_id)

    hard_violations = []
    deprecated_violations = []
    habitat_warnings: list[HabitatWarning] = []
    active_class_counts: Counter[str] = Counter()
    habitat_counts: Counter[str] = Counter()

    for use_id in active_use_ids:
        resolved_id = resolved_ids_by_use_id.get(use_id)
        if resolved_id is None:
            hard_violations.append(f"`{use_id}` is referenced by scripts but is not defined.")
            continue

        trainer = trainers[resolved_id]
        active_class_counts[class_label(trainer.trainer_class)] += 1
        species = parties.get(trainer.party, set())
        blocked_species = sorted((species & APEX_SPECIES) | (species & REMOVED_SPECIES))
        if blocked_species:
            hard_violations.append(
                f"{trainer_summary_for_use(use_id, trainer)} uses blocked species {', '.join(f'`{s}`' for s in blocked_species)}."
            )

        if trainer.trainer_class in DEPRECATED_CLASSES:
            deprecated_violations.append(
                f"{trainer_summary_for_use(use_id, trainer)} still uses deprecated class `{trainer.trainer_class}`."
            )

        for use in [active_use for active_use in uses if active_use.trainer_id == use_id]:
            habitat = classify_map(use.map_name)
            habitat_counts[habitat] += 1
            if is_story_exempt(trainer, use.map_name):
                continue
            if not habitat_allows(trainer.trainer_class, habitat):
                habitat_warnings.append(
                    HabitatWarning(
                        use_id=use_id,
                        resolved_id=resolved_id,
                        trainer_class=trainer.trainer_class,
                        trainer_name=trainer.name,
                        map_name=use.map_name,
                        habitat=habitat,
                        source=source_link(use),
                    )
                )

    default_rematch_rows = []
    rematch_coverage_gaps = []
    rematch_chain_rows = []
    seen_rematch_identities: set[tuple[str, str]] = set()
    for trainer_id in active_resolved_ids:
        trainer = trainers[trainer_id]
        first_use = uses_by_trainer[trainer_id][0]
        if is_story_exempt(trainer, first_use.map_name):
            continue
        identity = (trainer.trainer_class, trainer.name)
        if identity in seen_rematch_identities:
            continue
        seen_rematch_identities.add(identity)
        ids = sorted(set(trainers_by_identity[identity]))
        if len(ids) == 1:
            default_rematch_rows.append(f"{trainer_summary(trainer)} at `{first_use.map_name}` ({source_link(first_use)}).")
        else:
            rematch_chain_rows.append(
                f"`{class_label(trainer.trainer_class)} {trainer.name}`: {', '.join(f'`{rematch_id}`' for rematch_id in ids)}"
            )

    collapsed_habitat_warnings = collapse_habitat_warnings(habitat_warnings)
    raw_habitat_warning_count = len(habitat_warnings)

    lines = [
        "# Trainer Habitat Ruleset Audit",
        "",
        "Generated by `tools/audit_trainer_habitat_rules.py` from placed `trainerbattle` scripts.",
        "",
        "## Summary",
        "",
        f"- Active trainer script references: {len(uses)}",
        f"- Unique active trainers: {len(active_resolved_ids)}",
        f"- Active maps with trainers: {len({use.map_name for use in uses})}",
        f"- Hard species/reference violations: {len(hard_violations)}",
        f"- Deprecated trainer-class violations: {len(deprecated_violations)}",
        f"- Habitat-fit warnings: {len(collapsed_habitat_warnings)} unique placements ({raw_habitat_warning_count} script references)",
        f"- Non-story trainers without VS Seeker coverage: {len(rematch_coverage_gaps)}",
        f"- Trainers covered by default VS Seeker fallback: {len(default_rematch_rows)}",
        f"- Defined bespoke rematch chains: {len(rematch_chain_rows)}",
        "",
        "## Hard Violations",
        "",
        make_bullets(hard_violations),
        "## Deprecated Classes",
        "",
        make_bullets(deprecated_violations),
        "## Habitat Warnings",
        "",
        make_bullets(collapsed_habitat_warnings),
        "## Non-Story Trainers Without VS Seeker Coverage",
        "",
        make_bullets(rematch_coverage_gaps),
        "## Default VS Seeker Fallback Rematches",
        "",
        "These trainers do not have bespoke higher-level rematch parties, but `src/vs_seeker.c` rematches them with their original trainer party on eligible Kanto maps.",
        "",
        make_bullets(default_rematch_rows),
        "## Defined Bespoke Rematch Chains",
        "",
        make_bullets(rematch_chain_rows),
        "## Active Trainer Class Spread",
        "",
        make_bullets([f"`{trainer_class}`: {count}" for trainer_class, count in active_class_counts.most_common()]),
        "## Active Trainer Habitat Spread",
        "",
        make_bullets([f"`{habitat}`: {count}" for habitat, count in habitat_counts.most_common()]),
    ]

    REPORT_PATH.write_text("\n".join(lines), encoding="utf-8")

    print(f"Active trainer refs: {len(uses)}")
    print(f"Unique active trainers: {len(active_resolved_ids)}")
    print(f"Hard violations: {len(hard_violations)}")
    print(f"Deprecated class violations: {len(deprecated_violations)}")
    print(
        f"Habitat warnings: {len(collapsed_habitat_warnings)} unique placements "
        f"({raw_habitat_warning_count} script refs)"
    )
    print(f"Non-story trainers without VS Seeker coverage: {len(rematch_coverage_gaps)}")
    print(f"Default VS Seeker fallback rematches: {len(default_rematch_rows)}")
    print(f"Defined bespoke rematch chains: {len(rematch_chain_rows)}")
    print(f"Wrote {REPORT_PATH.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
