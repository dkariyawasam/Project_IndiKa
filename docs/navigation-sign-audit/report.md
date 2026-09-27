# Navigation sign audit

## Subsequent correction

Removed all four navigation posts added to Routes 4 and 7. Celadon Cave remains unnamed and unsigned before and after the blast; the pre-existing route/Mt. Moon/Underground Path signs remain. Fuchsia Forest’s northern-exit directions now use the existing sign at (6,12); the duplicate post at (7,13) has been removed. The completed pass now retains 13 navigation interactions, one reusing an existing post. Earlier implementation notes below describe the superseded first pass.


Audit followed by an implemented navigation-sign pass. The initial findings below are retained as the rationale; see the completion notes.

## Scope and limits

Scanned all 253 map definitions and 387 background events of type `sign`. These include navigation signs, posters, exhibits and scripted interaction points. The inventory includes legacy/inactive maps; a count is not a count of accessible navigation signs.

Checked existing sign text where resolvable through direct message/call/goto labels, map connections, warp approaches and adjacent collision-free tiles. This is not a live traversal or a proof that each sign is reachable from the player's current route. Branch-dependent text is flattened; unresolved dynamic text is left blank. Directional interaction restrictions, elevation compatibility and full path reachability still require an in-game pass. No assertion that every unnamed destination needs a sign.

## Initial findings (addressed below)

Existing signs are not yet sufficient for the expanded geography. Most retain vanilla place labels rather than directions for new connections. No outdoor sign in this scan lacked a collision-free adjacent tile. Four indoor interaction objects were flagged by the simplistic adjacency check: the Game Corner poster, the two Dojo scrolls and the department-store elevator controls. They require interaction-specific checks, not automatic relocation.

## Recommended work, in order

| Area | Evidence | Recommended change |
|---|---|---|
| Viridian Channel | No sign events. | Name the channel and mark Route 2 / Viridian versus Route 16 / Celadon at the bridge approaches. |
| Vermilion Harbour | No sign events despite two route connections, ship boarding and a forest entrance. | Mark Vermilion City, Route 15, Fuchsia Forest and the S.S. Anne at their actual branches. |
| Route 2 | Route and forest signs exist; the eastern channel branch is not named. | Add a Viridian Channel direction at the eastern fork. |
| Route 4 | Signs cover Mt. Moon and Cerulean; the Route 7 branch and Celadon Cave approach are not named. | Add a junction sign and identify the cave approach, conditionally if story discovery requires it. |
| Route 7 | Only the Underground Path is labelled. | Mark Celadon, Saffron, the Route 4 branch and Celadon Cave; respect the cave’s story timing. |
| Route 10 | Rock Tunnel and Power Plant are named; the Route 25 connection is not. | Distinguish the northern bridge, Route 9, Rock Tunnel and Power Plant at the forks. |
| Route 12 | Signs name Lavender and the fishing area. | Mark the Route 11 / Vermilion branch and continuation towards Route 13. |
| Route 15 | Fuchsia and Fuchsia Forest are named; the harbour connection is not. | Add Vermilion Harbour at the northern branch, retaining Fuchsia / Route 14 directions. |
| Route 16 | Cycling Road and Celadon–Fuchsia route names exist. | Include Viridian Channel at the western branch. |
| Route 18 | Two signs cover Cycling Road / Celadon–Fuchsia only. | Distinguish Route 17, Fuchsia, Route 21 and the sea approach towards Seafoam. |
| Route 20 | Both signs only name Seafoam Islands. | Add Cinnabar, Route 19 / Fuchsia, and the northern Route 18 approach on suitable shore or landing tiles. |
| Route 21 | Only the northern ferry landing is signed. | Add the Route 18 branch and Pallet / Cinnabar directions at useful landings. |
| Routes 24–25 | Route 24 has no signs; Route 25 labels only Sea Cottage. | Mark Sea Cottage versus the extended bridge towards Route 10, with a return direction to Cerulean. |
| Forest interiors | Viridian labels its Pewter exit; Fuchsia has welcome / berry advice. | Label the southern Viridian exit towards Viridian City, and both Fuchsia Forest exits towards Route 15 and Vermilion Harbour. |
| Major city junctions | Existing city signs mainly name buildings or the city itself. | Add selective orientation signs at Cerulean, Saffron, Fuchsia and Viridian’s multi-route junctions; avoid one sign at every doorway. |
| Dungeon entrances | Mt. Moon, Rock Tunnel, Power Plant, Seafoam and Diglett’s Cave already have some approach signs. | Review Victory Road, Cerulean Cave and the new Celadon Cave approaches visually; use internal signs only for major exits, not every puzzle fork. |

## Placement and wording

Use signs just before decisions, facing an accessible approach tile. Name the immediate destination, then optionally the town it leads to. Preserve exploration within dungeons; signs should identify major entrances and exits rather than disclose every puzzle solution. At water routes, prefer shore signs or landing notices to freestanding signs in the sea. Do not label Celadon Cave before the blast if its appearance is story-gated.

## Implementation completed

Added 17 directional posts across Viridian Channel, Vermilion Harbour, Routes 2/4/7/10/18/21/24/25, Cerulean Cave’s approach and the forest exits. Expanded 11 existing sign messages across Routes 12/15/16/20/23, Fuchsia Forest and Cerulean/Saffron/Fuchsia/Viridian cities. Route 12’s fishing-area sign now gives directions at the Route 11 junction as well as the northern route sign.

The harbour pier post identifies the city and S.S. Anne; the southern harbour post identifies Fuchsia Forest and its Route 15 exit. The Route 15 route sign gives the reciprocal forest/harbour guidance. One harbour pier post serves both boarding and return directions rather than placing adjacent duplicates. The pier post retains paving under its graphic.

Both Celadon Cave approach signs use route-only wording before FLAG_GIOVANNI_ACTIVATED_MEWTWO_SIGNAL, then identify the cave and its opposite route after the blast. The signs do not alter the cave-opening scripts or events.

Placement checks exclude existing objects, warp triggers, scripted terrain edits and nearby trainer movement ranges. New posts have a collision-free southern reading tile and preserve local paths between their neighboring walkable tiles. Rendered placements were inspected; see [placements.png](placements.png). Entrance tiles and existing warp events were not changed.

Validation: ROM build passed; `python3 tools/test_navigation_signs.py` passes for all 17 posts, checks cave disclosure conditions and reports zero map-connection conflicts or overlaps. `git diff --check` passed. Full live interaction/reachability testing has not been performed. The four indoor adjacency flags remain interaction-specific review notes, not confirmed navigation defects; no unrelated indoor objects were moved.

## Complete map inventory

Counts below include non-navigation interaction objects. Full coordinates, script names, extracted text, connections and warps are in [inventory.json](inventory.json).

| Map | Sign events | Connections | Warp events |
|---|---:|---:|---:|
| BattleColosseum_2P | 0 | 0 | 2 |
| BattleColosseum_4P | 0 | 0 | 4 |
| CeladonCave | 0 | 0 | 2 |
| CeladonCity | 6 | 2 | 14 |
| CeladonCity_BerryPatch | 7 | 0 | 3 |
| CeladonCity_Condominiums_1F | 2 | 0 | 6 |
| CeladonCity_Condominiums_2F | 2 | 0 | 4 |
| CeladonCity_Condominiums_3F | 8 | 0 | 4 |
| CeladonCity_Condominiums_Roof | 2 | 0 | 3 |
| CeladonCity_Condominiums_RoofRoom | 3 | 0 | 3 |
| CeladonCity_DepartmentStore_1F | 2 | 0 | 8 |
| CeladonCity_DepartmentStore_2F | 1 | 0 | 3 |
| CeladonCity_DepartmentStore_3F | 11 | 0 | 3 |
| CeladonCity_DepartmentStore_4F | 1 | 0 | 3 |
| CeladonCity_DepartmentStore_5F | 1 | 0 | 3 |
| CeladonCity_DepartmentStore_Elevator | 2 | 0 | 2 |
| CeladonCity_DepartmentStore_Roof | 4 | 0 | 1 |
| CeladonCity_GameCorner | 24 | 0 | 4 |
| CeladonCity_GameCorner_PrizeRoom | 0 | 0 | 3 |
| CeladonCity_Gym | 2 | 0 | 3 |
| CeladonCity_Hotel | 0 | 0 | 3 |
| CeladonCity_House1 | 0 | 0 | 3 |
| CeladonCity_PokemonCenter_1F | 0 | 0 | 4 |
| CeladonCity_PokemonCenter_2F | 0 | 0 | 3 |
| CeladonCity_Restaurant | 0 | 0 | 3 |
| CeruleanCave_1F | 0 | 0 | 8 |
| CeruleanCave_2F | 0 | 0 | 6 |
| CeruleanCave_B1F | 0 | 0 | 1 |
| CeruleanCity | 7 | 4 | 14 |
| CeruleanCity_BikeShop | 8 | 0 | 3 |
| CeruleanCity_Gym | 2 | 0 | 3 |
| CeruleanCity_House1 | 0 | 0 | 4 |
| CeruleanCity_House2 | 1 | 0 | 4 |
| CeruleanCity_House3 | 0 | 0 | 3 |
| CeruleanCity_House4 | 0 | 0 | 1 |
| CeruleanCity_House5 | 1 | 0 | 1 |
| CeruleanCity_Mart | 0 | 0 | 3 |
| CeruleanCity_PokemonCenter_1F | 0 | 0 | 4 |
| CeruleanCity_PokemonCenter_2F | 0 | 0 | 3 |
| CinnabarIsland | 4 | 2 | 6 |
| CinnabarIsland_Gym | 3 | 0 | 3 |
| CinnabarIsland_Mart | 0 | 0 | 3 |
| CinnabarIsland_PokemonCenter_1F | 0 | 0 | 4 |
| CinnabarIsland_PokemonCenter_2F | 0 | 0 | 3 |
| CinnabarIsland_PokemonLab_Entrance | 4 | 0 | 6 |
| CinnabarIsland_PokemonLab_ExperimentRoom | 0 | 0 | 1 |
| CinnabarIsland_PokemonLab_Lounge | 0 | 0 | 1 |
| CinnabarIsland_PokemonLab_ResearchRoom | 2 | 0 | 1 |
| CinnabarVolcano_1F | 0 | 0 | 3 |
| CinnabarVolcano_3F | 0 | 0 | 2 |
| CinnabarVolcano_LeftCorridor_1F | 0 | 0 | 2 |
| CinnabarVolcano_LeftCorridor_2F | 0 | 0 | 3 |
| CinnabarVolcano_LeftCorridor_3F | 0 | 0 | 2 |
| CinnabarVolcano_RightCorridor_1F | 0 | 0 | 2 |
| CinnabarVolcano_RightCorridor_2F | 0 | 0 | 2 |
| CinnabarVolcano_RightCorridor_3F | 0 | 0 | 3 |
| DiglettsCave_B2F | 0 | 0 | 2 |
| DiglettsCave_NorthEntrance | 0 | 0 | 2 |
| DiglettsCave_Northside_B1F | 0 | 0 | 2 |
| DiglettsCave_SouthEntrance | 0 | 0 | 2 |
| DiglettsCave_Southside_B1F | 0 | 0 | 2 |
| FuchsiaCity | 5 | 3 | 11 |
| FuchsiaCity_Gym | 2 | 0 | 3 |
| FuchsiaCity_House1 | 0 | 0 | 3 |
| FuchsiaCity_House2 | 0 | 0 | 4 |
| FuchsiaCity_House3 | 0 | 0 | 1 |
| FuchsiaCity_Mart | 0 | 0 | 3 |
| FuchsiaCity_PokemonCenter_1F | 0 | 0 | 4 |
| FuchsiaCity_PokemonCenter_2F | 0 | 0 | 3 |
| FuchsiaCity_SafariZone_Entrance | 0 | 0 | 4 |
| FuchsiaCity_SafariZone_Office | 0 | 0 | 3 |
| FuchsiaCity_WardensHouse | 4 | 0 | 3 |
| FuchsiaForest | 3 | 0 | 6 |
| IndigoPlateau_Exterior | 0 | 1 | 1 |
| IndigoPlateau_PokemonCenter_1F | 1 | 0 | 3 |
| IndigoPlateau_PokemonCenter_2F | 0 | 0 | 3 |
| LavenderTown | 3 | 3 | 5 |
| LavenderTown_HealingHouse | 0 | 0 | 3 |
| LavenderTown_House1 | 0 | 0 | 3 |
| LavenderTown_House2 | 0 | 0 | 3 |
| LavenderTown_VolunteerPokemonHouse | 3 | 0 | 3 |
| MtMoon_1F | 1 | 0 | 4 |
| MtMoon_B1F | 0 | 0 | 8 |
| MtMoon_B2F | 0 | 0 | 4 |
| PalletTown | 6 | 2 | 3 |
| PalletTown_PlayersHouse_1F | 1 | 0 | 4 |
| PalletTown_PlayersHouse_2F | 2 | 0 | 1 |
| PalletTown_ProfessorOaksLab | 2 | 0 | 3 |
| PalletTown_RivalsHouse | 3 | 0 | 3 |
| PewterCity | 4 | 2 | 7 |
| PewterCity_Gym | 2 | 0 | 3 |
| PewterCity_House1 | 0 | 0 | 3 |
| PewterCity_House2 | 0 | 0 | 3 |
| PewterCity_Mart | 0 | 0 | 3 |
| PewterCity_Museum_1F | 4 | 0 | 6 |
| PewterCity_Museum_2F | 8 | 0 | 1 |
| PewterCity_PokemonCenter_1F | 0 | 0 | 4 |
| PewterCity_PokemonCenter_2F | 0 | 0 | 3 |
| PokemonLeague_BrunosRoom | 0 | 0 | 2 |
| PokemonLeague_ChampionsRoom | 0 | 0 | 2 |
| PokemonLeague_HallOfFame | 0 | 0 | 1 |
| PokemonLeague_LancesRoom | 0 | 0 | 2 |
| PokemonLeague_LoreleisRoom | 0 | 0 | 2 |
| PokemonMansion_1F | 1 | 0 | 10 |
| PokemonMansion_2F | 3 | 0 | 5 |
| PokemonMansion_3F | 2 | 0 | 8 |
| PokemonMansion_B1F | 1 | 0 | 1 |
| PokemonTower_1F | 0 | 0 | 2 |
| PokemonTower_2F | 0 | 0 | 2 |
| PokemonTower_3F | 0 | 0 | 2 |
| PokemonTower_4F | 0 | 0 | 1 |
| PowerPlant | 0 | 0 | 3 |
| RockTunnel_1F | 1 | 0 | 6 |
| RockTunnel_B1F | 0 | 0 | 5 |
| RockTunnel_B2F | 9 | 0 | 3 |
| RocketLeague_Arena | 0 | 0 | 1 |
| RocketLeague_ChampionsRoom | 0 | 0 | 1 |
| RocketLeague_Lobby | 1 | 0 | 2 |
| Route1 | 1 | 2 | 0 |
| Route10 | 4 | 3 | 4 |
| Route10_HealingHouse | 0 | 0 | 1 |
| Route11 | 1 | 2 | 3 |
| Route11_EastEntrance_1F | 0 | 0 | 5 |
| Route11_EastEntrance_2F | 1 | 0 | 1 |
| Route12 | 2 | 3 | 4 |
| Route12_FishingHouse | 0 | 0 | 3 |
| Route12_NorthEntrance_1F | 0 | 0 | 5 |
| Route12_NorthEntrance_2F | 2 | 0 | 1 |
| Route13 | 1 | 2 | 0 |
| Route14 | 1 | 2 | 0 |
| Route15 | 2 | 3 | 4 |
| Route15_WestEntrance_1F | 0 | 0 | 5 |
| Route15_WestEntrance_2F | 2 | 0 | 1 |
| Route16 | 2 | 3 | 3 |
| Route16_House | 0 | 0 | 3 |
| Route16_NorthEntrance_1F | 0 | 0 | 3 |
| Route16_NorthEntrance_2F | 2 | 0 | 1 |
| Route17 | 2 | 2 | 0 |
| Route18 | 3 | 4 | 2 |
| Route18_EastEntrance_1F | 0 | 0 | 3 |
| Route18_EastEntrance_2F | 2 | 0 | 1 |
| Route19 | 1 | 2 | 0 |
| Route2 | 4 | 3 | 6 |
| Route20 | 2 | 3 | 2 |
| Route21_North | 2 | 3 | 0 |
| Route21_South | 0 | 0 | 0 |
| Route22 | 1 | 2 | 2 |
| Route22_NorthEntrance | 0 | 0 | 4 |
| Route23 | 1 | 2 | 4 |
| Route24 | 1 | 2 | 0 |
| Route25 | 2 | 2 | 1 |
| Route25_SeaCottage | 1 | 0 | 3 |
| Route2_House | 0 | 0 | 3 |
| Route3 | 1 | 2 | 0 |
| Route4 | 4 | 3 | 4 |
| Route4_HealingHouse | 0 | 0 | 1 |
| Route5 | 1 | 2 | 2 |
| Route5_PokemonDayCare | 0 | 0 | 3 |
| Route6 | 1 | 2 | 1 |
| Route7 | 3 | 3 | 2 |
| Route8 | 1 | 2 | 1 |
| Route9 | 1 | 2 | 0 |
| SSAnne_1F_Corridor | 0 | 0 | 13 |
| SSAnne_1F_Room1 | 0 | 0 | 1 |
| SSAnne_1F_Room2 | 0 | 0 | 1 |
| SSAnne_1F_Room3 | 0 | 0 | 1 |
| SSAnne_1F_Room4 | 0 | 0 | 1 |
| SSAnne_1F_Room5 | 0 | 0 | 1 |
| SSAnne_1F_Room6 | 0 | 0 | 1 |
| SSAnne_1F_Room7 | 0 | 0 | 1 |
| SSAnne_2F_Corridor | 0 | 0 | 9 |
| SSAnne_2F_Room1 | 0 | 0 | 1 |
| SSAnne_2F_Room2 | 0 | 0 | 1 |
| SSAnne_2F_Room3 | 0 | 0 | 1 |
| SSAnne_2F_Room4 | 0 | 0 | 1 |
| SSAnne_2F_Room5 | 0 | 0 | 1 |
| SSAnne_2F_Room6 | 0 | 0 | 1 |
| SSAnne_3F_Corridor | 0 | 0 | 3 |
| SSAnne_B1F_Corridor | 0 | 0 | 6 |
| SSAnne_B1F_Room1 | 0 | 0 | 1 |
| SSAnne_B1F_Room2 | 0 | 0 | 1 |
| SSAnne_B1F_Room3 | 0 | 0 | 1 |
| SSAnne_B1F_Room4 | 0 | 0 | 1 |
| SSAnne_B1F_Room5 | 0 | 0 | 1 |
| SSAnne_CaptainsOffice | 3 | 0 | 1 |
| SSAnne_Deck | 0 | 0 | 2 |
| SSAnne_Kitchen | 0 | 0 | 1 |
| SafariZone_Center | 2 | 0 | 13 |
| SafariZone_Center_RestHouse | 0 | 0 | 3 |
| SafariZone_East | 2 | 0 | 7 |
| SafariZone_East_RestHouse | 0 | 0 | 3 |
| SafariZone_North | 2 | 0 | 13 |
| SafariZone_North_RestHouse | 0 | 0 | 3 |
| SafariZone_SecretHouse | 0 | 0 | 3 |
| SafariZone_West | 3 | 0 | 11 |
| SafariZone_West_RestHouse | 0 | 0 | 3 |
| SaffronCity | 7 | 4 | 9 |
| SaffronCity_CopycatsHouse_1F | 0 | 0 | 4 |
| SaffronCity_CopycatsHouse_2F | 2 | 0 | 1 |
| SaffronCity_Dojo | 4 | 0 | 3 |
| SaffronCity_Gym | 2 | 0 | 33 |
| SaffronCity_House | 1 | 0 | 3 |
| SaffronCity_Mart | 0 | 0 | 3 |
| SaffronCity_MrPsychicsHouse | 0 | 0 | 3 |
| SaffronCity_PokemonCenter_1F | 0 | 0 | 4 |
| SaffronCity_PokemonCenter_2F | 0 | 0 | 3 |
| SaffronCity_PokemonTrainerFanClub | 0 | 0 | 1 |
| SeafoamIslands_1F | 0 | 0 | 4 |
| SeafoamIslands_B1F | 0 | 0 | 0 |
| SeafoamIslands_B2F | 0 | 0 | 3 |
| SeafoamIslands_B3F | 0 | 0 | 0 |
| SeafoamIslands_B4F | 1 | 0 | 1 |
| SilphCo_10F | 5 | 0 | 6 |
| SilphCo_11F | 5 | 0 | 3 |
| SilphCo_1F | 1 | 0 | 5 |
| SilphCo_2F | 9 | 0 | 7 |
| SilphCo_3F | 9 | 0 | 10 |
| SilphCo_4F | 9 | 0 | 7 |
| SilphCo_5F | 16 | 0 | 7 |
| SilphCo_6F | 5 | 0 | 5 |
| SilphCo_7F | 13 | 0 | 6 |
| SilphCo_8F | 5 | 0 | 7 |
| SilphCo_9F | 17 | 0 | 5 |
| SilphCo_Elevator | 1 | 0 | 1 |
| TradeCenter | 0 | 0 | 2 |
| UndergroundPath_EastEntrance | 0 | 0 | 4 |
| UndergroundPath_NorthEntrance | 0 | 0 | 4 |
| UndergroundPath_SouthEntrance | 0 | 0 | 4 |
| UndergroundPath_Tunnel | 0 | 0 | 4 |
| UndergroundPath_WestEntrance | 0 | 0 | 4 |
| UnionRoom | 0 | 0 | 1 |
| VermilionCity | 4 | 3 | 7 |
| VermilionCity_Gym | 2 | 0 | 3 |
| VermilionCity_House1 | 0 | 0 | 3 |
| VermilionCity_House2 | 0 | 0 | 3 |
| VermilionCity_House3 | 1 | 0 | 3 |
| VermilionCity_Mart | 0 | 0 | 3 |
| VermilionCity_PokemonCenter_1F | 0 | 0 | 4 |
| VermilionCity_PokemonCenter_2F | 0 | 0 | 3 |
| VermilionCity_PokemonFanClub | 2 | 0 | 3 |
| VermilionHarbor | 2 | 2 | 4 |
| VictoryRoad_1F | 0 | 0 | 2 |
| VictoryRoad_2F | 0 | 0 | 9 |
| VictoryRoad_3F | 0 | 0 | 5 |
| ViridianChannel | 2 | 2 | 0 |
| ViridianCity | 3 | 3 | 5 |
| ViridianCity_Gym | 2 | 0 | 3 |
| ViridianCity_House | 1 | 0 | 3 |
| ViridianCity_Mart | 0 | 0 | 3 |
| ViridianCity_PokemonCenter_1F | 0 | 0 | 4 |
| ViridianCity_PokemonCenter_2F | 0 | 0 | 3 |
| ViridianCity_School | 5 | 0 | 3 |
| ViridianForest | 2 | 0 | 6 |
