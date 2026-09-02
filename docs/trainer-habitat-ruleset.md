# Trainer Habitat Ruleset

This is the working ruleset for placing trainers, choosing their teams, and deciding which trainer classes belong in each habitat in Pokemon Expeditions: Kanto.

## Goals

- Every trainer should feel like they belong to the place where the player finds them.
- Trainer class, overworld sprite, front sprite, team, movement, sight range, and dialogue should all point in the same direction.
- Kanto trainers should mostly use Pokemon that make sense for Kanto's local ecology.
- Johto-compatible and trainer-only Pokemon can remain in the game, but should not become ordinary Kanto wild or route-trainer filler.
- Apex Pokemon should feel like discoveries, not ordinary trainer roster picks.

## Core Rules

- Regular trainers use Pokemon from the local habitat, route, dungeon, or a close thematic neighbour.
- Non-native Pokemon are allowed for named specialists, bosses, trades, gifts, Safari-style exceptions, and future Johto compatibility.
- Non-native Pokemon should not appear in ordinary Kanto trainer teams unless the trainer has a clear reason to own one.
- Apex Pokemon should not appear in trainer parties.
- Gym leaders and major bosses can bend habitat rules, but their teams should clearly express their narrative theme.
- Gym leader challenge teams are tests, so they can scale to the player's badge count rather than representing the leader's full true strength.
- Rematches should keep the trainer's name and identity, while letting the team grow stronger or more complete.

## Rematch Rules

- Every non-key Kanto trainer should be VS Seeker rematchable.
- Rematches should reuse the same trainer name as the original battle.
- Rematch teams should stay faithful to the trainer's class, habitat, and dialogue.
- Exceptions that should not receive normal rematches include SS Anne trainers, Saffron Dojo trainers, Silph Co Rocket takeover trainers, Elite Four / league set pieces, one-off scripted bosses, and Apex encounters.

## Class Ladders

- Youngster -> Rich Boy
- Lass -> Lady
- Hiker -> Ruin Maniac
- Bug Catcher -> Bug Maniac
- Biker -> Roughneck
- Aroma Lady -> Pokemon Breeder
- Scout M / Scout F are paired field beginners.
- Black Belt M / Black Belt F are the main fighting trainer pair.
- Expert M / Expert F are martial arts experts above Black Belts, with stronger fighting-type teams.
- Pokemon Ranger M / Pokemon Ranger F are protected-area specialists.
- Triathlete M / Triathlete F use Land, Water, and Cycling variants.
- Electrician replaces Rocker as the electric specialist class.
- Rocket Grunt -> Rocket Admin -> Rocket Ace -> Rocket Champion is the Team Rocket hierarchy.

## Habitat Buckets

### Forest And Field

Likely classes: Bug Catcher, Bug Maniac, Scout M, Scout F, Pokemon Ranger M, Pokemon Ranger F, Aroma Lady, Pokemon Breeder.

Team direction: bug, grass, poison, bird, small mammal, and forest-adjacent Pokemon. Tangrowth belongs here as an Apex encounter, not as a regular trainer Pokemon.

### Mountain, Cave, And Ruins

Likely classes: Hiker, Ruin Maniac, Pokemaniac, Scientist, Black Belt, Expert M, Expert F.

Team direction: rock, ground, cave, ancient, fossil, sturdy, and unusual Pokemon. Experts should appear where martial training feels intentional, such as the Saffron Dojo or Cinnabar Volcano.

### Shore, Sea, And Water Routes

Likely classes: Fisherman, Sailor, Swimmer M, Swimmer F, Tuber M, Tuber F, Triathlete M Water, Triathlete F Water.

Team direction: water, shoreline, fishing, reef, swimmer, and coastal Pokemon. Land and water versions of classes should use the right overworld form.

### Urban Routes And Town Edges

Likely classes: Youngster, Lass, Rich Boy, Lady, Gentleman, Pokefan, Electrician.

Team direction: route-local Pokemon, companion Pokemon, electric Pokemon near infrastructure, and Pokemon that feel owned rather than wild.

### Facilities, Labs, And Mansions

Likely classes: Scientist, Engineer, Burglar, Electrician, Rocket trainers.

Team direction: artificial, experimental, electric, poison, psychic, ghost, stolen, or facility-themed Pokemon. Porygon and Mewtwo story content belongs here thematically, but not as ordinary filler.

### Volcano

Likely classes: Kindler, Scientist, Engineer, Expert M, Expert F.

Team direction: fire, rock, cave, heat, pressure, and endurance-themed Pokemon.

### Cycling Routes

Likely classes: Biker, Roughneck, Triathlete M Cycling, Triathlete F Cycling.

Team direction: rough, fast, poison, fighting, road, and endurance-themed Pokemon.

### Gyms And Dojo

Most gyms should focus on the leader and aide. The Fighting Dojo and field challenge spaces can use extra trainers when the location itself is built around testing the player.

Gym leader teams should communicate their paired theme:

- Brock: fascination with the ancient.
- Misty: beauty of change.
- Surge: training to achieve power.
- Erika: love for family.
- Koga: strength in community.
- Sabrina: peace and emotional control.
- Blaine: advancement and innovation.
- Giovanni: the pursuit and rejection of absolute power.

## Placement Rules

- Trainers should stand on walkable tiles and should not block required navigation.
- Sight ranges should match the trainer's personality and local pathing.
- Movement behaviour should support the scene: watchful trainers can patrol, nervous trainers can look around, and calm specialists can hold position.
- Trainers should have distinct pre-battle, defeat, and post-battle dialogue when they are battle NPCs.
- Rumour NPCs should be placed where their rumour feels discoverable without becoming unavoidable clutter.

## Sprite Rules

- Every trainer class used in-game should have a dedicated overworld, front sprite, and palette unless it is intentionally sharing a paired class identity.
- Shared palettes are encouraged where they save space and preserve the class identity.
- Porymap visibility matters: if a sprite appears in-game, it should also be visible correctly in Porymap.
- Horizontal overworld sheets should remain horizontal where that is the established setup.

## Audit Checklist

When adding or changing a trainer, check:

- Class matches the habitat.
- Team matches the habitat or has a clear special reason.
- No Apex Pokemon are in the roster.
- No removed Pokemon are in the roster.
- Name is unique unless it is a rematch of the same trainer.
- Rematch exists unless the trainer is a planned exception.
- Dialogue matches the class, place, and team.
- Coordinates are walkable.
- Sight range does not make navigation annoying.
- Overworld appears in-game and in Porymap.
- Front sprite, overworld, and palette use the current naming convention.
