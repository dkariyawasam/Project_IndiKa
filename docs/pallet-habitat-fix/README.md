# Pallet boundary and habitat badges — 2026-10-03

Pallet's tileset now uses Route 21's physical tile graphics. Its full metatile table is generated from the original Pallet table with secondary tile references shifted by 128. Map IDs, layouts, attributes and Pallet's palettes remain unchanged. This prevents the graphics themselves changing under cached screen tiles when crossing the boundary. The saved-viewport translation also preserves MAPGRID_UNDEFINED rather than converting it into an ordinary or invalid metatile.

Habitat completion balls use explicit roles from the already loaded Dex palette: red, white, grey and black. Nearest-RGB matching previously collapsed the red shadow and outline into brown, producing the blocky right edge.

Validation: ROM build, four Route 21 tileset tests, and the page-reward host tests passed. On an isolated ROM/save copy, walked south from Pallet to Route 21 and north back into Pallet; screenshots show intact terrain and building graphics. The original reported corruption was not reproduced from the clean-load fixture, so this is not proof of every possible triggering path. Habitat screenshot confirms the revised outlines. Main player save was untouched.
