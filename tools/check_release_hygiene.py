#!/usr/bin/env python3
"""Check the Git index for full ROMs and private emulator data; not a rights audit."""
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
ALLOWED = {'data/mb_colosseum.gba'}  # Documented upstream build dependency.
PRIVATE = {'.gba', '.gb', '.gbc', '.nds', '.sav', '.srm', '.sgm', '.state', '.elf'}
files = subprocess.check_output(['git', 'ls-files', '-z'], cwd=ROOT).decode().split('\0')
bad = [p for p in files if p and p not in ALLOWED and
       (Path(p).suffix.lower() in PRIVATE or
        (Path(p).suffix.lower().startswith('.ss') and Path(p).suffix[3:].isdigit()))]
if bad:
    print('Remove these generated/private files from the index (keep local copies):')
    print('\n'.join(bad))
    sys.exit(1)
print('Index hygiene passed. Historical commits and asset rights are not certified.')
