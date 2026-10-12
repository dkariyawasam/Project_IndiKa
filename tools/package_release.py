#!/usr/bin/env python3
"""Create a patch-only package after validating the base and round-trip output."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import zipfile

ROOT = Path(__file__).resolve().parents[1]
BASE_SHA1 = '41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc'


def digest(path, algorithm='sha256'):
    return hashlib.new(algorithm, path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--base', type=Path, required=True)
    parser.add_argument('--target', type=Path, default=ROOT / 'pokefirered.gba')
    parser.add_argument('--flips', default='flips')
    parser.add_argument('--output', type=Path, required=True, help='New package directory')
    args = parser.parse_args()
    base, target, output = args.base.resolve(), args.target.resolve(), args.output.resolve()
    if digest(base, 'sha1') != BASE_SHA1:
        parser.error('Base is not the required English FireRed revision 0. No package created.')
    if base == target or base.read_bytes() == target.read_bytes():
        parser.error('Target must be a modified build, distinct from the base.')
    stage = ROOT / 'release/2026-10-12-preview'
    metadata = json.loads((stage / 'manifest.json').read_text())
    if digest(target) != metadata['target']['sha256']:
        parser.error('Target differs from the reviewed staging manifest. Refresh staging first.')
    if output.exists() or output.with_suffix('.zip').exists():
        parser.error('Output directory or ZIP already exists. Choose a new destination.')
    flips = shutil.which(args.flips)
    if not flips:
        parser.error('Floating IPS not found. Supply --flips /path/to/flips.')
    subprocess.run(['python3', str(ROOT / 'tools/check_release_hygiene.py')], check=True)
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='expedition-patch-') as scratch:
        scratch = Path(scratch)
        package = scratch / 'package'
        package.mkdir()
        patch = package / 'Expeditions-Kanto-preview.bps'
        subprocess.run([flips, '--create', '--bps', str(base), str(target), str(patch)], check=True)
        rebuilt = scratch / 'verified.gba'
        subprocess.run([flips, '--apply', str(patch), str(base), str(rebuilt)], check=True)
        if digest(rebuilt) != digest(target):
            raise RuntimeError('Round-trip ROM does not match target; package withheld.')
        shutil.copytree(stage / 'docs', package / 'docs')
        for source, name in [(ROOT / 'RELEASE.md', 'README.md'), (ROOT / 'RIGHTS.md', 'RIGHTS.md')]:
            shutil.copy2(source, package / name)
        metadata['status'] = 'BPS generated and round-trip verified; not a legal-clearance claim'
        metadata['source_commit'] = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
        metadata['includes_uncommitted_changes'] = bool(subprocess.check_output(['git', 'status', '--porcelain'], cwd=ROOT))
        metadata['patch_sha256'] = digest(patch)
        metadata['base_sha1'] = BASE_SHA1
        metadata['rom_included'] = False
        (package / 'manifest.json').write_text(json.dumps(metadata, indent=2) + '\n')
        # Explicit extension allowlist; never archive the working tree or scratch ROM.
        allowed = {'.bps', '.md', '.txt', '.json'}
        files = sorted(p for p in package.rglob('*') if p.is_file())
        for p in files:
            if p.is_symlink() or p.suffix.lower() not in allowed:
                raise RuntimeError(f'Unexpected release file: {p.relative_to(package)}')
        (package / 'SHA256SUMS.txt').write_text(''.join(f'{digest(p)}  {p.relative_to(package)}\n' for p in files))
        shutil.copytree(package, output)
    with zipfile.ZipFile(output.with_suffix('.zip'), 'x', zipfile.ZIP_DEFLATED) as archive:
        for p in sorted(output.rglob('*')):
            if p.is_file():
                archive.write(p, str(Path(output.name) / p.relative_to(output)))
    print(f'Verified patch-only package: {output.with_suffix(".zip")}')


if __name__ == '__main__':
    main()
