"""Keep Pallet's full metatile set in Route 21's shared physical tile slots."""
from pathlib import Path
import struct
root = Path(__file__).resolve().parents[1]
path = root / 'data/tilesets/secondary/pallet_town'
raw = (path / 'metatiles.bin').read_bytes()
tiles = struct.unpack('<' + 'H' * (len(raw) // 2), raw)
shifted = [(tile & ~1023) | ((tile & 1023) + (128 if (tile & 1023) >= 640 else 0)) for tile in tiles]
(path / 'connected_metatiles.bin').write_bytes(struct.pack('<' + 'H' * len(shifted), *shifted))
