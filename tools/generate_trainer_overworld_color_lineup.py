#!/usr/bin/env python3
"""Generate a hue-sorted swatch sheet of colors used by trainer overworlds."""

from __future__ import annotations

import colorsys
import importlib.util
import json
import math
from collections import defaultdict
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
LINEUP_SCRIPT = ROOT / "tools/generate_trainer_overworld_lineup.py"
OUT_PNG = ROOT / "docs/trainer-overworld-colors.png"
OUT_JSON = ROOT / "docs/trainer-overworld-colors.json"


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


def color_sort_key(color: tuple[int, int, int]) -> tuple[float, float, float]:
    r, g, b = color
    h, s, v = colorsys.rgb_to_hsv(r / 255, g / 255, b / 255)
    if s < 0.08:
        return (1.0 + v, 0.0, v)
    return (h, s, v)


def color_hex(color: tuple[int, int, int]) -> str:
    return "#{:02X}{:02X}{:02X}".format(*color)


def collect_colors() -> list[dict[str, object]]:
    lineup = load_lineup_module()
    colors: dict[tuple[int, int, int], set[str]] = defaultdict(set)

    for row in lineup.collect_rows():
        sprite = lineup.load_sprite(str(row["gfx"]))
        pixels = sprite.get_flattened_data() if hasattr(sprite, "get_flattened_data") else sprite.getdata()
        for r, g, b, a in pixels:
            if a == 0:
                continue
            colors[(r, g, b)].add(str(row["gfx"]))

    output = []
    for color, gfx_names in colors.items():
        h, s, v = colorsys.rgb_to_hsv(color[0] / 255, color[1] / 255, color[2] / 255)
        output.append(
            {
                "hex": color_hex(color),
                "rgb": list(color),
                "hue": round(h * 360, 2),
                "saturation": round(s, 4),
                "value": round(v, 4),
                "overworlds": sorted(gfx_names),
            }
        )

    return sorted(output, key=lambda row: color_sort_key(tuple(row["rgb"])))


def make_sheet(rows: list[dict[str, object]]) -> None:
    cols = 10
    swatch = 54
    gap = 10
    label_h = 26
    cell_w = swatch + gap
    cell_h = swatch + label_h + gap
    pad = 18
    title_h = 62
    rows_n = math.ceil(len(rows) / cols)

    sheet = Image.new("RGB", (cols * cell_w + pad * 2 - gap, rows_n * cell_h + title_h + pad), "#f4f6f8")
    draw = ImageDraw.Draw(sheet)

    title_font = font(22, bold=True)
    detail_font = font(10)
    hex_font = font(9, bold=True)

    draw.text((pad, pad), "Trainer Overworld Colors By Hue", fill="#17202a", font=title_font)
    draw.text((pad, pad + 30), f"{len(rows)} unique visible colors from trainer overworld front sprites", fill="#5b6470", font=detail_font)

    for i, row in enumerate(rows):
        col = i % cols
        row_i = i // cols
        x = pad + col * cell_w
        y = title_h + row_i * cell_h
        color = tuple(row["rgb"])
        hex_value = str(row["hex"])

        draw.rounded_rectangle((x, y, x + swatch, y + swatch), radius=4, fill=color, outline="#c7ced8")
        text_w = draw.textbbox((0, 0), hex_value, font=hex_font)[2]
        draw.text((x + (swatch - text_w) // 2, y + swatch + 5), hex_value, fill="#17202a", font=hex_font)

    OUT_PNG.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUT_PNG)


def main() -> None:
    rows = collect_colors()
    OUT_JSON.write_text(json.dumps(rows, indent=2) + "\n")
    make_sheet(rows)
    print(f"wrote {OUT_PNG}")
    print(f"wrote {OUT_JSON}")


if __name__ == "__main__":
    main()
