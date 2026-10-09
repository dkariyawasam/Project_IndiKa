# Map connection pass — 9 October 2026

Controlled live QA on a disposable copy of the 51-owned campaign save. Debug placement brought the player near each approach; crossings themselves used normal directional inputs. No changes to the earned campaign or main user save. This is connection/rendering coverage, not an earned travel or encounter-balance pass.

## Findings and fixes

- Pallet–Route 21: the current build rendered the northern approach, seam, Pallet arrival, and southbound return cleanly at the land crossing. The previously reported black/purple corruption did not reproduce. No further Pallet code change was made.
- Fuchsia Forest south exit: both warp-event positions had ordinary ground behavior instead of a south-exit trigger. Replaced them with the existing Viridian Forest natural south-exit metatiles.
- Vermilion Harbor forest approach: ordinary ground occupied the exit positions and two colliding tree tiles blocked the approach. Added two metatiles reusing Route 2's natural south-exit graphics and behavior (all referenced graphics are in the shared primary tileset), and cleared only the two approach tiles. Both entry lanes and the forest's two return lanes are checked live.

The changes preserve map dimensions, connection offsets, events and the surrounding terrain.

## Validation

`python3 tools/test_natural_forest_exits.py` checks all 16 natural forest entrance/exit positions, directional warp behavior, event presence and two clear approach tiles per lane. `python3 tools/test_route21_tileset.py` passes all four shared-border tests. The collision audit reports 246 maps with zero hard findings and zero invalid fixed warp targets; water and seam heuristics remain candidates rather than certified defects. `make -j8` passes.

Live results and selected screenshots accompany this report. Coverage is limited to the listed crossings and camera positions, not every tile, tide/time palette, or possible approach.

## Live crossing coverage

- Pallet ↔ Route 21: land and water approaches, both directions, clean rendering.
- Route 21 ↔ Route 18 and Route 18 ↔ Route 20: water crossings, both directions.
- Route 18 ↔ Fuchsia City: both directions.
- Viridian Channel ↔ Route 2 and Viridian Channel ↔ Route 16: both bridge ends, both directions.
- Vermilion City ↔ Vermilion Harbor: both directions.
- Viridian Forest: both natural entrances and exits.
- Fuchsia Forest: Route 15 entrance; both repaired southern exit lanes; both repaired harbor entrance lanes and both northern return lanes.

The initial fixture attempt to place the player at Route 17 y=158 was invalid because the debug warp path narrows coordinates to signed bytes. That sample is excluded; the cycling seam is checked by crossing normally from Route 18 and immediately returning.

The earned campaign save still matches checkpoint SHA-256 `001a04fc396009ff0a8fecd353eba53a721b32f6577dc2c19950472aa66b4ac4`.

Route 18 ↔ Route 17 also passed in both directions using continuous ordinary movement. The repaired harbor entrance lands inside Fuchsia Forest and permits walking onward (6,9 → 6,12).

The return landing in Vermilion Harbor also permits walking out north (27,48 → 27,45), confirming the former blocked recess is cleared.
