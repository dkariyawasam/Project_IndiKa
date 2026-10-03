#!/usr/bin/env python3
"""Check imported cry mappings, PCM assets, and compiled normal/reverse tables."""
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import tempfile
import wave
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools/debug_dashboard'))
from catalog import load_symbols


def main():
    records = []
    for manifest, prefix in [('cry-imports.json', 'Cry_Imported_'), ('custom-cries.json', 'Cry_Custom_')]:
        if (ROOT / 'docs' / manifest).exists():
            for record in json.loads((ROOT / 'docs' / manifest).read_text())['cries']:
                records.append(dict(record, symbol=prefix + record['species']))
    source = (ROOT / 'src/pokemon.c').read_text()
    function = source[source.index('u16 SpeciesToCryId(u16 species)'):]
    function = function[:function.index('\n}\n') + 3]
    assertions = '\n'.join(f'assert(SpeciesToCryId(SPECIES_{r["species"]} - 1) == {r["cry_id"]});' for r in records)
    code = '''#include <assert.h>
#include "constants/species.h"
#include "constants/hoenn_cries.h"
typedef unsigned short u16;
#include "src/data/pokemon/cry_ids.h"
''' + function + '\nint main(void) {\n' + assertions + '\nreturn 0; }\n'
    with tempfile.TemporaryDirectory() as folder:
        cfile = Path(folder) / 'cries.c'
        binary = Path(folder) / 'cries'
        cfile.write_text(code)
        subprocess.run(['cc', '-I', str(ROOT / 'include'), '-I', str(ROOT), str(cfile), '-o', str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
    rom = (ROOT / 'pokefirered.gba').read_bytes()
    symbols = load_symbols(ROOT / 'pokefirered.elf')
    count = len(re.findall(r'^\tcry ', (ROOT / 'sound/cry_tables.inc').read_text(), re.M))
    assert count <= 512
    for record in records:
        path = ROOT / record['wav']
        assert hashlib.sha256(path.read_bytes()).hexdigest() == record['wav_sha256']
        with wave.open(str(path)) as wav:
            assert (wav.getnchannels(), wav.getsampwidth(), wav.getframerate()) == (1, 1, 10512)
            pcm = wav.readframes(wav.getnframes())
            assert 0 < len(pcm) < 10512 * 3 and max(pcm) - min(pcm) > 10
        sample = symbols[record['symbol']]
        packed = path.with_suffix('.bin').read_bytes()
        offset = sample - 0x08000000
        assert rom[offset:offset + len(packed)] == packed
        for table, tone in [('gCryTable', 0x20), ('gCryTable_Reverse', 0x30)]:
            offset = symbols[table] - 0x08000000 + record['cry_id'] * 12
            assert rom[offset] == tone
            assert struct.unpack_from('<I', rom, offset + 4)[0] == sample
    print(f'{len(records)} cries: species mappings, WAV format, ROM samples and both tone tables passed.')


if __name__ == '__main__':
    main()
