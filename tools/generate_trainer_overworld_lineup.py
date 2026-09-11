#!/usr/bin/env python3
"""Generate a contact sheet of active trainer-class overworld sprites."""

from __future__ import annotations

import importlib.util
import json
import math
from collections import Counter, defaultdict
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
PEOPLE_DIR = ROOT / "graphics/object_events/pics/people"
AUDIT_SCRIPT = ROOT / "tools/audit_trainer_overworlds.py"
OUT_PNG = ROOT / "docs/trainer-overworld-lineup.png"


GFX_STEMS = {
    "POKE_MANIAC": "pokemaniac",
    "POKEMANIAC": "pokemaniac",
    "BREEDER": "pokemon_breeder",
    "POKEMON_BREEDER": "pokemon_breeder",
    "BLACK_BELT": "black_belt",
    "BLACK_BELT_F": "black_belt_f",
    "CRUSH_GIRL": "black_belt_f",
    "ROCKET_M": "rocket_m",
    "ROCKET_F": "rocket_f",
    "ROCKET_ADMIN_ARCHER": "rocket_admin_archer",
    "ROCKET_ARIANA": "rocket_admin_ariana",
    "ROCKET_ADMIN_ARIANA": "rocket_admin_ariana",
    "ROCKET_PETREL": "rocket_admin_petrel",
    "ROCKET_ADMIN_PETREL": "rocket_admin_petrel",
    "ROCKET_ADMIN_PROTON": "rocket_admin_proton",
    "ROUGHNECK": "cue_ball",
    "FISHER": "fisherman",
    "FISHERMAN": "fisherman",
    "SCOUT_F": "scout_f",
    "SCOUT_M": "scout_m",
    "LT_SURGE": "lt_surge",
    "PSYCHIC_M": "psychic_m",
    "JANINE": "janine",
    "MISTY_WATER": "misty_water",
}


PIC_TO_GFX = {
    "BLACK_BELT_F": "BLACK_BELT_F",
    "BLACK_BELT_M": "BLACK_BELT",
    "BLACK_BELT": "BLACK_BELT",
    "FISHERMAN": "FISHERMAN",
    "PKMN_BREEDER": "BREEDER",
    "POKEMON_BREEDER": "BREEDER",
    "POKEMON_BREEDER_F_RS": "BREEDER",
    "POKEMON_BREEDER_M_RS": "BREEDER",
    "POKEMANIAC": "POKEMANIAC",
    "ROCKET_GRUNT_M": "ROCKET_M",
    "ROCKET_GRUNT_F": "ROCKET_F",
    "ROCKET_ADMIN_ARIANA": "ROCKET_ADMIN_ARIANA",
    "LEADER_BROCK": "BROCK",
    "LEADER_MISTY": "MISTY",
    "LEADER_LT_SURGE": "LT_SURGE",
    "LEADER_ERIKA": "ERIKA",
    "LEADER_KOGA": "KOGA",
    "LEADER_BLAINE": "BLAINE",
    "LEADER_SABRINA": "SABRINA",
    "LEADER_GIOVANNI": "GIOVANNI",
    "ELITE_FOUR_AGATHA": "AGATHA",
    "ELITE_FOUR_BRUNO": "BRUNO",
    "ELITE_FOUR_LANCE": "LANCE",
    "ELITE_FOUR_LORELEI": "LORELEI",
    "SWIMMER_M": "SWIMMER_M_LAND",
    "SWIMMER_F": "SWIMMER_F_LAND",
    "TUBER_M": "TUBER_M_LAND",
    "TUBER_F": "TUBER_F_LAND",
    "CYCLING_TRIATHLETE_M": "TRIATHLETE_M_CYCLING",
    "CYCLING_TRIATHLETE_F": "TRIATHLETE_F_CYCLING",
    "RUNNING_TRIATHLETE_M": "TRIATHLETE_M_LAND",
    "RUNNING_TRIATHLETE_F": "TRIATHLETE_F_LAND",
    "SWIMMING_TRIATHLETE_M": "TRIATHLETE_M_WATER",
    "SWIMMING_TRIATHLETE_F": "TRIATHLETE_F_WATER",
}


EXTRA_NAMED_ROWS = [
    {
        "label": "ROCKET_ADMIN_PETREL",
        "class": "ROCKET_ADMIN",
        "pic": "ROCKET_GRUNT_M",
        "gfx": "ROCKET_ADMIN_PETREL",
        "count": 0,
        "defined_count": 1,
        "trainer_examples": ["PETREL"],
        "maps": [],
    },
    {
        "label": "ROCKET_ADMIN_ARCHER",
        "class": "ROCKET_ADMIN",
        "pic": "ROCKET_GRUNT_M",
        "gfx": "ROCKET_ADMIN_ARCHER",
        "count": 0,
        "defined_count": 1,
        "trainer_examples": ["ARCHER"],
        "maps": [],
    },
    {
        "label": "ROCKET_ADMIN_ARIANA",
        "class": "ROCKET_ADMIN",
        "pic": "ROCKET_ADMIN_ARIANA",
        "gfx": "ROCKET_ADMIN_ARIANA",
        "count": 0,
        "defined_count": 1,
        "trainer_examples": ["ARIANA"],
        "maps": [],
    },
    {
        "label": "ROCKET_ADMIN_PROTON",
        "class": "ROCKET_ADMIN",
        "pic": "ROCKET_GRUNT_M",
        "gfx": "ROCKET_ADMIN_PROTON",
        "count": 0,
        "defined_count": 1,
        "trainer_examples": ["PROTON"],
        "maps": [],
    },
    {
        "label": "JANINE",
        "class": "NINJA",
        "pic": "NINJA",
        "gfx": "JANINE",
        "count": 0,
        "defined_count": 1,
        "trainer_examples": ["JANINE"],
        "maps": ["CinnabarVolcano_RightCorridor_3F"],
    },
    {
        "label": "MISTY_WATER",
        "class": "LEADER",
        "pic": "LEADER_MISTY",
        "gfx": "MISTY_WATER",
        "count": 0,
        "defined_count": 1,
        "trainer_examples": ["MISTY"],
        "maps": [],
    },
]

DOUBLE_CLASSES = {
    "ACES",
    "CRUSH_KIN",
    "SIS_AND_BRO",
    "TWINS",
    "TRENDSETTERS",
}

DOUBLE_GFX_ORDER = {
    "ACES": ["ACE_TRAINER_M", "ACE_TRAINER_F"],
    "SIS_AND_BRO": ["TUBER_M_WATER", "SWIMMER_F_WATER"],
    "TRENDSETTERS": ["TRENDSETTER_M", "TRENDSETTER_F"],
}

DOUBLE_LABELS = {
    "SIS_AND_BRO": "SWIM_SIBS",
    "TRENDSETTERS": "TRENDSETTERS",
}


def load_audit_module():
    spec = importlib.util.spec_from_file_location("trainer_overworld_audit", AUDIT_SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Could not load {AUDIT_SCRIPT}")

    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def find_gfx_for_class_pic(trainer_class: str, trainer_pic: str) -> str | None:
    for seed in (trainer_pic, trainer_class):
        if not seed or seed.startswith("UNUSED"):
            continue

        candidate = PIC_TO_GFX.get(seed, seed)
        stem = GFX_STEMS.get(candidate, candidate.lower())
        if (PEOPLE_DIR / f"{stem}.png").exists():
            return candidate

    return None


def suffix(value: str, prefix: str) -> str:
    return value[len(prefix) :] if value.startswith(prefix) else value


def collect_rows() -> list[dict[str, object]]:
    audit = load_audit_module()
    trainers = audit.parse_trainers()
    scripts = [audit.GLOBAL_TRAINERS_INC, *sorted(audit.MAPS_DIR.glob("*/scripts.inc"))]
    script_labels = audit.parse_script_labels(scripts, set(trainers))

    rows: dict[tuple[str, str, str], dict[str, object]] = {}
    maps_by_key: dict[tuple[str, str, str], set[str]] = defaultdict(set)
    count_by_key: Counter[tuple[str, str, str]] = Counter()

    for map_json in sorted(audit.MAPS_DIR.glob("*/map.json")):
        data = json.loads(map_json.read_text())
        map_name = data.get("name") or map_json.parent.name

        for obj in data.get("object_events", []):
            script_info = script_labels.get(obj.get("script", ""))
            if not script_info:
                continue

            trainer_ids = script_info["trainer_ids"]
            graphics_id = obj.get("graphics_id", "")
            if len(trainer_ids) != 1 or graphics_id.startswith("OBJ_EVENT_GFX_VAR_"):
                continue

            trainer = trainers[trainer_ids[0]]
            trainer_class = suffix(trainer["class"], "TRAINER_CLASS_")
            trainer_pic = suffix(trainer["pic"], "TRAINER_PIC_")
            gfx = suffix(graphics_id, "OBJ_EVENT_GFX_")
            key = (trainer_class, trainer_pic, gfx)

            rows.setdefault(
                key,
                {
                    "class": trainer_class,
                    "pic": trainer_pic,
                    "gfx": gfx,
                    "trainer_examples": [],
                },
            )
            if len(rows[key]["trainer_examples"]) < 4:
                rows[key]["trainer_examples"].append(trainer["name"])
            maps_by_key[key].add(map_name)
            count_by_key[key] += 1

    output = []
    for key, row in rows.items():
        row = dict(row)
        row["count"] = count_by_key[key]
        row["maps"] = sorted(maps_by_key[key])
        output.append(row)

    represented_class_pics = {(row["class"], row["pic"]) for row in output}
    total_by_class_pic: Counter[tuple[str, str]] = Counter()
    examples_by_class_pic: dict[tuple[str, str], list[str]] = defaultdict(list)

    for trainer in trainers.values():
        trainer_class = suffix(trainer["class"], "TRAINER_CLASS_")
        trainer_pic = suffix(trainer["pic"], "TRAINER_PIC_")
        if not trainer_class or not trainer_pic:
            continue
        if trainer_class == "PKMN_TRAINER_UNUSED" or trainer_class.startswith("UNUSED"):
            continue
        if trainer_pic.startswith("UNUSED"):
            continue

        key = (trainer_class, trainer_pic)
        total_by_class_pic[key] += 1
        if len(examples_by_class_pic[key]) < 4:
            examples_by_class_pic[key].append(trainer["name"])

    for (trainer_class, trainer_pic), total in total_by_class_pic.items():
        if (trainer_class, trainer_pic) in represented_class_pics:
            continue

        gfx = find_gfx_for_class_pic(trainer_class, trainer_pic)
        if gfx is None:
            continue

        output.append(
            {
                "class": trainer_class,
                "pic": trainer_pic,
                "gfx": gfx,
                "count": 0,
                "defined_count": total,
                "trainer_examples": examples_by_class_pic[(trainer_class, trainer_pic)],
                "maps": [],
            }
        )

    output = [row for row in output if row["class"] != "ROCKET_ADMIN"]
    output.extend(dict(row) for row in EXTRA_NAMED_ROWS)
    output = compose_double_rows(output)

    return sorted(output, key=lambda row: (str(row.get("label", row["class"])), str(row["pic"]), str(row["gfx"])))


def compose_double_rows(rows: list[dict[str, object]]) -> list[dict[str, object]]:
    single_rows: list[dict[str, object]] = []
    double_groups: dict[tuple[str, str, tuple[str, ...], tuple[str, ...]], dict[str, object]] = {}

    for row in rows:
        trainer_class = str(row["class"])
        if trainer_class not in DOUBLE_CLASSES:
            single_rows.append(row)
            continue

        key = (
            trainer_class,
            str(row["pic"]),
            tuple(row.get("trainer_examples", [])),
            tuple(row.get("maps", [])),
        )
        group = double_groups.setdefault(
            key,
            {
                "class": trainer_class,
                "pic": row["pic"],
                "gfx": [],
                "count": 0,
                "defined_count": row.get("defined_count", 0),
                "trainer_examples": list(row.get("trainer_examples", [])),
                "maps": list(row.get("maps", [])),
            },
        )
        group["gfx"].append(str(row["gfx"]))
        group["count"] = max(int(group.get("count", 0)), int(row.get("count", 0)))
        group["defined_count"] = max(int(group.get("defined_count", 0)), int(row.get("defined_count", 0)))

    for group in double_groups.values():
        trainer_class = str(group["class"])
        preferred = DOUBLE_GFX_ORDER.get(trainer_class, [])
        present = list(dict.fromkeys(str(gfx) for gfx in group["gfx"]))
        ordered = [gfx for gfx in preferred if gfx in present]
        ordered.extend(gfx for gfx in present if gfx not in ordered)
        group["gfx"] = ordered
        group["label"] = DOUBLE_LABELS.get(trainer_class, trainer_class)
        single_rows.append(group)

    return single_rows


def load_sprite(gfx: str) -> Image.Image:
    stem = GFX_STEMS.get(gfx, gfx.lower())
    path = PEOPLE_DIR / f"{stem}.png"
    if not path.exists():
        raise FileNotFoundError(f"No overworld sprite PNG for OBJ_EVENT_GFX_{gfx}: {path}")

    source = Image.open(path)
    if source.mode == "P":
        alpha = Image.new("L", source.size, 255)
        pixels = source.get_flattened_data() if hasattr(source, "get_flattened_data") else source.getdata()
        alpha.putdata([0 if px == 0 else 255 for px in pixels])
        sprite = source.convert("RGBA")
        sprite.putalpha(alpha)
    else:
        sprite = source.convert("RGBA")

    frame_w = sprite.width // 10 if sprite.width % 10 == 0 else 16
    frame_w = frame_w if frame_w in {16, 32} else 16
    return sprite.crop((0, 0, min(frame_w, sprite.width), min(32, sprite.height)))


def load_row_sprite(row: dict[str, object]) -> Image.Image:
    gfx = row["gfx"]
    if not isinstance(gfx, list):
        return load_sprite(str(gfx))

    sprites = [load_sprite(str(part)) for part in gfx]
    width = sum(sprite.width for sprite in sprites)
    height = max(sprite.height for sprite in sprites)
    composed = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    x = 0
    for sprite in sprites:
        composed.paste(sprite, (x, height - sprite.height), sprite)
        x += sprite.width
    return composed


def wrap(draw: ImageDraw.ImageDraw, text: str, font: ImageFont.ImageFont, width: int) -> list[str]:
    words = text.split("_")
    lines: list[str] = []
    current = ""
    for word in words:
        trial = word if not current else f"{current}_{word}"
        if draw.textbbox((0, 0), trial, font=font)[2] <= width or not current:
            current = trial
        else:
            lines.append(current)
            current = word
    if current:
        lines.append(current)
    return lines


def make_sheet(rows: list[dict[str, object]]) -> None:
    scale = 4
    cols = 5
    cell_w = 210
    cell_h = 162
    pad = 16
    title_h = 54
    rows_n = math.ceil(len(rows) / cols)

    sheet = Image.new("RGB", (cols * cell_w + pad * 2, rows_n * cell_h + title_h + pad), "#f4f6f8")
    draw = ImageDraw.Draw(sheet)

    def font(size: int, bold: bool = False) -> ImageFont.ImageFont:
        names = [
            "/System/Library/Fonts/Supplemental/Arial Bold.ttf" if bold else "/System/Library/Fonts/Supplemental/Arial.ttf",
            "/Library/Fonts/Arial Bold.ttf" if bold else "/Library/Fonts/Arial.ttf",
        ]
        for name in names:
            try:
                return ImageFont.truetype(name, size)
            except OSError:
                pass
        return ImageFont.load_default()

    title_font = font(22, bold=True)
    label_font = font(13, bold=True)
    detail_font = font(10)

    draw.text((pad, pad), "Trainer Class Overworld Lineup", fill="#17202a", font=title_font)
    draw.text((pad, pad + 28), f"{len(rows)} placed or defined class / overworld variants", fill="#5b6470", font=detail_font)

    for i, row in enumerate(rows):
        col = i % cols
        row_i = i // cols
        x = pad + col * cell_w
        y = title_h + row_i * cell_h

        draw.rounded_rectangle((x + 4, y + 4, x + cell_w - 8, y + cell_h - 8), radius=8, fill="#ffffff", outline="#d8dde5")

        sprite = load_row_sprite(row)
        sprite = sprite.resize((sprite.width * scale, sprite.height * scale), Image.Resampling.NEAREST)
        sx = x + (cell_w - sprite.width) // 2
        sy = y + 12
        sheet.paste(sprite, (sx, sy), sprite)

        label_y = y + 12 + 32 * scale + 7
        label = str(row.get("label", row["class"]))
        for line in wrap(draw, label, label_font, cell_w - 24)[:2]:
            tw = draw.textbbox((0, 0), line, font=label_font)[2]
            draw.text((x + (cell_w - tw) // 2, label_y), line, fill="#111827", font=label_font)
            label_y += 15

        gfx = row["gfx"]
        gfx_text = f"gfx: {'+'.join(gfx) if isinstance(gfx, list) else gfx}"
        if row["count"]:
            count_text = f"placed: {row['count']}"
        else:
            count_text = f"defined: {row.get('defined_count', 0)}"
        tw = draw.textbbox((0, 0), gfx_text, font=detail_font)[2]
        draw.text((x + (cell_w - tw) // 2, label_y + 2), gfx_text, fill="#596273", font=detail_font)
        tw = draw.textbbox((0, 0), count_text, font=detail_font)[2]
        draw.text((x + (cell_w - tw) // 2, label_y + 16), count_text, fill="#596273", font=detail_font)

    OUT_PNG.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUT_PNG)


def main() -> None:
    rows = collect_rows()
    make_sheet(rows)
    print(f"wrote {OUT_PNG}")


if __name__ == "__main__":
    main()
