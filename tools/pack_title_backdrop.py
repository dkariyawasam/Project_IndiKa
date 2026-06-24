#!/usr/bin/env python3

import struct
import sys
import zlib


DISPLAY_WIDTH = 240
DISPLAY_HEIGHT = 160
MAP_WIDTH = 32
MAP_HEIGHT = 20
BACKDROP_FIRST_ROW = 1
BACKDROP_ROWS = 17
BG3_TILE_OFFSET = 144
BACKDROP_TILE_OFFSET = BG3_TILE_OFFSET + 64
BACKDROP_PAL_NUM = 4
MAX_BACKDROP_TILES = 512 - BACKDROP_TILE_OFFSET


def read_png_rgba(path):
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

    if width < DISPLAY_WIDTH or height != DISPLAY_HEIGHT:
        raise ValueError(f"expected at least {DISPLAY_WIDTH}x{DISPLAY_HEIGHT}, got {width}x{height}")
    if bit_depth != 8 or color_type not in (2, 6) or interlace != 0:
        raise ValueError("expected an 8-bit RGB/RGBA, non-interlaced PNG")

    bytes_per_pixel = 4 if color_type == 6 else 3
    raw = zlib.decompress(bytes(idat))
    rows = []
    prev = [0] * (width * bytes_per_pixel)
    p = 0

    for _ in range(height):
        filter_type = raw[p]
        p += 1
        row = list(raw[p:p + width * bytes_per_pixel])
        p += width * bytes_per_pixel

        for x in range(len(row)):
            left = row[x - bytes_per_pixel] if x >= bytes_per_pixel else 0
            up = prev[x]
            up_left = prev[x - bytes_per_pixel] if x >= bytes_per_pixel else 0

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

        pixels = []
        for x in range(DISPLAY_WIDTH):
            i = x * bytes_per_pixel
            alpha = row[i + 3] if bytes_per_pixel == 4 else 255
            pixels.append((row[i], row[i + 1], row[i + 2], alpha))
        rows.append(pixels)
        prev = row

    return rows


def write_u16(buf, index, value):
    struct.pack_into("<H", buf, index * 2, value)


def gba_color(rgb):
    r, g, b = rgb
    return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10)


def make_tile(index_rows, tx, ty):
    tile = bytearray(32)
    for y in range(8):
        for x in range(0, 8, 2):
            left = index_rows[ty * 8 + y][tx * 8 + x]
            right = index_rows[ty * 8 + y][tx * 8 + x + 1]
            tile[y * 4 + (x // 2)] = left | (right << 4)
    return bytes(tile)


def tile_distance(left, right):
    return sum(abs((a & 0xF) - (b & 0xF)) + abs((a >> 4) - (b >> 4)) for a, b in zip(left, right))


def best_tile_id(tile, tiles):
    best_id = 0
    best_distance = None
    for i, existing in enumerate(tiles):
        distance = tile_distance(tile, existing)
        if best_distance is None or distance < best_distance:
            best_id = i
            best_distance = distance
            if distance == 0:
                break
    return best_id


def main():
    if len(sys.argv) != 6:
        raise SystemExit("usage: pack_title_backdrop.py backdrop.png border_bg.bin backdrop.4bpp backdrop.bin backdrop.gbapal")

    png_path, base_map_path, tiles_path, map_path, pal_path = sys.argv[1:]
    rows = read_png_rgba(png_path)

    colors = []
    color_to_index = {}
    index_rows = []
    for y in range(DISPLAY_HEIGHT):
        index_row = []
        for x in range(DISPLAY_WIDTH):
            r, g, b, alpha = rows[y][x]
            if alpha == 0:
                index_row.append(0)
                continue
            color = (r, g, b)
            if color not in color_to_index:
                if len(colors) >= 15:
                    raise ValueError("backdrop uses more than 15 visible colors")
                colors.append(color)
                color_to_index[color] = len(colors)
            index_row.append(color_to_index[color])
        index_rows.append(index_row)

    with open(base_map_path, "rb") as f:
        tilemap = bytearray(f.read())

    for i in range(MAP_WIDTH * MAP_HEIGHT):
        entry = struct.unpack_from("<H", tilemap, i * 2)[0]
        tile_id = entry & 0x3FF
        if tile_id < 64:
            write_u16(tilemap, i, (entry & ~0x3FF) | (BG3_TILE_OFFSET + tile_id))

    tile_to_index = {}
    tiles = []
    for ty in range(BACKDROP_ROWS):
        source_ty = ty + BACKDROP_FIRST_ROW
        for tx in range(DISPLAY_WIDTH // 8):
            tile = make_tile(index_rows, tx, source_ty)
            if tile not in tile_to_index:
                if len(tiles) < MAX_BACKDROP_TILES:
                    tile_to_index[tile] = len(tiles)
                    tiles.append(tile)
                else:
                    tile_to_index[tile] = best_tile_id(tile, tiles)
            tile_id = tile_to_index[tile]
            write_u16(
                tilemap,
                (BACKDROP_FIRST_ROW + ty) * MAP_WIDTH + tx,
                (BACKDROP_PAL_NUM << 12) | (BACKDROP_TILE_OFFSET + tile_id),
            )

    palette = [0] + [gba_color(color) for color in colors]
    palette.extend([0] * (16 - len(palette)))

    with open(tiles_path, "wb") as f:
        f.write(b"".join(tiles))
    with open(map_path, "wb") as f:
        f.write(tilemap)
    with open(pal_path, "wb") as f:
        for color in palette:
            f.write(struct.pack("<H", color))


if __name__ == "__main__":
    main()
