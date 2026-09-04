#!/usr/bin/env python3
"""Render trainer front pics next to their overworld front sprites."""

from __future__ import annotations

import importlib.util
import math
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
FRONT_PICS_DIR = ROOT / "graphics/trainers/front_pics"
LINEUP_SCRIPT = ROOT / "tools/generate_trainer_overworld_lineup.py"
OUT_PNG = ROOT / "docs/trainer-front-overworld-comparison.png"


PIC_STEMS = {
    "BLACK_BELT": "black_belt_m",
    "COOLTRAINER_M": "ace_trainer_m",
    "COOLTRAINER_F": "ace_trainer_f",
    "PKMN_BREEDER": "pokemon_breeder",
    "POKEMON_BREEDER": "pokemon_breeder",
    "POKEMANIAC": "pokemaniac",
    "SAILOR": "sailor_rs",
    "TRAINER_MANIAC": "pokemaniac",
}


def load_lineup_module():
    spec = importlib.util.spec_from_file_location("trainer_overworld_lineup", LINEUP_SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Could not load {LINEUP_SCRIPT}")

    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def load_rgba(path: Path) -> Image.Image:
    source = Image.open(path)
    if source.mode == "P":
        alpha = Image.new("L", source.size, 255)
        pixels = source.get_flattened_data() if hasattr(source, "get_flattened_data") else source.getdata()
        alpha.putdata([0 if px == 0 else 255 for px in pixels])
        image = source.convert("RGBA")
        image.putalpha(alpha)
        return image

    return source.convert("RGBA")


def front_pic_path(pic: str) -> Path:
    stem = PIC_STEMS.get(pic, pic.lower())
    return FRONT_PICS_DIR / f"{stem}_front_pic.png"


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


def wrap(draw: ImageDraw.ImageDraw, text: str, text_font: ImageFont.ImageFont, width: int) -> list[str]:
    words = text.split("_")
    lines: list[str] = []
    current = ""
    for word in words:
        trial = word if not current else f"{current}_{word}"
        if draw.textbbox((0, 0), trial, font=text_font)[2] <= width or not current:
            current = trial
        else:
            lines.append(current)
            current = word
    if current:
        lines.append(current)
    return lines


def make_sheet() -> None:
    lineup = load_lineup_module()
    rows = lineup.collect_rows()

    cols = 3
    cell_w = 338
    cell_h = 196
    pad = 18
    title_h = 62
    rows_n = math.ceil(len(rows) / cols)

    sheet = Image.new("RGB", (cols * cell_w + pad * 2, rows_n * cell_h + title_h + pad), "#f4f6f8")
    draw = ImageDraw.Draw(sheet)

    title_font = font(22, bold=True)
    label_font = font(13, bold=True)
    detail_font = font(10)

    draw.text((pad, pad), "Trainer Front Pic / Overworld Comparison", fill="#17202a", font=title_font)
    draw.text((pad, pad + 30), f"{len(rows)} placed or defined class / overworld variants", fill="#5b6470", font=detail_font)

    missing: list[str] = []
    for i, row in enumerate(rows):
        col = i % cols
        row_i = i // cols
        x = pad + col * cell_w
        y = title_h + row_i * cell_h

        draw.rounded_rectangle((x + 4, y + 4, x + cell_w - 8, y + cell_h - 8), radius=8, fill="#ffffff", outline="#d8dde5")

        front_path = front_pic_path(str(row["pic"]))
        if front_path.exists():
            front = load_rgba(front_path)
            front.thumbnail((128, 128), Image.Resampling.NEAREST)
            fx = x + 54 - front.width // 2
            fy = y + 20 + (128 - front.height) // 2
            sheet.paste(front, (fx, fy), front)
        else:
            missing.append(f"{row['class']} / {row['pic']}")
            draw.rectangle((x + 12, y + 20, x + 136, y + 148), outline="#c03737")
            draw.text((x + 22, y + 74), "missing\nfront", fill="#a12828", font=detail_font)

        overworld = lineup.load_sprite(str(row["gfx"]))
        overworld = overworld.resize((overworld.width * 4, overworld.height * 4), Image.Resampling.NEAREST)
        ox = x + 225 - overworld.width // 2
        oy = y + 20 + (128 - overworld.height) // 2
        sheet.paste(overworld, (ox, oy), overworld)

        draw.text((x + 31, y + 148), "FRONT", fill="#7a8491", font=detail_font)
        draw.text((x + 199, y + 148), "OVERWORLD", fill="#7a8491", font=detail_font)

        label_y = y + 163
        for line in wrap(draw, str(row["class"]), label_font, cell_w - 24)[:1]:
            text_w = draw.textbbox((0, 0), line, font=label_font)[2]
            draw.text((x + (cell_w - text_w) // 2, label_y), line, fill="#111827", font=label_font)

        details = f"pic: {row['pic']}  gfx: {row['gfx']}"
        text_w = draw.textbbox((0, 0), details, font=detail_font)[2]
        draw.text((x + (cell_w - text_w) // 2, label_y + 16), details, fill="#596273", font=detail_font)

    OUT_PNG.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(OUT_PNG)

    if missing:
        print("missing front PNGs:")
        for item in missing:
            print(f"- {item}")
    print(f"wrote {OUT_PNG}")


if __name__ == "__main__":
    make_sheet()
