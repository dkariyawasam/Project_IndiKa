# Installing Pokémon Expeditions Kanto

This is a development preview, not a final release. Player distribution is a BPS patch with credits and checksums. A patch is not legal clearance for the underlying content; see [rights and attribution](RIGHTS.md).

## Required base

Use an unmodified English Pokémon FireRed revision 0 file obtained and used in accordance with applicable law. No ROM is supplied here.

- SHA-1: `41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc`
- LeafGreen, FireRed revision 1, translations and previously patched files are not supported bases.

## Apply the patch

1. Back up any ROM and battery save you already use.
2. Use a BPS-compatible patcher such as [Floating IPS](https://github.com/Sir-Walrus/Flips). Select the release `.bps`, the clean base, and a separate output file.
3. Do not bypass a source-checksum error. Verify the base hash instead.
4. Compare the output SHA-256 with `target.sha256` in the package's `manifest.json`.
5. Open the patched output in a GBA emulator. A new game is recommended. Existing cross-version saves are not certified; emulator save states are not a substitute for an in-game save and CONTINUE.

The local development build has been tested with mGBA 0.10.5. This does not imply certification of every emulator or device.

## Preview scope

Playthrough and trainer line-of-sight reviews have been accepted. Sprite polish remains unfinished; some imported asset attribution is unresolved. Custom-ability-specific live battle testing is deferred. Patch Disc distribution/Porygon3 acquisition and extra ferry destinations remain deliberately unavailable. These are not promised preview features.

See [credits](docs/credits/README.md) and the [asset attribution report](docs/credits/sprite-credit-research-oct12.md). If you can identify missing creators, contact Dee Kariyawasam through the project release page or repository issues. Do not upload ROMs or personal saves with reports.

## Maintainer packaging

Build the mod locally using INSTALL.md and `make COMPARE=0`, review it, and refresh the staging manifest's target hashes to the approved build. Keep the clean base outside Git. With Floating IPS installed:

```sh
python3 tools/package_release.py --base /path/to/clean-firered.gba --output dist/expeditions-kanto-preview
```

Use `--flips /path/to/flips` if it is not on PATH. The command rejects the wrong base, a stale target manifest and existing output destinations. It generates BPS, reapplies it to the clean base, compares the reconstructed ROM byte-for-byte via SHA-256, and creates a ZIP containing only the patch and documentation. It never publishes anything. Retain the package checksums when uploading an approved release.

Current staging is `release/2026-10-12-preview`. A staging directory without a `.bps` is not an installable release. Source checkout instructions and maintainer tools are not included in the player ZIP.
