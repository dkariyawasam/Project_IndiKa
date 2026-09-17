# Viridian Channel trainers

Viridian Channel connects Route 2 to Route 16 by a walkable bridge. Added three optional trainers with early-route teams:

| Trainer | Position | Team |
| --- | --- | --- |
| Fisherman FEN | (10,14), south-facing platform | Goldeen 7, Poliwag 8 |
| Fisherman HARLAN | (17,11), north-facing platform | Magikarp 9, Goldeen 10 |
| Swimmer CORAL | (22,16), water, facing west | Staryu 9, Horsea 10 |

Stationary, sight range 1, no story flags. Both Fishermen face out from the crossing; the Swimmer is an optional water encounter. Dialogue explains fishing on the bridge and the Route 2/Route 16 connection. Each has intro, defeat, post-battle and rematch text, with the existing same-team VS Seeker fallback.

Validation: ROM build passed. All placements match tile collision/elevation; a walking-path flood fill still connects west to east with the trainers blocking their occupied tiles. Trainer overworld audit has no new mismatches; habitat audit has no hard/deprecated-class violations; VS Seeker coverage passes. Heatmap refreshed (441 project encounter sites). No live battle or traversal test performed.

## Platform visibility investigation

The attempted platform-layer change was reverted after a reported map glitch when entering from Route 2. Original platform tiles, attributes and map references are restored; the three trainers remain. Fisherman visibility is unresolved.

## Confirmed Fisherman tiling fix

The horizontal 160×32 Fisherman sheet was using generic tile packing instead of 16×32 frame packing. 2,751 of 5,120 pixels differed when decoded as the expected ten frames. Added the explicit `-mwidth 2 -mheight 4` conversion rule and regenerated the graphic. Every pixel in all ten frames now matches the source PNG when decoded directly from the rebuilt ROM. No map or palette changes in this fix.

## Swimmer F water palette fix

Water Swimmer F retained fixed NPC palette slot 3 despite using the bespoke swimmer palette tag. Changed to dynamic loading, matching the land variant. Source palette matches all used PNG colour indices; all ten water frames are correctly packed. Built-ROM graphics metadata verified (tag 0x1137, dynamic slot 15). Palette lifetime/recovery regression checks passed. No map or artwork changes. Live visual confirmation pending.
