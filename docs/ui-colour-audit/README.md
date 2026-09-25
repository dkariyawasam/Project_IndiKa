# Default UI colour audit

Checked 2026-09-24 using the main-build menu captures in `docs/blog/images`, default settings, and palette-loading code. This is a core-menu audit, not a live sweep of every battle, PC, shop, multiplayer, and minigame state.

## Findings

The game has a shared blue header theme, but not every interface panel is blue. Neutral text/backgrounds and meaningful colours (HP, status, types, gender, highlighted choices, map terrain, item/Pokémon artwork) are distinct from decorative interface colours.

| Area | Finding | Evidence |
| --- | --- | --- |
| New-game dialogue frame | Blue. Frame 1 is the default (index 0), mapped to the blue type5 asset. Existing saves retain their selected frame. | `src/new_game.c`, `src/text_window_graphics.c`, `graphics/text_window/type5.png` |
| Shared hint headers | Blue, RGB(0,15,25) in GBA 5-bit channels. Palette also includes functional text colours. | `src/ui_hint_header.c` |
| Radial menu | Blue hints and description; gold/brown wheel and icons. | [Capture](../blog/images/radial-current.png) |
| Bag | Blue header/description, teal striped background, cream/yellow list panel. | [Capture](../blog/images/bag-items.png) |
| Field notes / gym / Apex logs | Blue headers/description, cream/yellow list panel, orange paging indicator. | [Capture](../blog/images/field-notes.png) |
| Pokédex | Blue shared header; habitat and entry panels are not uniformly blue (beige/brown remains). | [Capture](../blog/images/dex-current.png) |
| Party screen | Blue hints with teal panels; HP/status colours remain functional. | Fresh capture `/tmp/kanto-blog-captures/party-menu.png` |
| Pokémon summary | Blue hints, purple level/name strip and orange information-row accents. | [Capture](../blog/images/pokemon-summary.png) |
| Trainer Card | Captured card is blue/teal. Other rank palettes and the female background palette still exist; not all variants have been visually verified. | [Capture](../blog/images/trainer-card.png), `src/trainer_card.c` |
| Help | Blue header but yellow/orange outer surround and grey content. | [Capture](../blog/images/help-header.png) |
| Settings | Uses its own option palette, blue help-window palette, and the selected dialogue frame. Full screen not freshly captured for this audit. | `src/option_menu.c` |
| Region map | Independent map, top-bar and cursor palettes. Terrain should retain geographical colours. Full screen not freshly captured for this audit. | `src/region_map.c` |
| Battle / PC / shops and specialist screens | Separate palette paths; not covered by a claim that the common header makes all UI blue. Additional visual pass needed for complete coverage. | `graphics/battle_interface`, `graphics/pokemon_storage` |

No blanket palette replacement was made. To make decorative UI uniformly blue, the cream/yellow panels, Help surround, summary accents and radial wheel would need a deliberate colour pass. Preserve semantic colours and artwork during that pass.

## Daisy and the map

`SetUpStartMenu_NormalField` adds MAP unconditionally, independent of possession of ITEM_TOWN_MAP. Daisy still had the vanilla gift branch. It has now been replaced with advice to press START and choose MAP. It advances the same scene variable to 2, so later rest dialogue and postgame grooming/friendship services remain intact. There is no item award, inventory-capacity check, gift fanfare or table-map removal in the replacement branch. The table map description now points to the menu too.

Existing saves that already received the old item keep it; no save inventory migration was performed. The old scene-variable values remain compatible.

Validation: main ROM build succeeded. Script diff confirms all Daisy item-grant paths were removed, and the scene-2 and postgame branches remain present. The revised dialogue has not yet been live-playtested.
