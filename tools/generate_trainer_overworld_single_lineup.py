#!/usr/bin/env python3
"""Generate a compact lineup of single-trainer overworld sprite variants."""

from __future__ import annotations

import colorsys
import importlib.util
import json
import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
LINEUP_SCRIPT = ROOT / "tools/generate_trainer_overworld_lineup.py"
OUT_PNG = ROOT / "docs/trainer-overworld-lineup-single.png"
OUT_VALUE_PNG = ROOT / "docs/trainer-overworld-colors-by-value.png"
OUT_VALUE_JSON = ROOT / "docs/trainer-overworld-colors-by-value.json"

EXCLUDED_CLASSES = {
    "ACES",
    "CRUSH_KIN",
    "RS_TWINS",
    "SIS_AND_BRO",
    "TWINS",
    "TRENDSETTERS",
}

LABEL_OVERRIDES = {
    ("BLACK_BELT", "BLACK_BELT_M", "BLACK_BELT"): "BLACK_BELT_M",
    ("BLACK_BELT", "BLACK_BELT_F", "CRUSH_GIRL"): "BLACK_BELT_F",
}


def load_lineup_module():
    spec = importlib.util.spec_from_file_location("trainer_overworld_lineup", LINEUP_SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Could not load {LINEUP_SCRIPT}")

    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


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


def display_label(row: dict[str, object]) -> str:
    key = (str(row["class"]), str(row["pic"]), str(row["gfx"]))
    if key in LABEL_OVERRIDES:
        return LABEL_OVERRIDES[key]

    if row.get("label"):
        return str(row["label"])

    trainer_class = str(row["class"])
    trainer_pic = str(row["pic"])
    if trainer_pic.startswith(f"{trainer_class}_"):
        return trainer_pic
    return trainer_class


def collect_rows() -> list[dict[str, object]]:
    lineup = load_lineup_module()
    rows = [row for row in lineup.collect_rows() if str(row["class"]) not in EXCLUDED_CLASSES]

    best_by_key: dict[tuple[str, str], dict[str, object]] = {}
    for row in rows:
        if row.get("label"):
            key = (display_label(row), str(row["gfx"]))
        else:
            key = (str(row["class"]), str(row["gfx"]))
        current = best_by_key.get(key)
        if current is None:
            best_by_key[key] = row
            continue

        current_count = int(current.get("count", 0))
        row_count = int(row.get("count", 0))
        if row_count > current_count:
            best_by_key[key] = row

    return sorted(best_by_key.values(), key=lambda row: (display_label(row), str(row["pic"]), str(row["gfx"])))


def color_sort_key(row: dict[str, object]) -> tuple[float, float, float]:
    r, g, b = row["rgb"]
    h, s, v = colorsys.rgb_to_hsv(r / 255, g / 255, b / 255)
    return (v, h, s)


def make_lineup_sheet(rows: list[dict[str, object]]) -> None:
    lineup = load_lineup_module()
    cols = 8
    cell_w = 118
    cell_h = 78
    pad = 16
    title_h = 50
    rows_n = math.ceil(len(rows) / cols)

    sheet = Image.new("RGBA", (cols * cell_w + pad * 2, rows_n * cell_h + title_h + pad), (246, 248, 250, 255))
    draw = ImageDraw.Draw(sheet)

    label_font = font(9, bold=True)
    detail_font = font(8)

    draw.text((pad, pad), "Trainer Overworld Lineup (Single Classes)", fill=(23, 32, 42), font=font(18, bold=True))
    draw.text((pad, pad + 25), f"{len(rows)} rows; paired double-battle classes excluded", fill=(91, 100, 112), font=detail_font)

    for i, row in enumerate(rows):
        x = pad + (i % cols) * cell_w
        y = title_h + (i // cols) * cell_h

        draw.rounded_rectangle((x, y, x + cell_w - 8, y + cell_h - 8), radius=4, fill=(255, 255, 255, 255), outline=(203, 210, 218, 255))

        sprite = lineup.load_sprite(str(row["gfx"]))
        frame = sprite.crop((0, 0, 16, 32)).resize((32, 64), Image.Resampling.NEAREST)
        sheet.alpha_composite(frame, (x + 6, y + 5))

        draw.text((x + 44, y + 8), display_label(row)[:16], fill=(23, 32, 42), font=label_font)
        draw.text((x + 44, y + 25), str(row["pic"])[:18], fill=(91, 100, 112), font=detail_font)
        draw.text((x + 44, y + 39), str(row["gfx"])[:18], fill=(91, 100, 112), font=detail_font)

    OUT_PNG.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUT_PNG)


def make_value_sheet(rows: list[dict[str, object]]) -> None:
    lineup = load_lineup_module()
    colors: dict[tuple[int, int, int], set[str]] = {}
    for row in rows:
        sprite = lineup.load_sprite(str(row["gfx"]))
        pixels = sprite.get_flattened_data() if hasattr(sprite, "get_flattened_data") else sprite.getdata()
        for r, g, b, a in pixels:
            if a:
                colors.setdefault((r, g, b), set()).add(str(row["gfx"]))

    color_rows = []
    for (r, g, b), gfx_names in colors.items():
        h, s, v = colorsys.rgb_to_hsv(r / 255, g / 255, b / 255)
        color_rows.append(
            {
                "hex": f"#{r:02X}{g:02X}{b:02X}",
                "rgb": [r, g, b],
                "hue": round(h * 360, 2),
                "saturation": round(s, 4),
                "value": round(v, 4),
                "overworlds": sorted(gfx_names),
            }
        )
    color_rows.sort(key=color_sort_key)
    OUT_VALUE_JSON.write_text(json.dumps(color_rows, indent=2) + "\n")

    cols = 10
    swatch = 54
    gap = 10
    label_h = 26
    cell_w = swatch + gap
    cell_h = swatch + label_h + gap
    pad = 18
    title_h = 62
    rows_n = math.ceil(len(color_rows) / cols)

    sheet = Image.new("RGB", (cols * cell_w + pad * 2 - gap, rows_n * cell_h + title_h + pad), "#f4f6f8")
    draw = ImageDraw.Draw(sheet)
    hex_font = font(9, bold=True)

    draw.text((pad, pad), "Trainer Overworld Colors By Value And Hue", fill="#17202a", font=font(22, bold=True))
    draw.text((pad, pad + 30), f"{len(color_rows)} unique visible colors from single-class trainer overworlds", fill="#5b6470", font=font(10))

    for i, row in enumerate(color_rows):
        x = pad + (i % cols) * cell_w
        y = title_h + (i // cols) * cell_h
        hex_value = str(row["hex"])

        draw.rounded_rectangle((x, y, x + swatch, y + swatch), radius=4, fill=tuple(row["rgb"]), outline="#c7ced8")
        text_w = draw.textbbox((0, 0), hex_value, font=hex_font)[2]
        draw.text((x + (swatch - text_w) // 2, y + swatch + 5), hex_value, fill="#17202a", font=hex_font)

    sheet.save(OUT_VALUE_PNG)
    print(f"wrote {OUT_PNG}")
    print(f"wrote {OUT_VALUE_PNG}")
    print(f"wrote {OUT_VALUE_JSON}")
    print(f"rows {len(rows)} colors {len(color_rows)}")


def main() -> None:
    rows = collect_rows()
    make_lineup_sheet(rows)
    make_value_sheet(rows)


if __name__ == "__main__":
    main()
