# Live graphical check — 9 October 2026

Used the disposable traversal fixture, not the main user save or earned campaign. Menus were opened with ordinary controls; debug placement was used only to reach the sampled overworld scenes. Theme changes were made through Settings and were not saved to the earned campaign.

## Menus checked

- Blue: radial menu, Pokédex habitats/page/details/area and return, all five Bag pockets, map, Help, Settings, both Field Aide card sides, logbook/list details, party menu and all three Pokémon summary pages.
- Red and green: Settings, radial menu, Pokédex habitats/navigation, Bag, map and Help.
- Completed Mountain habitat marker rendered correctly in all three themes.
- Captured 120 consecutive green radial-menu frames and 120 Pokédex frames while moving the selection. Every captured header pixel at (0,0) remained green (107,189,74); the full Pokédex row at y=15 was identical across all 120 frames. The radial row changes with the animated overworld underneath, as intended. No blue flashes or header-edge corruption reproduced in these samples.

## Fixed finding

Help's header reserved colour 10 retained the generic teal colour, although its panel background is gold. Load that edge colour from Help's existing background palette before drawing the header. Rebuilt and reopened Help live: all 240 pixels in row 15 now match RGB (255,231,90), replacing (107,206,198). Screenshots `help.png` and `help-fixed.png` show before/after.

## Validation and limits

Build passed. The compiled overworld palette audit passed 181 sprite/palette pairs with no detected index permutations or whole-map fixed-slot conflicts. Live object palette records and overworld screenshots are retained separately. This is sampled graphical coverage, not a claim that every battle animation, reward state, weather combination or camera position has been checked. The Help fix was verified after reloading the rebuilt ROM; the broader menu/theme samples were captured immediately before that small correction.

Aroma Lady Mia's normal forest sightline triggered a battle during sampling. Her overworld sprite, introductory portrait, Oddish/Bellsprout front sprites, Venusaur back sprite and battle UI rendered cleanly in the captured views. The disposable battle was ended by soft reset and normal CONTINUE; this is not a completed-match or balance result.

Full transient captures (120 frames per menu) remain in `/tmp/kanto-graphics-oct9`; representative frames are retained here. The earned campaign save checksum remains `001a04fc396009ff0a8fecd353eba53a721b32f6577dc2c19950472aa66b4ac4`.

Seven completed overworld samples across six maps (two Game Corner positions, Vermilion fan club, Saffron fan club, Celadon, Pallet and Vermilion Harbor) reported no mismatched dynamic sprite palette tags. Their screenshots were visually inspected; no further graphical defects reproduced.
