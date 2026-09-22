# Pokémon encounter gaps and official Johto availability

Scope: the 399-species active project roster. Exclude species that appear in wild encounter tables, trainer-party definitions, or explicit scripted wild battles. Also exclude any Pokémon reachable through one or more implemented evolution steps starting from a Kanto wild encounter Pokémon (including scripted wild encounters) or an explicitly scripted gift Pokémon. This includes Eevee’s stone/friendship evolutions and Porygon’s trade evolutions. Evolution routes count even when they require items, trading, moves, discovery, or habitat gates. This checks configured evolution routes, not full item accessibility or runtime reachability.

The previous direct-encounter rule identified 94 candidates. The new evolution rule removes 16, leaving **78 report candidates**. Gift-only starting Pokémon remain candidates under the encounter-focused rule, but their implemented evolutions are excluded. This is a static source audit, not a live proof of every reachable encounter: unused trainer definitions can conceal additional gaps, and variable-driven encounters need separate tracing. It is not a definitive unobtainable-species count.

Official comparison: Gold, Silver, Crystal, HeartGold and SoulSilver, restricted to Johto locations **excluding the Johto Safari Zone**. Pokémon with another qualifying Johto location remain eligible; Safari-only species and evolutions do not. Kanto locations within those games, external transfers and Pokéwalker are separate. Celebi's Crystal encounter requires its event/Virtual Console unlock. No Pokémon GO geographic spawns, arbitrary player trades, or player-imported Pokémon counted as native Johto availability. Radio, swarms and other conditions still apply; the location evidence is not a promise of an ordinary unconditional grass encounter.

PokéAPI's community encounter data provides game/location/method evidence; it is not an official publisher dataset. Evolution classifications are inferred from its ancestry data with explicit exceptions for Leafeon/Glaceon. Project evolution notes record table links, not a full obtainable-item/ancestry proof. Active placeholders are included, deliberately retired species excluded. No game assets or encounters changed.

## Updated project decisions

- AIPOM can evolve into AMBIPOM in FUCHSIA FOREST with DOUBLE HIT; no wild Apex has developed there, so no discovery gate applies.
- LICKITUNG can evolve into LICKILICKY in CERULEAN CAVE with ROLLOUT only after MEWTWO is defeated or caught. Mewtwo occupies its potential Apex habitat. Cerulean Cave is distinct from Celadon Cave.
- SWINUB, PILOSWINE and MAMOSWINE belong to Johto and remain together in the non-native Pokédex habitat. MAMOSWINE has no Kanto evolution rule.
- TANGROWTH, ANNIHILAPE, MIME SR. and OSSCYTHE retain their scripted encounters; their pre-evolutions now evolve after discovery, knowing ANCIENTPOWER / RAGE FIST / BARRIER / BONE RUSH respectively, and levelling up in Viridian Forest / Mt. Moon / Diglett's Cave / Pokémon Tower. Their required moves, plus DOUBLE HIT and ROLLOUT above, are learned at level 33.
- ARTICUNO, ZAPDOS, MOLTRES and MEWTWO are also Apex species. All eleven have the Pokédex Apex marker/highlight; that UI classification does not imply an evolution or local encounter.
- The count now excludes evolutions reachable from wild Pokémon or explicit gifts. AMBIPOM and LICKILICKY qualify through their implemented habitat evolution routes. MAMOSWINE has no player evolution route, but is now excluded because Rocket League Champion Giovanni uses it.
- Existing saves that resolved Mewtwo before the new victory flag was added cannot reliably distinguish a past win from escape. See [Apex evolution rules](../apex-evolution/README.md).

Trainer exclusion was rechecked against every species in `src/data/trainer_parties.h`: none of the remaining candidates appears in a defined trainer team. This is broader than verified Kanto trainer encounters because unused party definitions are also excluded.

Lorelei’s active six-member team includes PILOSWINE and WEAVILE. The Pokéwalker-only Snorunt line has been retired. MAMOSWINE belongs exclusively to Rocket League Champion Giovanni, replacing Nidoqueen. All three are excluded from the candidate count. Mamoswine remains non-native and cannot be evolved locally by the player.

## Counts

- Custom species: 2
- Evolution of Johto source: 22
- Excluded Johto Safari source: 2
- HGSS Kanto source or revival: 4
- Johto encounter: 41
- Johto event: 1
- Johto gift: 5
- Later generation/form: 1

## Sources

- [Encounter/location/species dataset](https://github.com/PokeAPI/pokeapi/tree/master/data/v2/csv) (downloaded 2026-09-19; relevant evidence retained in data.json and report rows).
- [HGSS Safari Zone](https://www.serebii.net/heartgoldsoulsilver/safarizone.shtml) (excluded source; retained for provenance)
- [Hoenn/Sinnoh radio encounters](https://www.serebii.net/heartgoldsoulsilver/hoennsinnoh.shtml)
- [Pokéwalker species list](https://bulbapedia.bulbagarden.net/wiki/Pokemon_found_in_Pokewalker)
- [Eevee evolution restrictions](https://www.serebii.net/games/eevee.shtml)
- [HGSS unobtainable Pokémon](https://www.serebii.net/heartgoldsoulsilver/unobtainable.shtml)
- [Anorith](https://www.serebii.net/pokedex-dp/347.shtml), [Lileep](https://www.serebii.net/pokedex-dp/345.shtml), [Clamperl](https://www.serebii.net/pokedex-dp/366.shtml), [Buneary](https://www.serebii.net/pokedex-dp/427.shtml), [Porygon](https://www.serebii.net/pokedex-dp/137.shtml).

[Crystal Celebi](https://www.serebii.net/crystal/celebi.shtml); [Johto fossil locations](https://www.serebii.net/heartgoldsoulsilver/rocksmash.shtml).

## Every candidate

| Pokémon | Official Johto status | Evidence | Project evolution/gift note |
|---|---|---|---|
| ABSOL | Johto encounter | heartgold / dark-cave / walk; heartgold / ice-path / walk; heartgold / johto-route-47 / walk; heartgold / mt-mortar / walk; heartgold / mt-silver / walk; heartgold / slowpoke-well / walk; heartgold / tohjo-falls / walk; heartgold / union-cave / walk; heartgold / whirl-islands / walk; soulsilver / dark-cave / walk; soulsilver / ice-path / walk; soulsilver / johto-route-47 / walk; soulsilver / mt-mortar / walk; soulsilver / mt-silver / walk; soulsilver / slowpoke-well / walk; soulsilver / tohjo-falls / walk; soulsilver / union-cave / walk; soulsilver / whirl-islands / walk | No incoming evolution in project table |
| AMPHAROS | Evolution of Johto source | Evolve FLAAFFY. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from FLAAFFY |
| ANORITH | HGSS Kanto source or revival | Claw Fossil can be found in Johto Cliff Cave; revival is in Pewter City, Kanto. | No incoming evolution in project table |
| ARMALDO | HGSS Kanto source or revival | Evolution of a Pokémon obtained or revived in Kanto in HGSS. | Evolves from ANORITH |
| AZUMARILL | Evolution of Johto source | Evolve MARILL. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from MARILL |
| AZURILL | Excluded Johto Safari source | Only Johto Safari Zone evidence found for this Pokémon or its pre-evolutions; excluded by the project source rule. | No incoming evolution in project table |
| BARBOACH | Excluded Johto Safari source | Only Johto Safari Zone evidence found for this Pokémon or its pre-evolutions; excluded by the project source rule. | No incoming evolution in project table |
| BAYLEEF | Evolution of Johto source | Evolve CHIKORITA. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from CHIKORITA |
| CELEBI | Johto event | crystal / ilex-forest / static | No incoming evolution in project table |
| CHIKORITA | Johto gift | crystal / new-bark-town / gift; gold / new-bark-town / gift; heartgold / new-bark-town / gift; silver / new-bark-town / gift; soulsilver / new-bark-town / gift | No incoming evolution in project table |
| CRADILY | HGSS Kanto source or revival | Evolution of a Pokémon obtained or revived in Kanto in HGSS. | Evolves from LILEEP |
| CROCONAW | Evolution of Johto source | Evolve TOTODILE. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from TOTODILE |
| CYNDAQUIL | Johto gift | crystal / new-bark-town / gift; gold / new-bark-town / gift; heartgold / new-bark-town / gift; silver / new-bark-town / gift; soulsilver / new-bark-town / gift | No incoming evolution in project table |
| DELIBIRD | Johto encounter | crystal / ice-path / walk; silver / ice-path / walk; soulsilver / ice-path / walk | No incoming evolution in project table |
| DRAGONAIR | Johto encounter | crystal / dragons-den / super-rod; crystal / johto-route-45 / super-rod; gold / dragons-den / super-rod; gold / johto-route-45 / super-rod; heartgold / dragons-den / super-rod; silver / dragons-den / super-rod; silver / johto-route-45 / super-rod; soulsilver / dragons-den / super-rod | Evolves from DRATINI |
| DRATINI | Johto encounter | crystal / dragons-den / gift; crystal / dragons-den / good-rod; crystal / dragons-den / super-rod; crystal / dragons-den / surf; crystal / johto-route-45 / good-rod; crystal / johto-route-45 / super-rod; gold / dragons-den / good-rod; gold / dragons-den / super-rod; gold / dragons-den / surf; gold / goldenrod-city / gift; gold / johto-route-45 / good-rod; gold / johto-route-45 / super-rod; heartgold / dragons-den / gift; heartgold / dragons-den / good-rod; heartgold / dragons-den / super-rod; heartgold / dragons-den / surf; heartgold / goldenrod-city / gift; silver / dragons-den / good-rod; silver / dragons-den / super-rod; silver / dragons-den / surf; silver / goldenrod-city / gift; silver / johto-route-45 / good-rod; silver / johto-route-45 / super-rod; soulsilver / dragons-den / gift; soulsilver / dragons-den / good-rod; soulsilver / dragons-den / super-rod; soulsilver / dragons-den / surf; soulsilver / goldenrod-city / gift | No incoming evolution in project table |
| DUNSPARCE | Johto encounter | crystal / dark-cave / walk; gold / dark-cave / walk; heartgold / dark-cave / rock-smash; heartgold / dark-cave / walk; silver / dark-cave / walk; soulsilver / dark-cave / rock-smash; soulsilver / dark-cave / walk | No incoming evolution in project table |
| ENTEI | Johto encounter | crystal / roaming-johto / roaming-grass; gold / roaming-johto / roaming-grass; heartgold / roaming-johto / roaming-grass; silver / roaming-johto / roaming-grass; soulsilver / roaming-johto / roaming-grass | No incoming evolution in project table |
| EXPLOUD | Evolution of Johto source | Evolve WHISMUR. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from LOUDRED |
| FARFETCHD | Johto encounter | crystal / johto-route-43 / walk; gold / johto-route-38 / walk; gold / johto-route-39 / walk; heartgold / johto-route-38 / walk; heartgold / johto-route-39 / walk; heartgold / johto-route-47 / walk; heartgold / johto-route-48 / walk; silver / johto-route-38 / walk; silver / johto-route-39 / walk; soulsilver / johto-route-38 / walk; soulsilver / johto-route-39 / walk; soulsilver / johto-route-47 / walk; soulsilver / johto-route-48 / walk | No incoming evolution in project table |
| FARFETCHD_GALARIAN | Later generation/form | Introduced after the Johto games; no GSC/HGSS encounter. | No incoming evolution in project table |
| FERALIGATR | Evolution of Johto source | Evolve TOTODILE. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from CROCONAW |
| FLAAFFY | Johto encounter | gold / johto-route-42 / walk; gold / johto-route-43 / walk; heartgold / johto-route-42 / walk; heartgold / johto-route-43 / walk; silver / johto-route-42 / walk; silver / johto-route-43 / walk; soulsilver / johto-route-42 / walk; soulsilver / johto-route-43 / walk | Evolves from MAREEP |
| GALLADE | Evolution of Johto source | Evolve RALTS. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from KIRLIA |
| GARDEVOIR | Evolution of Johto source | Evolve RALTS. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from KIRLIA |
| GLIGAR | Johto encounter | crystal / johto-route-45 / walk; gold / johto-route-45 / walk; heartgold / johto-route-45 / walk | No incoming evolution in project table |
| GRUMPIG | Evolution of Johto source | Evolve SPOINK. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from SPOINK |
| HARIYAMA | Evolution of Johto source | Evolve MAKUHITA. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from MAKUHITA |
| HO_OH | Johto encounter | crystal / bell-tower / static; gold / bell-tower / static; heartgold / bell-tower / static; silver / bell-tower / static; soulsilver / bell-tower / static | No incoming evolution in project table |
| ILLUMISE | Johto encounter | heartgold / national-park / walk; soulsilver / national-park / walk | No incoming evolution in project table |
| KABUSTAR | Custom species | No official-game counterpart. | No incoming evolution in project table; Cinnabar fossil gift exists |
| KIRLIA | Evolution of Johto source | Evolve RALTS. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from RALTS |
| LILEEP | HGSS Kanto source or revival | Root Fossil can be found in Johto Cliff Cave; revival is in Pewter City, Kanto. | No incoming evolution in project table |
| LINOONE | Johto encounter | heartgold / johto-route-30 / walk; heartgold / johto-route-31 / walk; heartgold / johto-route-32 / walk; heartgold / johto-route-34 / walk; heartgold / johto-route-35 / walk; heartgold / johto-route-42 / walk; heartgold / johto-route-43 / walk; heartgold / johto-route-44 / walk; heartgold / johto-route-45 / walk; heartgold / johto-route-47 / walk; heartgold / mt-silver / walk; heartgold / ruins-of-alph / walk; soulsilver / johto-route-30 / walk; soulsilver / johto-route-31 / walk; soulsilver / johto-route-32 / walk; soulsilver / johto-route-34 / walk; soulsilver / johto-route-35 / walk; soulsilver / johto-route-42 / walk; soulsilver / johto-route-43 / walk; soulsilver / johto-route-44 / walk; soulsilver / johto-route-45 / walk; soulsilver / johto-route-47 / walk; soulsilver / mt-silver / walk; soulsilver / ruins-of-alph / walk | Evolves from ZIGZAGOON |
| LOUDRED | Evolution of Johto source | Evolve WHISMUR. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from WHISMUR |
| LUGIA | Johto encounter | crystal / whirl-islands / static; gold / whirl-islands / static; heartgold / whirl-islands / static; silver / whirl-islands / static; soulsilver / whirl-islands / static | No incoming evolution in project table |
| MAKUHITA | Johto encounter | heartgold / dark-cave / walk; heartgold / ice-path / walk; heartgold / johto-route-47 / walk; heartgold / mt-mortar / walk; heartgold / mt-silver / walk; heartgold / slowpoke-well / walk; heartgold / tohjo-falls / walk; heartgold / union-cave / walk; heartgold / whirl-islands / walk; soulsilver / dark-cave / walk; soulsilver / ice-path / walk; soulsilver / johto-route-47 / walk; soulsilver / mt-mortar / walk; soulsilver / mt-silver / walk; soulsilver / slowpoke-well / walk; soulsilver / tohjo-falls / walk; soulsilver / union-cave / walk; soulsilver / whirl-islands / walk | No incoming evolution in project table |
| MANTINE | Johto encounter | crystal / johto-sea-route-41 / surf; gold / johto-sea-route-41 / surf; heartgold / johto-sea-route-41 / surf | No incoming evolution in project table |
| MAREEP | Johto encounter | gold / johto-route-32 / walk; gold / johto-route-42 / walk; gold / johto-route-43 / walk; heartgold / johto-route-32 / walk; heartgold / johto-route-42 / walk; heartgold / johto-route-43 / walk; heartgold / violet-city / gift-egg; silver / johto-route-32 / walk; silver / johto-route-42 / walk; silver / johto-route-43 / walk; soulsilver / johto-route-32 / walk; soulsilver / johto-route-42 / walk; soulsilver / johto-route-43 / walk; soulsilver / violet-city / gift-egg | No incoming evolution in project table |
| MARILL | Johto encounter | crystal / johto-route-42 / walk; crystal / mt-mortar / surf; crystal / mt-mortar / walk; gold / mt-mortar / surf; gold / mt-mortar / walk; heartgold / mt-mortar / walk; silver / mt-mortar / surf; silver / mt-mortar / walk; soulsilver / mt-mortar / walk | Evolves from AZURILL |
| MEDICHAM | Evolution of Johto source | Evolve MEDITITE. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from MEDITITE |
| MEDITITE | Johto encounter | heartgold / bell-tower / walk; heartgold / burned-tower / walk; heartgold / sprout-tower / walk; soulsilver / bell-tower / walk; soulsilver / burned-tower / walk; soulsilver / sprout-tower / walk | No incoming evolution in project table |
| MEGANIUM | Evolution of Johto source | Evolve CHIKORITA. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from BAYLEEF |
| MILTANK | Johto encounter | crystal / johto-route-38 / walk; crystal / johto-route-39 / walk; gold / johto-route-38 / walk; gold / johto-route-39 / walk; heartgold / johto-route-38 / walk; heartgold / johto-route-39 / walk; heartgold / johto-route-47 / walk; silver / johto-route-38 / walk; silver / johto-route-39 / walk; soulsilver / johto-route-38 / walk; soulsilver / johto-route-39 / walk; soulsilver / johto-route-47 / walk | No incoming evolution in project table |
| NINCADA | Johto encounter | heartgold / national-park / walk; soulsilver / national-park / walk | No incoming evolution in project table |
| NINJASK | Evolution of Johto source | Evolve NINCADA. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from NINCADA |
| OCTILLERY | Evolution of Johto source | Evolve REMORAID. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from REMORAID |
| OMATO | Custom species | No official-game counterpart. | No incoming evolution in project table; Cinnabar fossil gift exists |
| PINSIR | Johto encounter | heartgold / national-park / walk; soulsilver / national-park / walk | No incoming evolution in project table |
| QUILAVA | Evolution of Johto source | Evolve CYNDAQUIL. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from CYNDAQUIL |
| RAIKOU | Johto encounter | crystal / roaming-johto / roaming-grass; gold / roaming-johto / roaming-grass; heartgold / roaming-johto / roaming-grass; silver / roaming-johto / roaming-grass; soulsilver / roaming-johto / roaming-grass | No incoming evolution in project table |
| RALTS | Johto encounter | heartgold / johto-route-34 / walk; soulsilver / johto-route-34 / walk | No incoming evolution in project table |
| REMORAID | Johto encounter | gold / johto-route-44 / good-rod; gold / johto-route-44 / old-rod; gold / johto-route-44 / super-rod; heartgold / johto-route-44 / good-rod; heartgold / johto-route-44 / old-rod; heartgold / johto-route-44 / super-rod; silver / johto-route-44 / good-rod; silver / johto-route-44 / old-rod; silver / johto-route-44 / super-rod; soulsilver / johto-route-44 / good-rod; soulsilver / johto-route-44 / old-rod; soulsilver / johto-route-44 / super-rod | No incoming evolution in project table |
| SCIZOR | Evolution of Johto source | Evolve SCYTHER. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from SCYTHER |
| SCYTHER | Johto encounter | heartgold / national-park / walk; soulsilver / national-park / walk | No incoming evolution in project table |
| SHEDINJA | Evolution of Johto source | Evolve NINCADA. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from NINCADA |
| SKARMORY | Johto encounter | crystal / johto-route-45 / walk; silver / johto-route-45 / walk; soulsilver / johto-route-45 / walk | No incoming evolution in project table |
| SMEARGLE | Johto encounter | crystal / ruins-of-alph / walk; gold / ruins-of-alph / walk; heartgold / ruins-of-alph / walk; silver / ruins-of-alph / walk; soulsilver / ruins-of-alph / walk | No incoming evolution in project table |
| SNUBBULL | Johto encounter | crystal / johto-route-34 / walk; crystal / johto-route-35 / walk; gold / johto-route-38 / walk; heartgold / johto-route-38 / walk; silver / johto-route-38 / walk; soulsilver / johto-route-38 / walk | No incoming evolution in project table |
| SPINDA | Johto encounter | heartgold / bell-tower / walk; heartgold / burned-tower / walk; heartgold / sprout-tower / walk; soulsilver / bell-tower / walk; soulsilver / burned-tower / walk; soulsilver / sprout-tower / walk | No incoming evolution in project table |
| STANTLER | Johto encounter | crystal / johto-route-37 / walk; gold / johto-route-36 / walk; gold / johto-route-37 / walk; heartgold / johto-route-36 / walk; heartgold / johto-route-37 / walk; silver / johto-route-36 / walk; silver / johto-route-37 / walk; soulsilver / johto-route-36 / walk; soulsilver / johto-route-37 / walk | No incoming evolution in project table |
| SUICUNE | Johto encounter | crystal / bell-tower / static; gold / roaming-johto / roaming-grass; silver / roaming-johto / roaming-grass | No incoming evolution in project table |
| SWABLU | Johto encounter | heartgold / johto-route-45 / walk; soulsilver / johto-route-45 / walk | No incoming evolution in project table |
| SWELLOW | Evolution of Johto source | Evolve TAILLOW. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from TAILLOW |
| SWINUB | Johto encounter | crystal / ice-path / walk; gold / ice-path / walk; heartgold / ice-path / walk; silver / ice-path / walk; soulsilver / ice-path / walk | No incoming evolution in project table; Johto line by design; all three grouped in non-native habitat. |
| TAILLOW | Johto encounter | heartgold / cherrygrove-city / headbutt; soulsilver / cherrygrove-city / headbutt | No incoming evolution in project table |
| TAUROS | Johto encounter | crystal / johto-route-38 / walk; crystal / johto-route-39 / walk; gold / johto-route-38 / walk; gold / johto-route-39 / walk; heartgold / johto-route-38 / walk; heartgold / johto-route-39 / walk; heartgold / johto-route-48 / walk; silver / johto-route-38 / walk; silver / johto-route-39 / walk; soulsilver / johto-route-38 / walk; soulsilver / johto-route-39 / walk; soulsilver / johto-route-48 / walk | No incoming evolution in project table |
| TOGEPI | Johto gift | crystal / violet-city / gift-egg; gold / violet-city / gift-egg; heartgold / violet-city / gift-egg; silver / violet-city / gift-egg; soulsilver / violet-city / gift-egg | No incoming evolution in project table |
| TOGETIC | Evolution of Johto source | Evolve TOGEPI. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from TOGEPI |
| TOTODILE | Johto gift | crystal / new-bark-town / gift; gold / new-bark-town / gift; heartgold / new-bark-town / gift; silver / new-bark-town / gift; soulsilver / new-bark-town / gift | No incoming evolution in project table |
| TYPHLOSION | Evolution of Johto source | Evolve CYNDAQUIL. Evolution may require trading or an item; this does not claim a wild encounter. | Evolves from QUILAVA |
| TYROGUE | Johto gift | crystal / johto-route-34 / gift-egg; crystal / mt-mortar / gift; gold / mt-mortar / gift; heartgold / mt-mortar / gift; silver / mt-mortar / gift; soulsilver / mt-mortar / gift | No incoming evolution in project table |
| UNOWN | Johto encounter | crystal / ruins-of-alph / walk; gold / ruins-of-alph / walk; heartgold / ruins-of-alph / walk; silver / ruins-of-alph / walk; soulsilver / ruins-of-alph / walk | No incoming evolution in project table |
| VOLBEAT | Johto encounter | heartgold / national-park / walk; soulsilver / national-park / walk | No incoming evolution in project table |
| WHISCASH | Johto encounter | heartgold / violet-city / good-rod; heartgold / violet-city / old-rod; heartgold / violet-city / super-rod; soulsilver / violet-city / good-rod; soulsilver / violet-city / old-rod; soulsilver / violet-city / super-rod | Evolves from BARBOACH |
| WHISMUR | Johto encounter | heartgold / johto-route-30 / walk; heartgold / johto-route-31 / walk; heartgold / johto-route-32 / walk; heartgold / johto-route-34 / walk; heartgold / johto-route-35 / walk; heartgold / johto-route-42 / walk; heartgold / johto-route-43 / walk; heartgold / johto-route-44 / walk; heartgold / johto-route-45 / walk; heartgold / johto-route-47 / walk; heartgold / mt-silver / walk; heartgold / ruins-of-alph / walk; soulsilver / johto-route-30 / walk; soulsilver / johto-route-31 / walk; soulsilver / johto-route-32 / walk; soulsilver / johto-route-34 / walk; soulsilver / johto-route-35 / walk; soulsilver / johto-route-42 / walk; soulsilver / johto-route-43 / walk; soulsilver / johto-route-44 / walk; soulsilver / johto-route-45 / walk; soulsilver / johto-route-47 / walk; soulsilver / mt-silver / walk; soulsilver / ruins-of-alph / walk | No incoming evolution in project table |
| YANMA | Johto encounter | crystal / johto-route-35 / walk; gold / johto-route-35 / walk; heartgold / johto-route-35 / walk; silver / johto-route-35 / walk; soulsilver / johto-route-35 / walk | No incoming evolution in project table |
| ZIGZAGOON | Johto encounter | heartgold / bell-tower / walk; heartgold / burned-tower / walk; heartgold / sprout-tower / walk; soulsilver / bell-tower / walk; soulsilver / burned-tower / walk; soulsilver / sprout-tower / walk | No incoming evolution in project table |

## Excluded through evolution from Kanto wild Pokémon or gifts

- AIPOM → AMBIPOM
- EEVEE → ESPEON
- EEVEE → FLAREON
- EEVEE → GLACEON
- CLAMPERL → GOREBYSS
- CLAMPERL → HUNTAIL
- EEVEE → JOLTEON
- KABUSTAR → KABUKNIGHT
- EEVEE → LEAFEON
- LICKITUNG → LICKILICKY
- BUNEARY → LOPUNNY
- OMATO → OMATOPS
- PORYGON → PORYGON2
- PORYGON → PORYGON2 → PORYGON_Z
- BUDEW → ROSELIA → ROSERADE
- EEVEE → VAPOREON
