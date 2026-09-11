# Trendsetter encounters

Nine distinct battles: three male singles, three female singles, and three doubles. The doubles each have one male and one female overworld object, for twelve people total (six male and six female).

| Type | Trainer | Map | Tile coordinates | Team |
| --- | --- | --- | --- | --- |
| Double | BRI & REMI | SSAnne_1F_Room4 | (4, 3), (5, 3) | Pikachu Lv. 12, Clefairy Lv. 13 |
| Double | LEA & JED | Route16 | (28, 10), (29, 10) | Meowth Lv. 18, Happiny Lv. 18 |
| Double | TAMIA & LEO | CeladonCity | (7, 17), (8, 17) | Vulpix Lv. 19, Meowth Lv. 20 |
| Female | CLAIRE | Route7 | (16, 60) | Meowth Lv. 18, Clefairy Lv. 19 |
| Female | DAWN | SSAnne_2F_Room4 | (5, 4) | Meowth Lv. 10, Happiny Lv. 11 |
| Female | LORI | CeladonCity | (18, 17) | Clefairy Lv. 19, Eevee Lv. 20 |
| Male | DALE | SSAnne_2F_Room2 | (5, 5) | Meowth Lv. 18, Growlithe Lv. 18, Happiny Lv. 18 |
| Male | ELIO | Route7 | (11, 66) | Meowth Lv. 18, Growlithe Lv. 19 |
| Male | TYLER | SSAnne_1F_Room2 | (5, 4) | Meowth Lv. 10 |

Celadon and S.S. Anne additions are stationary and begin on interaction. Claire and Elio on Route 7 have two-tile sight ranges. New placements use passable tiles and avoid existing objects, warps, triggers, and background events.

Claire moved from Route 15 to Route 7. Elio is placed on Route 7 and has his own trainer-defeated flag. Route 8 has no Trendsetter encounters. Tyler remains aboard the S.S. Anne. Bri & Remi and Tamia & Leo use previously unplaced trainer entries; existing trainer numeric IDs and MAX_TRAINERS_COUNT are preserved.

Validation: 3/3/3 unique encounters, independent trainer IDs, double-battle flags and parties, map collision checks, 310/310 overworld matches, and make -j4 passed. The broader wiring audit reports the same ten pre-existing unreferenced party definitions. No emulator playtest performed.
