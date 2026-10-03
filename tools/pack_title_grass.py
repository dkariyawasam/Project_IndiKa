#!/usr/bin/env python3

import re
from pathlib import Path
import struct
import sys
import zlib


WIDTH = 240
HEIGHT = 144
TILE_OFFSET = 64
GRASS_PAL_NUM = 14
BLANK_TILE = 0xF03D
HEADER_COLOR_INDEX = 13
HEADER_BORDER_COLOR_INDEX = 14


def read_png_indices(path):
    with open(path, "rb") as f:
        data = f.read()

    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("not a PNG")

    pos = 8
    width = height = bit_depth = color_type = interlace = None
    idat = bytearray()

    while pos < len(data):
        length = struct.unpack(">I", data[pos:pos + 4])[0]
        chunk_type = data[pos + 4:pos + 8]
        chunk = data[pos + 8:pos + 8 + length]
        pos += 12 + length

        if chunk_type == b"IHDR":
            width, height, bit_depth, color_type, _, _, interlace = struct.unpack(">IIBBBBB", chunk)
        elif chunk_type == b"IDAT":
            idat.extend(chunk)
        elif chunk_type == b"IEND":
            break

    if (width, height) != (WIDTH, HEIGHT):
        raise ValueError(f"expected {WIDTH}x{HEIGHT}, got {width}x{height}")
    if bit_depth != 8 or color_type != 3 or interlace != 0:
        raise ValueError("expected an 8-bit indexed, non-interlaced PNG")

    raw = zlib.decompress(bytes(idat))
    stride = width
    rows = []
    prev = [0] * stride
    p = 0

    for _ in range(height):
        filter_type = raw[p]
        p += 1
        row = list(raw[p:p + stride])
        p += stride

        for x in range(stride):
            left = row[x - 1] if x else 0
            up = prev[x]
            up_left = prev[x - 1] if x else 0

            if filter_type == 1:
                row[x] = (row[x] + left) & 0xFF
            elif filter_type == 2:
                row[x] = (row[x] + up) & 0xFF
            elif filter_type == 3:
                row[x] = (row[x] + ((left + up) >> 1)) & 0xFF
            elif filter_type == 4:
                predictor = left + up - up_left
                pa = abs(predictor - left)
                pb = abs(predictor - up)
                pc = abs(predictor - up_left)
                if pa <= pb and pa <= pc:
                    predict = left
                elif pb <= pc:
                    predict = up
                else:
                    predict = up_left
                row[x] = (row[x] + predict) & 0xFF
            elif filter_type != 0:
                raise ValueError(f"unsupported PNG filter {filter_type}")

        rows.append(row)
        prev = row

    return rows


def read_u16_map(path):
    with open(path, "rb") as f:
        return bytearray(f.read())


def write_u16(buf, index, value):
    struct.pack_into("<H", buf, index * 2, value)


def read_u16(buf, index):
    return struct.unpack_from("<H", buf, index * 2)[0]


def make_tile(rows, tx, ty):
    tile = bytearray(32)
    for y in range(8):
        for x in range(0, 8, 2):
            left = rows[ty * 8 + y][tx * 8 + x]
            right = rows[ty * 8 + y][tx * 8 + x + 1]
            if left > 15 or right > 15:
                raise ValueError("grass PNG uses palette indexes above 15")
            tile[y * 4 + (x // 2)] = left | (right << 4)
    return bytes(tile)


def make_footer():
    # Match the original 12px strip, exposing the BG3 grass beneath it.
    rows = [[12] * WIDTH for _ in range(12)] + [[0] * WIDTH for _ in range(4)]
    rows[0] = rows[11] = [13] * WIDTH

    root = Path(__file__).resolve().parent.parent
    charmap = dict((char, int(code, 16)) for char, code in
                   re.findall(r"^'(.)'\s*=\s*([0-9A-F]{2})$",
                              (root / "charmap.txt").read_text(), re.M))
    font = (root / "graphics/fonts/latin_small.latfont").read_bytes()
    glyphs = []
    for char in "MOD BY DEE KARIYAWASAM - 2026":
        if char == " ":
            glyphs.append((3, []))
            continue
        data = struct.unpack_from("<16H", font, charmap[char] * 32)
        pixels = [(x, y - 4, (data[y] >> (14 - x * 2)) & 3)
                  for y in range(4, 12) for x in range(8)
                  if ((data[y] >> (14 - x * 2)) & 3) in (1, 2)]
        width = max(x for x, _, _ in pixels) + 1
        glyphs.append((width + 1, pixels))
    text_width = sum(width for width, _ in glyphs) - 1
    if text_width > WIDTH - 16:
        raise ValueError("mod credit is too wide for the title footer")
    cursor = (WIDTH - text_width) // 2
    for width, pixels in glyphs:
        for x, y, color in pixels:
            rows[2 + y][cursor + x] = 10 if color == 1 else 6
        cursor += width
    return rows


def main():
    if len(sys.argv) != 5:
        raise SystemExit("usage: pack_title_grass.py grass.png copyright_press_start.bin grass.4bpp grass.bin")

    png_path, copyright_map_path, tiles_path, map_path = sys.argv[1:]
    rows = read_png_indices(png_path)
    copyright_map = read_u16_map(copyright_map_path)

    tile_to_index = {}
    tiles = []
    tilemap = bytearray(32 * 20 * 2)

    for i in range(32 * 20):
        write_u16(tilemap, i, BLANK_TILE)

    for ty in range(HEIGHT // 8):
        for tx in range(WIDTH // 8):
            tile = make_tile(rows, tx, ty)
            if tile not in tile_to_index:
                tile_to_index[tile] = len(tiles)
                tiles.append(tile)
            tile_id = tile_to_index[tile]
            write_u16(tilemap, ty * 32 + tx, (GRASS_PAL_NUM << 12) | (TILE_OFFSET + tile_id))

    header_tile = bytearray([HEADER_COLOR_INDEX | (HEADER_COLOR_INDEX << 4)] * 32)
    for x in range(4):
        header_tile[7 * 4 + x] = HEADER_BORDER_COLOR_INDEX | (HEADER_BORDER_COLOR_INDEX << 4)
    header_tile = bytes(header_tile)
    if header_tile not in tile_to_index:
        tile_to_index[header_tile] = len(tiles)
        tiles.append(header_tile)
    header_tile_id = tile_to_index[header_tile]
    for col in range(WIDTH // 8):
        write_u16(tilemap, col, (GRASS_PAL_NUM << 12) | (TILE_OFFSET + header_tile_id))

    # Overlay only the visible PRESS START tiles so the grass row remains intact.
    for col in range(32):
        index = 16 * 32 + col
        entry = read_u16(copyright_map, index)
        tile = entry & 0x3FF
        if tile != 0 and tile != 0x3D:
            write_u16(tilemap, index, entry)

    # The original credits remain on the intro splash; the title footer
    # identifies the mod. Keep its tiles outside the PRESS START blink range.
    footer = make_footer()
    for ty in range(2):
        for tx in range(WIDTH // 8):
            tile = make_tile(footer, tx, ty)
            if tile not in tile_to_index:
                tile_to_index[tile] = len(tiles)
                tiles.append(tile)
            write_u16(tilemap, (18 + ty) * 32 + tx,
                      (15 << 12) | (TILE_OFFSET + tile_to_index[tile]))
    if TILE_OFFSET + len(tiles) > 512:
        raise ValueError("title foreground exceeds its character block")

    with open(tiles_path, "wb") as f:
        f.write(b"".join(tiles))
    with open(map_path, "wb") as f:
        f.write(tilemap)


if __name__ == "__main__":
    main()
