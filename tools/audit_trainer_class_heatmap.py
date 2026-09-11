#!/usr/bin/env python3
"""Audit script-reachable trainer encounter sites, including aliases and branches."""
from __future__ import annotations

import json
import re
from collections import Counter, defaultdict
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs"


def definitions(path):
    return dict(re.findall(r"^#define\s+(\w+)\s+(\w+)\b", path.read_text(), re.M))


def resolve(name, defs):
    seen = set()
    while name in defs and name not in seen:
        seen.add(name)
        name = defs[name]
    return int(name) if name.isdecimal() else name


def pretty(value):
    value = re.sub(r"([a-z])([A-Z])", r"\1 \2", value)
    return value.replace("_", " ").title().replace("Pkmn", "Pokémon").replace("Rs ", "")


def audit():
    ids = definitions(ROOT / "include/constants/opponents.h")
    class_defs = definitions(ROOT / "include/constants/trainers.h")
    class_names = dict(re.findall(r'\[(TRAINER_CLASS_\w+)\]\s*=\s*_\("([^"]+)"\)',
                                 (ROOT / "src/data/text/trainer_class_names.h").read_text()))
    classes = {}
    for token, label in class_names.items():
        if token.endswith("NONE") or "UNUSED" in token:
            continue
        suffix = token.removeprefix("TRAINER_CLASS_")
        label = label.replace("{PKMN}", "Pokémon").title()
        if suffix.endswith(("_M", "_F")):
            label += " " + suffix[-1]
        if label in {"Rival", "Champion", "Elite Four", "Leader"}:
            label = pretty(suffix)
        classes[resolve(token, class_defs)] = {"id": token, "label": label}

    split_labels = {"Ace Trainer", "Black Belt", "Expert", "Pokéfan", "Pokémon Breeder", "Pokémon Ranger",
                    "Psychic", "Scout", "Swimmer", "Team Rocket", "Rocket Ace", "Tuber", "Youngster", "Trendsetter", "Triathlete"}
    display_rows = {}
    def row_for(base, gender=None):
        label = base + (" " + gender if gender else "")
        key = "CLASS_" + re.sub(r"\W+", "_", label.upper())
        display_rows[key] = {"id": key, "label": label}
        return key
    for c in classes.values():
        base = re.sub(r" [MF]$", "", c["label"])
        if base in split_labels:
            row_for(base, "M")
            row_for(base, "F")
        else:
            row_for(base)

    trainers = {}
    overwritten = []
    text = (ROOT / "src/data/trainers.h").read_text()
    for match in re.finditer(r"^\s*\[(TRAINER_\w+)\]\s*=\s*\{(.*?)^    \},", text, re.M | re.S):
        token, block = match.groups()
        cid = re.search(r"\.trainerClass\s*=\s*(\w+)", block)
        name = re.search(r'\.trainerName\s*=\s*_\("([^"]*)"\)', block)
        if not cid or not name:
            continue
        number = resolve(token, ids)
        if number in trainers:
            overwritten.append([trainers[number]["token"], token])
        class_id = resolve(cid[1], class_defs)
        if class_id not in classes:
            continue
        base = re.sub(r" [MF]$", "", classes[class_id]["label"])
        pic = re.search(r"\.trainerPic\s*=\s*(\w+)", block)
        gender_match = re.search(r"_([MF])(?:_|$)", pic[1]) if pic else None
        gender = gender_match[1] if gender_match else ("F" if "F_TRAINER_FEMALE" in block else "M")
        trainers[number] = {"token": token, "name": name[1], "class": row_for(base, gender if base in split_labels else None),
                            "encoded_class": cid[1], "double": ".doubleBattle = TRUE" in block}

    # Only parse assembly sources included by the compiled event script unit.
    files = set()
    def include(path):
        if path in files or not path.exists():
            return
        files.add(path)
        for rel in re.findall(r'\.include\s+"([^"]+)"', path.read_text()):
            include(ROOT / rel)
    include(ROOT / "data/event_scripts.s")
    blocks, sources = {}, {}
    for path in sorted(files):
        current = None
        for line_no, line in enumerate(path.read_text().splitlines(), 1):
            match = re.match(r"^(\w+)::?\s*(?:@.*)?$", line)
            if match:
                current = match[1]
                blocks[current] = []
                sources[current] = f"{path.relative_to(ROOT)}:{line_no}"
            elif current:
                blocks[current].append(line.strip())
    battles, edges, dynamic = defaultdict(set), defaultdict(set), {}
    for label, lines in blocks.items():
        for line in lines:
            line = line.split("@", 1)[0].strip()
            op = line.split()[0] if line else ""
            if op.startswith("trainerbattle") and "rematch" not in op:
                tokens = re.findall(r"\bTRAINER_[A-Z0-9_]+\b", line)
                hits = [resolve(t, ids) for t in tokens if t in ids]
                if hits:
                    battles[label].update(hits)
                else:
                    dynamic[label] = line
            if op.startswith(("goto", "call", "map_script")):
                edges[label].update(t for t in re.findall(r"\b\w+\b", line) if t in blocks)
            # Battle continuation scripts may lead to another story battle.
            if op.startswith("trainerbattle") and "rematch" not in op:
                edges[label].update(t for t in re.findall(r"\b\w+\b", line) if t in blocks)

    def walk(label):
        seen, todo, found, unresolved = set(), [label], set(), set()
        while todo:
            here = todo.pop()
            if here in seen:
                continue
            seen.add(here)
            found.update(battles[here])
            if here in dynamic:
                unresolved.add(here)
            todo.extend(edges[here] - seen)
        return found, unresolved

    groups = json.loads((ROOT / "data/maps/map_groups.json").read_text())
    active_maps = {m for group in groups["group_order"] for m in groups[group]}
    encounters, unresolved, missing, all_areas = [], [], [], set()
    for map_name in sorted(active_maps):
        path = ROOT / "data/maps" / map_name / "map.json"
        if not path.exists():
            continue
        data = json.loads(path.read_text())
        area = pretty(data["region_map_section"].removeprefix("MAPSEC_"))
        if map_name.startswith("SSAnne"):
            area = "S.S. Anne"
        elif map_name.startswith("SilphCo"):
            area = "Silph Co."
        elif map_name.startswith("PokemonLeague"):
            area = "Pokémon League"
        elif map_name.startswith("RocketLeague"):
            area = "Rocket League"
        area = area.replace("Pokemon", "Pokémon").replace("Mt Moon", "Mt. Moon")
        all_areas.add(area)
        roots = []
        for i, obj in enumerate(data.get("object_events", []), 1):
            roots.append((obj.get("script", "NULL"), f"object {i} ({obj['x']},{obj['y']})"))
        for kind in ("coord_events", "bg_events"):
            for event in data.get(kind, []):
                if event.get("script"):
                    roots.append((event["script"], f"{kind} ({event['x']},{event['y']})"))
        roots.append((map_name + "_MapScripts", "map callback"))
        candidates = []
        for label, origin in roots:
            if label in {"NULL", "0", "0x0"}:
                continue
            if label not in blocks:
                missing.append({"map": map_name, "script": label, "origin": origin})
                continue
            found, unknown = walk(label)
            unresolved.extend({"map": map_name, "root": label, "script": u, "command": dynamic[u]} for u in unknown)
            by_class = defaultdict(set)
            for tid in found:
                if tid in trainers:
                    by_class[trainers[tid]["class"]].add(tid)
            for cid, tids in by_class.items():
                candidates.append({"class": cid, "ids": tids, "roots": {label}, "origins": {origin}})
        # Merge two halves of a double battle and repeated triggers for one fight.
        merged = []
        for candidate in candidates:
            while True:
                match = next((e for e in merged if e["class"] == candidate["class"] and e["ids"] & candidate["ids"]), None)
                if match is None:
                    break
                merged.remove(match)
                for field in ("ids", "roots", "origins"):
                    candidate[field].update(match[field])
            merged.append(candidate)
        for e in merged:
            encounters.append({"class": e["class"], "area": area, "map": map_name,
                               "names": sorted({trainers[t]["name"] for t in e["ids"]}),
                               "trainer_ids": sorted(e["ids"]),
                               "variants": sorted(trainers[t]["token"] for t in e["ids"]),
                               "encoded_classes": sorted({trainers[t]["encoded_class"] for t in e["ids"]}),
                               "roots": sorted(e["roots"]), "origins": sorted(e["origins"]),
                               "sources": sorted({sources[r] for r in e["roots"]}),
                               "conditional": len(e["ids"]) > 1})

    def area_sort(name):
        m = re.match(r"Route\s*(\d+)(.*)", name)
        return (0, int(m[1]), m[2]) if m else (1, 0, name)
    areas = sorted({e["area"] for e in encounters}, key=area_sort)
    rows = sorted(display_rows.values(), key=lambda c: (c["label"], c["id"]))
    matrix = [[sum(e["class"] == c["id"] and e["area"] == a for e in encounters) for a in areas] for c in rows]
    repeated = defaultdict(set)
    for e in encounters:
        for tid in e["trainer_ids"]:
            repeated[tid].add(e["map"])
    return {"classes": rows, "areas": areas, "matrix": matrix, "encounters": encounters,
            "missing_scripts": missing, "dynamic_battles": unresolved, "overwritten_entries": overwritten,
            "areas_without_battles": sorted(all_areas - set(areas)),
            "shared_flags_across_maps": [{"id": tid, "trainer": trainers[tid]["token"], "maps": sorted(maps)} for tid, maps in repeated.items() if len(maps) > 1],
            "summary": {"maps": len(active_maps), "classes": len(rows), "areas": len(areas), "encounter_sites": len(encounters),
                        "unplaced_classes": sum(not sum(row) for row in matrix), "conditional_sites": sum(e["conditional"] for e in encounters)}}


def render_png(data):
    cw, rh, left, top = 37, 27, 240, 320
    w, h = left + cw * len(data["areas"]) + 95, top + rh * len(data["classes"]) + 60
    im = Image.new("RGB", (w, h), "white")
    draw = ImageDraw.Draw(im)
    font_path = "/System/Library/Fonts/Supplemental/Arial.ttf"
    f = ImageFont.truetype(font_path, 14)
    title = ImageFont.truetype(font_path, 24)
    draw.text((18, 16), "Trainer class encounters by area", font=title, fill="#18251d")
    draw.text((18, 50), "Script-reachable encounter sites; doubles count once; conditional rosters share a site; rematches excluded.", font=f, fill="#34483c")
    peak = max(max(row, default=0) for row in data["matrix"])
    draw.text((18, 76), f"{len(data['encounters'])} sites | {len(data['classes'])} classes | {len(data['areas'])} areas | cell range 0–{peak}", font=f, fill="#34483c")
    for j, area in enumerate(data["areas"]):
        label = Image.new("RGBA", (190, 24))
        ImageDraw.Draw(label).text((0, 1), area, font=f, fill="#18251d")
        label = label.rotate(90, expand=True)
        im.paste(label, (left + j * cw + 7, top - 195), label)
    for i, cls in enumerate(data["classes"]):
        y = top + i * rh
        draw.text((10, y + 5), cls["label"], font=f, fill="#18251d")
        draw.text((left + len(data["areas"]) * cw + 14, y + 5), str(sum(data["matrix"][i])), font=f, fill="#18251d")
        for j, count in enumerate(data["matrix"][i]):
            x = left + j * cw
            t = (count / peak) ** 0.55 if count else 0
            color = tuple(round(a + (b - a) * t) for a, b in zip((246, 249, 245), (24, 112, 63)))
            draw.rectangle((x, y, x + cw - 2, y + rh - 2), fill=color)
            if count:
                draw.text((x + cw / 2, y + 5), str(count), anchor="mt", font=f, fill="white" if t > .55 else "#18251d")
    im.save(OUT / "trainer-class-heatmap.png")


def main():
    data = audit()
    (OUT / "trainer-class-heatmap.json").write_text(json.dumps(data, indent=2) + "\n")
    lines = ["# Trainer class placement audit", "", "Counts represent script-reachable encounter sites in compiled maps, grouped by the project's region-map sections (with the S.S. Anne, Silph Co. and Pokémon/Rocket Leagues separated). Map objects, coordinate/background triggers and map callbacks are traversed through call/goto branches.", "", "Trainer aliases are resolved. Double-battle partners sharing a fight count once. Badge/starter/story-dependent rosters at the same trigger count as one site per class/variant. Rematch commands are excluded. Conditional appearances are potential encounters, not a claim that every branch is available simultaneously. Script commands without a literal trainer ID are reported separately; battles selected solely in C/procedural facilities are outside this static script audit.", "", "Rows follow the displayed trainer class, with M/F battle-sprite variants separated for shared classes. Sprite gender falls back to the female trainer flag where the picture name has no gender suffix. Original encoded class IDs remain in each encounter record. Duplicate legacy class labels are merged. Zero rows include declared classes with no script-reachable encounter. Only areas containing an encounter appear in the heatmap; empty areas are listed in the JSON.", "", "| Class | Sites | Areas |", "| --- | ---: | --- |"]
    for cls, row in zip(data["classes"], data["matrix"]):
        lines.append(f"| {cls['label']} | {sum(row)} | " + ", ".join(f"{a} ({n})" for a, n in zip(data["areas"], row) if n) + " |")
    lines += ["", "## Coverage", "", json.dumps(data["summary"]), "", f"Unresolved map-root scripts: {len(data['missing_scripts'])}. Dynamic battle references: {len(data['dynamic_battles'])}. Duplicate trainer-table numeric entries: {len(data['overwritten_entries'])} (last definition wins, matching C).", "", "Exact maps, trainer names, variant IDs, source locations and shared flags are retained in trainer-class-heatmap.json.", ""]
    (OUT / "trainer-class-heatmap.md").write_text("\n".join(lines))
    render_png(data)
    print(json.dumps(data["summary"]))
    print("missing", data["missing_scripts"])
    print("dynamic", data["dynamic_battles"])
    print("unplaced", [c["label"] for c, row in zip(data["classes"], data["matrix"]) if not sum(row)])


if __name__ == "__main__":
    main()
