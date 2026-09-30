# Aeropteryx FRLG pixel pass

Direct pixel editing of the original 64×64 draft, preserving its silhouette and pose. No generated artwork is used in this pass.

Changes: consolidated bone shading into larger contiguous planes; clearer eye socket, mouth and jaw; simplified chest plate; separated the folded wing bones and purple membranes; restored pale gold wing-tip accents.

- `front.png`: native 64×64 indexed PNG; transparent index 0, 16-entry palette (14 visible colours used).
- `normal.pal`: matching JASC palette, with GBA-compatible colour channel values.
- `front-preview.png`: 8× nearest-neighbour preview.
- `comparison.png`: original and refined drafts at 6× and native size.
- `refine.py`: reproducible pixel edits; run from the repository root with Pillow.

Validated both PNG and palette with the repository's `tools/gbagfx/gbagfx` converter (2048-byte 4bpp sprite, 32-byte palette). The original draft remains in the parent directory. This pass is not installed in the ROM.
