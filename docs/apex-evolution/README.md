# Apex evolution rules

All checks run on level-up while the required move is known. All floors sharing the habitat's map-section ID qualify. Everstone prevents evolution. Moves can be learned elsewhere first.

| Pre-evolution | Apex | Move (learned at 33) | Habitat | Discovery needed |
|---|---|---|---|---|
| Tangela | Tangrowth | AncientPower | Viridian Forest | Encounter Tangrowth |
| Primeape | Annihilape | Rage Fist | Mt. Moon | Encounter Annihilape |
| Mr. Mime | Mime Sr. | Barrier | Diglett's Cave | Encounter Mime Sr. |
| Marowak | Osscythe | Bone Rush | Pokémon Tower | Encounter Osscythe |
| Aipom | Ambipom | Double Hit | Fuchsia Forest | No |
| Lickitung | Lickilicky | Rollout | Cerulean Cave | Defeat or catch Mewtwo |

Ambipom is a potential Kanto Apex with no wild encounter. Mewtwo occupies Lickilicky's potential Cerulean Cave habitat; evolution requires defeating or catching Mewtwo; fleeing or losing does not unlock it. Cerulean Cave and Celadon Cave are distinct habitats.

Piloswine's line belongs to Johto in this design. No Kanto Mamoswine evolution or wild Apex encounter is added.

Double Hit uses the existing two-hit battle effect, Normal type, 35 power per strike, 90 accuracy, 10 PP, contact, and Slam animation per hit. No TM added. Ambipom learns it at 33 as well; Lickilicky shares Lickitung's learnset.

Validation: tools/test_eevee_evolution.py executes the project's evolution function/table with host mocks, including both additional habitat rules, wrong habitats, the four discovery-gated species, and Piloswine remaining without an evolution. Build completed; no live battle test performed.

Mewtwo victory uses a new persistent flag. Older saves that already resolved Mewtwo do not contain reliable victory-versus-escape history, so this flag cannot be inferred safely from the old fought flag.

Trainer exception: Rocket League Champion Giovanni uses Mamoswine; Lorelei uses Piloswine. No other defined trainer team contains an Apex species. This does not add a player evolution route for Mamoswine.
