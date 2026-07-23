# Pokemon Expeditions: Kanto - Vanilla FireRed Change Log

Last updated: 2026-07-23

This is a living summary of the work done across the Codex Pokemon modding sessions. It tracks the major deltas from vanilla FireRed by system, story beat, map, UI, and asset area. It is meant as a project memory file rather than a patch-by-patch git log.

## Core Direction

- Reframed the game from a linear badge journey into "Pokemon Expeditions: Kanto", with the player acting as Professor Oak's field aide.
- Shifted Oak's framing toward direct observation of Pokemon, people, habitats, friendship, battle, trust, and evolution.
- Made the game less dependent on the vanilla gym order by adding alternate routes and narrative pointers.
- Established three major narrative pillars:
  - Gym leaders as experts in Pokemon growing alongside people.
  - Apex Pokemon as examples of powerful evolution or transformation without trainers.
  - Giovanni and Mewtwo as the throughline about the pursuit of absolute power.
- Built a fourth supporting tool layer around the Logbook, Apex dossiers, and field notes so the player can track discoveries.

## Oak And Early Game

- Rewrote Professor Oak's intro to be shorter and more directly about fieldwork.
- Changed the intro role from a standard Pokemon journey to a LAB assignment and FIELD AIDE record.
- Rewrote Oak's starter/Pokedex/Poke Ball sequence so the player and rival are sent out as field aides.
- Rewrote Oak stopping the player from leaving Pallet without a Pokemon.
- Changed Oak's intro Pokemon from Nidoran F to Nidorino.
- Replaced the intro Nidorino pose with a 3/4 view based on the Gengar/Nidorino sequence.
- Adjusted the intro Nidorino to render behind Oak but in front of Oak's floor circle.
- Added Oak-style messages after Apex-related progress using the repurposed quest/logbook message window.
- Tied Pokemon obedience to field aide progress / Oak subquest completion rather than gym badges.
- Removed gym leader dialogue references to obedience as a badge perk.

## Rival Route And Narrative Guidance

- Removed the vanilla Route 22 rival encounters.
- Removed the S.S. Anne rival encounter.
- Kept Silph Co and Pokemon Tower rival encounters for now.
- Added a Route 2 rival encounter to break the vanilla route assumption.
- Route 2 rival now points the player toward:
  - Viridian Channel and Celadon.
  - Diglett Cave and Vermilion.
  - The Pallet / Route 21 ferry landing toward Cinnabar.
- Route 2 rival now omits route hints the player has already followed.
- Added a Route 11 rival encounter to introduce rumours and Apex Pokemon through Diglett Cave / Vermilion context.
- Added a Celadon rival encounter that replaces the removed Cerulean rival battle and points toward gym leaders as evolution experts.
- Added a Cinnabar rival encounter to point toward Pokemon Mansion and the Giovanni / Mewtwo thread.
- Added an optional Pokemon Mansion 3F rival battle near the jump-down area.
- Reworked rival encounters to use exclamation marks, delays, facing, and walking paths consistently.
- Normalized rival walking speed so he does not zip away outside special cases like credits.
- Updated rival pathing so he avoids clipping through the player and map collision.
- Added step-back behavior when declining optional rival battles.
- Updated rival tone to be more annoying, entitled, and competitive.
- Changed rival references to Oak from "Oak" to "Gramps" where appropriate.

## Giovanni And Mewtwo Story

- Added a Giovanni scene in Pokemon Mansion B1F beside the shattered Mewtwo vat.
- Added a broken Mewtwo vat graphic to the Pokemon Mansion tileset and placed it in the map.
- Made Giovanni face the vat before noticing the player.
- Giovanni's Mansion scene now:
  - Begins with him reflecting on the empty vat.
  - Uses an exclamation mark and sound cue.
  - Has Giovanni acknowledge the player and move beside them.
  - Records the first Mewtwo rumour only after Giovanni leaves.
  - Unlocks the Giovanni/Mewtwo thread in the Logbook.
- Removed Pokemon Mansion B1F gate/switch/statue behavior that no longer fits the scene.
- Added creepy Mansion B1F tinting and Flash-style darkness/cutout behavior.
- Adjusted Flash darkness so it does not cover dialogue, the start radial menu, or NPCs incorrectly.
- Added a Silph Co Giovanni scene that connects Silph's Master Ball technology to Mewtwo control.
- Rewrote Silph Co Giovanni dialogue around control, command, and reclaiming Mewtwo.
- Added President and assistant reactions in the Silph Co confrontation.
- Made the player approach Giovanni only after Giovanni tells them to come forth.
- Added Giovanni's Ambition as a Logbook quest with subquests rather than a single dynamic description.
- Set Team Rocket in Saffron to appear only after the Pokemon Mansion Giovanni scene.
- Planned the later flow:
  - Giovanni fails to get full Master Ball manufacturing control.
  - Giovanni activates or uses a control device.
  - Mewtwo clashes with the Elite Four on Route 7.
  - Mewtwo breaks free and retreats toward Cerulean Cave.
  - Giovanni follows and eventually loses faith in absolute power after the player's later victories.

## Route 7 Mewtwo Aftermath

- Extended Route 7 upward to form a path toward Route 4.
- Added a beam-cut route scar through the forest.
- Added pre-Silph tree coverage so the destruction is hidden until the correct story point.
- Added visibility logic so the beam cut and Elite Four aftermath appear after the Silph/Giovanni progression point.
- Added Lorelei, Bruno, and Agatha as defeated aftermath NPCs on Route 7.
- Moved Lance's aftermath beat into Celadon Cave / Tunnel so his scene sits farther along Mewtwo's escape path.
- Imported geyser-style tiles from Cinnabar Volcano into the Celadon/Route 7 tileset for hot or ruptured ground.
- Added post-flashback dialogue that plays immediately after each flashback, before control returns.
- Made talking to the Elite Four again repeat their post-flashback dialogue rather than replaying the whole flashback.

## Elite Four Flashbacks

- Built a reusable flashback transition pattern:
  - Fade gameplay to white.
  - Hold a full white overlay while repositioning/prepping flashback objects.
  - Fade from white into the flashback.
  - Use grayscale tint for the flashback.
  - Fade back through white and restore gameplay cleanly.
- Fixed repeated issues where the player, Lorelei, or field effects remained invisible or blackened after flashbacks.
- Fixed tall grass field effects showing during the return transition.
- Fixed grass field effects, non-flashback NPCs, and player visibility leaking into or out of flashback transitions.
- Ensured flashback-only NPCs are distinct from real aftermath NPCs.
- Slowed flashback music to 25 percent speed for Lorelei, Bruno, and Agatha flashbacks.
- Removed "LORELEI remembers" style text because grayscale already conveys memory.
- Added effect grayscale handling so flashback visual effects match the scene tint.
- Reused the solid white overlay approach at both the start and end of flashbacks so object repositioning, grayscale changes, and field-effect cleanup are fully hidden.

### Lorelei Flashback

- Shows Giovanni and controlled Mewtwo facing the Elite Four before the beam cut.
- Mewtwo faces upward.
- Giovanni orders Mewtwo to attack the Elite Four, not to clear the path.
- Added looping charge effects:
  - Sparkle.
  - Dust kickup beneath the sparkle.
  - Bespoke Mewtwo flashback beam.
  - Bespoke Mewtwo flashback charge circle.
- Made the beam expand upward from its lower point.
- Made the charge circle draw over the beam.
- Used the small sparkle palette for the beam/circle effects after duplicating the circle as a bespoke asset.
- Tuned white fade timing so the beam attack transitions cleanly.
- Removed extra explanatory dialogue about a sphere of light forming.
- Adjusted Lorelei's current line to "It tore through..." and removed "At first."

### Bruno Flashback

- Places Bruno at 10,44, Mewtwo at 10,46, and Giovanni at 10,47.
- Camera focuses on Mewtwo.
- Beam-cut trees are already removed in this scene.
- Removed Lorelei-style spark/circle effects from Bruno's scene.
- Mewtwo shoves Bruno sideways to 13,44.
- Bruno stays facing down while being forced sideways.
- Movement uses continuous-style movement instead of stop-start tile pauses.
- Flashback ends when Bruno reaches his impact position.
- Added a short delay and a single screen shake on impact.
- Added impact dust and an impact sound cue.
- Removed unused text about Mewtwo raising a hand and the ground splitting.

### Agatha Flashback

- Built around showing Giovanni's control over Mewtwo beginning to wane.
- Added an Agatha confrontation scene with Mewtwo/Giovanni.
- Fixed a lock where the game stopped after Giovanni's "then strike her" line and a screen shake.
- Uses the same flashback transition and music slowdown systems as Lorelei and Bruno.

### Lance Flashback

- Added a Lance flashback inside Celadon Cave / Tunnel.
- Includes Lance as part of the flashback rather than leaving him only as a post-event aftermath NPC.
- Shows Lance already on the ground when the memory begins, implying Mewtwo attacked him before the visible moment.
- Shows Mewtwo going rogue and Giovanni pursuing it north after losing control.
- Uses the shared Elite Four flashback transition plumbing.

## Celadon Cave / Tunnel

- Added a new cave map connected to Route 7 and Route 4.
- Chose a compact 24x26 footprint based on the carved-out cave space.
- Added a south entrance at x:9 and a north exit at x:9.
- Switched the cave to use a Sevii-style cave tileset.
- Added a vertical corridor layout from bottom to top.
- Began adding walls, floor variation, and cave features.
- Added mixed wild encounters from Mt. Moon and Cerulean Cave.
- Added region map / map section work so the cave can have its own identity.
- Added Lance to the cave as the final Elite Four aftermath interaction before Giovanni/Mewtwo continue north.
- Added flashback-only Lance/Mewtwo/Giovanni object handling for the cave scene.
- Connected the cave narratively as the passage Mewtwo tore open between Route 7 and Route 4.

## Apex Pokemon System

- Renamed the encounter concept away from "Legendary" and toward "Apex".
- Converted Tangrowth, Zapdos, Articuno, and Mewtwo to use StartApexBattle naming.
- Updated Apex encounters to use `seteventmon`.
- Added Lugia-style screen shake to Apex encounters.
- Added unique all-caps roar text for Apex Pokemon, with extended vowels.
- Made Apex overworlds animate/walk in place where possible.
- Removed the Ho-Oh encounter.
- Removed the Lugia encounter.
- Converted Mew to a regular wild encounter.
- Removed remaining Legendary encounter plumbing once unused.
- Removed the two Power Plant Electrode encounters.
- Added Apex encounters in:
  - Pokemon Tower last floor for Osscythe.
  - Cinnabar Volcano last floor for Moltres.
  - Diglett Cave lowest floor for Mime Sr.
  - Mt. Moon B2F for Annihilape.
- Added or updated Apex overworld sprites for:
  - Tangrowth.
  - Osscythe.
  - Annihilape.
  - Mime Sr.
  - Mewtwo.
- Treated Ambipom and Lickilicky as Apex-style Pokemon in availability logic.

## Rumour System And Apex Log

- Added a compact helper-based rumour system.
- Hearing the first rumour unlocks the main Apex quest and the relevant subquest.
- Hearing three rumours for one Apex Pokemon reveals that Apex Pokemon in the world.
- Apex Pokemon now appear immediately after the final rumour rather than requiring the player to leave and re-enter the map.
- Added "rumour recorded/updated" feedback after rumour NPC dialogue.
- Added a short delay after the rumour recorded message before returning control.
- Fixed rumour dialogue retriggering because the dialogue box did not close properly.
- Repurposed Fame Checker into the Apex Log / Apex Dossier.
- Removed Fame Checker as a given key item.
- Made Apex dossier NPCs match the actual rumour-giving NPCs.
- Blacked out rumour NPCs before discovery and revealed them fully after hearing their rumour.
- Put the Apex Pokemon silhouette in the top-center dossier slot.
- Made top-left and top-right dossier slots non-selectable.
- Made the Apex slot highlighted immediately when entering the dossier.
- Removed obsolete Apex name/CANCEL window from the dossier.
- Made B return from the dossier to the subquest menu.

### Apex Rumour Themes

- Annihilape: out-of-control emotion.
- Articuno: sudden change.
- Mime Sr: isolated community.
- Osscythe: family and loss.
- Tangrowth: mysticism and urban legend.
- Moltres: renewing natural energy.
- Zapdos: natural infinite power.
- Mewtwo/Giovanni: absolute power and control.

## Logbook / Field Notes

- Repurposed the Quest Log into the Logbook.
- Renamed the menu internally from quest menu to logbook menu.
- Changed the main title from ALL MISSIONS to FIELD NOTES.
- Removed quest log behavior that was no longer wanted while keeping graphic assets for possible reuse.
- Reworked statuses into title case:
  - Active.
  - Done.
  - Traded.
  - Found.
- Changed Completed to Done.
- Added counters for Oak's "Nature of Evolution" subquests.
- Prevented counters such as Evolution by Instinct from displaying beyond their completion cap.
- Added Bond progress sources:
  - Receiving Brock's Golem.
  - Receiving Misty's Politoed.
  - Receiving Surge's Electivire.
  - Budew evolving into Roselia.
  - Golbat evolving into Crobat.
  - Chingling evolving into Chimecho.
  - Receiving Blaine's Porygon2.
  - Receiving Giovanni's Rhyperior.
- Added Design progress sources:
  - Receiving Blaine's Porygon.
  - Reviving fossil combinations.
  - Reviving Aerodactyl from Old Amber.
- Added Gym Leader Trials statuses:
  - In Progress.
  - Done.
  - Traded.
- Added Traded as a green completed-style status.
- Ensured Blaine's trial only becomes Traded when Porygon is received, not merely when Blaine is defeated.
- Removed CANCEL and Back entries from Logbook lists after adding header hints.
- Removed A glyphs from mission titles where they were no longer needed.
- Made A OK disappear on entries that are not selectable.
- Reduced Logbook to a single FIELD NOTES page rather than multiple categories.
- Expanded the Logbook list to five entries.
- Retiled the Logbook to use Bag-style colors, title positioning, hint header, borders, and description box.

## Start / Radial Menu

- Replaced the vanilla start menu list with a radial menu around the player.
- Added radial entries:
  - Pokedex.
  - Pokemon.
  - Bag.
  - Card.
  - Logbook.
  - Help.
- Removed Exit as a selectable entry because pressing the menu button exits the radial menu.
- Added SAVE as a non-selectable top-left label with an L glyph.
- Added SETTINGS as a non-selectable top-right label with an R glyph.
- Added a central label for the selected radial entry.
- Tuned the central label size, width, vertical position, and background.
- Added a Pokeball-shaped backdrop behind the radial icons.
- Animated the backdrop scaling in from zero on menu open.
- Hid icons until the backdrop finishes scaling.
- Added selected-icon bobbing.
- Kept radial icons normal size rather than scaling inactive entries down.
- Fixed repeated map tile corruption caused by menu tile/palette handling.
- Fixed a sprite palette tag collision with weather palettes by moving radial icon tags from 0x1200/0x1201 to 0x1250/0x1251.
- Fixed palette corruption when opening/closing the radial menu in Cinnabar Volcano.
- Added a visible help bar at the bottom while the radial menu is open.
- Added and tuned custom radial icons for:
  - Pokemon.
  - Pokedex.
  - Options/Settings.
  - Logbook.
  - Card.
  - Bag.
- Consolidated radial icon palettes where possible.
- Cleaned unused radial menu assets.

## Shared Menu UI

- Began unifying major menus around a common blue hint header style.
- Menus updated toward shared elements:
  - Bag.
  - Pokedex.
  - Pokemon party.
  - Pokemon summary.
  - Trainer Card.
  - Logbook.
  - Help.
- Added or tuned header hints such as:
  - + PICK.
  - A OK.
  - A FLIP.
  - B BACK.
- Removed redundant Cancel entries where B BACK now communicates exiting.
- Used notches to show page/pocket position where applicable.
- Tuned hint-header colors to match Bag/Pokemon UI colors.
- Cleaned and retiled menu tile assets where duplicate or unused tiles existed.

## Bag Menu

- Expanded the Bag into five pockets:
  - Items.
  - Berries.
  - Poke Balls.
  - TMs.
  - Key Items.
- Added new bag pocket graphics for Berries and TMs.
- Reordered visible pocket layout:
  - Items.
  - Berries.
  - Poke Balls.
  - TMs.
  - Key Items.
- Added a header hint strip with + PICK, A OK, B BACK.
- Removed Cancel entries from each Bag pocket list.
- Moved pocket names into the header.
- Added page/pocket notches in the header.
- Removed left/right arrows near the bag graphic because notches now show pocket navigation.
- Shifted bag graphic and drop shadow to align with the new header layout.
- Fixed bag shadow tiling and mirrored the right-side rounded shadow.
- Changed the footer area into a cleaner single block color.
- Retiled the item list box into a cleaner rectangle.
- Fixed list-box corner and edge tile issues.
- Fixed top option menu overlap when pressing A on a Bag item.
- Fixed TMs causing a reset when entering the teach-to-Pokemon flow.
- Fixed Berries not being usable in battle.
- Modernized berry effects for berries that previously only said they needed grinding into powder.
- Fixed no-effect berries needing a second A press before their action menu appeared.
- Confirmed vitamins remain in-game.
- Fixed pre-Thunder Pass mart menus showing an empty slot.

## Pokedex Menu

- Removed the old Pokedex header strip and replaced it with the shared hint header.
- Moved + PICK, A OK, B BACK into the top hint strip.
- Removed redundant menu text:
  - TABLE OF CONTENTS.
  - POKEDEX title.
  - OTHER category.
  - CLOSE POKEDEX entry.
  - The word "POKEMON" from each habitat category.
- Moved Seen and Owned numbers closer to their labels.
- Adjusted Seen/Owned column positioning.
- Considered but paused splitting habitat categories into two columns.
- Renamed/considered habitat labels for space:
  - Water's Edge to Swamp.
  - Rough Terrain to Plain.
- Removed Sevii map sections from the region map.
- Removed Sevii tiles from the region map graphic.

## Pokemon Party And Summary Menus

- Removed "Choose your Pokemon" box and related tile box.
- Removed bottom-right Cancel button and button tiles.
- Shifted Pokemon party layout to make space for the shared hint header.
- Added the hint header to the party screen.
- Fixed party menu header colors.
- Changed Pokemon summary A behavior:
  - A no longer cancels out on Pokemon Info.
  - B backs out.
- Renamed summary page labels:
  - POKEMON INFO to INFO.
  - POKEMON SKILLS to STATS.
  - KNOWN MOVES to MOVESET.
- Added B BACK hints to summary pages.
- Fixed MOVESET page hint display.
- Moved summary notches and removed extra notches.
- Made Bag notches match the Pokemon summary notch shape.

## Trainer Card

- Removed the Pokedex count entry to make room for the hint header.
- Shifted Name, ID, Money, and related card content to fit the new layout.
- Added hint headers to both front and back card faces.
- Changed front-card Time field to Coins and displayed the current coin amount.
- Changed A behavior:
  - A flips between front and back.
  - B exits the card.
- Kept the A FLIP / B BACK header fixed while the card flips.
- Fixed flickering and duplicate overlay during card flips.
- Fixed front-card header not appearing until A was pressed.
- Fixed back-card tile rows and bottom-card layout issues after shifting.
- Kept the Trainer Card framed as a field aide registration card.

## Help / Teachy TV

- Removed the old L/R help system.
- Removed Help from the title screen.
- Removed map/location help.
- Removed PC storage help.
- Removed field move help.
- Removed help entries for Bag, Pokemon, Pokedex, Logbook, Card, Options, and Save.
- Repurposed Teachy TV as the Help menu.
- Renamed Teachy TV internally to the new Help naming.
- Removed Teachy TV as an item and removed the game-start gift.
- Changed options menu Button Mode into Help.
- Removed L=A as an option.
- Set Sound to Stereo by default.
- Changed the Options menu entry OPTION to HELP and wired it into the Help menu.
- Added Help entries:
  - Catching.
  - Battling.
  - Training.
  - Status Conditions.
  - Type Matchups.
- Removed Cancel/Back entries from Help after adding the shared header.
- Added submenus where appropriate:
  - Demonstration.
  - Theory.
  - Cancel, where still needed in internal submenus.
- Expanded Type Matchups theory to include all types, including Fairy.
- Expanded Battling theory to explain single and double battles.
- Expanded Catching theory to explain Poke Ball choice and catch difficulty.
- Expanded Status Conditions.
- Added Training theory for evolution, moves, stats, and abilities.
- Fixed Type Matchups Theory going into Demonstration.
- Fixed the Help static background palette after cleanup made it red.

## Title Screen And Start Screen

- Centered the title screen logo.
- Removed the black band behind the top of the Pokemon logo.
- Doubled the bottom "Game Freak 2004" bar thickness.
- Changed the bottom bar from red to dark brown.
- Added lighter brown borders/lines to the footer.
- Moved the footer copyright text upward/downward through several passes.
- Replaced Charizard with Tangrowth on the start screen.
- Moved Tangrowth left and behind PRESS START.
- Adjusted Tangrowth tilemap and palette behavior.
- Added custom forest foreground/grass/tree imagery in front of Tangrowth and behind PRESS START.
- Added a Kanto vista backdrop behind the title logo.
- Fixed backdrop tile/palette glitches.
- Added day/night idle transition:
  - Transition toward night around 15 seconds idle.
  - Transition back toward day around 30 seconds idle.
- Made night tint darker and slightly purple.
- Replaced flame animation with Game Freak-style sparkles.
- Tuned sparkle amount, lifetime, drift, vertical spawn area, speed, and layering.
- Made sparkles render behind PRESS START but over the lower black area.
- Changed title/footer/header/logo entrance from sliding to fading.
- Made PRESS START blink between visible and transparent rather than black.

## League Systems

### Indigo League

- Began replacing the vanilla Elite Four flow with a dedicated Indigo League controller.
- Designed Indigo League as:
  - Four random battles from a pool of vanilla Elite Four plus gym leaders with Elite Four-level teams.
  - Rival always as the champion.
  - All teams temporarily set to level 50.
  - Player chooses 4 Pokemon from their party.
  - No EXP from Pokemon defeated.
  - Money awarded after trainer defeat.
- Rewired Bruno's room as the random-four battle room and linked it to the Champion room.
- Removed Elite Four rematches.

### Rocket League

- Built Rocket League using the same league framework.
- Rocket League rules:
  - Four random trainers from Rocket admins and Rocket ace trainers.
  - Singles format.
  - No forced level scaling.
  - No party size selection.
  - EXP is awarded.
  - No money from trainer defeat.
- Added a Rocket League champion room by duplicating the Pokemon League champion room concept.
- Moved Rocket League maps into their own map section.
- Added Rocket League lobby whiteout handling near the Rocket nurse.
- Added ready prompt behavior in the arena.
- Added logic so leaving voluntarily, whiting out, or completing a run resets the multiplier.
- Added post-battle dialogue and exit movement for Rocket League trainers.
- Added Rocket nurse dialogue and Rocket nurse overworld using Rocket Ariana's palette.
- Renamed Rocket aces:
  - DEE to KARDINAL, Psychic team.
  - HONGMIAO to SHION, Fairy team.
  - JAYCE to ATTRYCE, Water team with Slowking.
  - JAKE to MOJAY, Ghost team.
- Ensured Rocket ace teams use only Pokemon in the Pokedex.

## Gym Leaders, Badge Items, And Passes

- Clarified gym leader thematic mirrors with Apex Pokemon:
  - Misty and Articuno: change.
  - Koga and Mime Sr: community.
  - Erika and Osscythe: family.
  - Brock and Tangrowth: ancient/mystic fascination.
  - Sabrina and Annihilape: emotional control.
  - Blaine and Moltres: innovation/progress vs natural renewal.
  - Surge and Zapdos: trained power vs natural power.
  - Giovanni and Mewtwo: absolute power.
- Updated gym leader dialogue structures for consistency between field challenges and gym challenges.
- Made Erika match Koga/Sabrina structure.
- Made Surge match Brock/Misty structure.
- Ensured field challenge badge-mode items are given only at field challenge locations rather than also in gyms.
- Made Rainbow Key only given in Viridian Forest.
- Added/updated Marsh Finder for Sabrina and moved Soul Finder function to Marsh Finder.
- Added Thunder Pass special mart behavior:
  - Marts show BUY, SELL, CANCEL before pass.
  - Marts show BUY, SELL, SPECIALS, CANCEL after pass.
  - SPECIALS include vitamins and battle items.
  - First clerk interaction after earning pass acknowledges it.
- Added Soul Pass nurse behavior:
  - Nurse multichoice becomes HEAL, CHECK FRIENDSHIP, CANCEL.
  - First nurse interaction after earning pass acknowledges it.
- Changed Surge's aide text from "Lightning American" to "Lightning Unovan."
- Fixed "BIRD type" text.
- Blaine no longer gifts/trades a Pokemon directly after defeat.
- Blaine grants access to a machine that generates Porygon.
- Blaine's Porygon flow is intended to feel like receiving the result of a generated/trade-like interaction.
- Hooked Blaine/Porygon into Logbook progress.

## Items And Economy

- Removed Coin Case requirement for collecting coins.
- Removed Coin Case as an item.
- Removed Bike Voucher entirely.
- Made bicycles purchasable from the Bike Shop.
- Added bicycle rental systems to guard houses:
  - Left of Celadon.
  - West/east Fuchsia sides.
  - South of Lavender.
- Rental guards charge money.
- Renting a bicycle automatically walks/warps the player out and mounts them.
- Returning through a guard house walks the player to the guard, returns the bicycle, and exits them on foot.
- Fixed rental bicycle state so the player is not still treated as riding after return.
- Guards let the player through normally if they own a bicycle.
- Fixed east Fuchsia guard house rental trigger and bike mounting behavior.
- Removed Berry Fix.

## World And Map Flow

- Removed or made inaccessible several Sevii-related maps and story areas:
  - Navel Rock.
  - One Island.
  - Three Island.
  - Three Isle Port.
  - Treasure Beach.
- Removed Sevii map sections from the region map.
- Removed S.S. Anne and Saffron Dojo trainers from rematch logic where appropriate.
- Removed Silph Co Rocket rematches.
- Removed Mr. Fuji's Pokemon Tower presence.
- Removed the vanilla Pokemon Tower Marowak encounter.
- Repurposed Route2_East building as Lavender Town's healing house.
- Moved Lavender Pokemon Center interior contents into the healing house except the nurse, then removed the nurse.
- Removed inaccessible Lavender Pokemon Center and Poke Mart maps from active use.
- Added healing houses, including the player house, as whiteout respawn points.
- Added a Route 21 north ferry landing sign readable from both sides:
  - "PALLET TOWN - ROUTE 21 FERRY LANDING."
- Added guard and ferry logic considerations to prevent the player from being stranded if they explore out of vanilla order.
- Added Viridian Channel as a route alternative.
- Added Viridian Channel's own map section.
- Fixed bridge visibility/tiles across Route 2, Viridian Channel, and Route 16 after several tileset compatibility passes.

## Species, Pokedex, Habitats, And Availability

- Reworked habitat lists to support Kanto native, Johto/non-native, safari/gift/trainer-only reserve logic.
- Established non-native habitat as a compatibility bucket for future Expeditions: Johto trading.
- Moved Johto wild/gift/safari species into non-native where appropriate.
- Ensured Pokemon past Mew are not treated as ordinary Kanto wild Pokemon unless deliberately added.
- Added or updated non-native handling for:
  - Tauros.
  - Smeargle.
  - Miltank.
  - Marill, Azurill, Azumarill.
  - Mareep line.
  - Girafarig.
  - Farfetch'd.
  - Ralts line and Gallade.
- Removed Pokemon not wanted in the project, including:
  - Aron.
  - Lairon.
  - Aggron.
  - Hoenn starters.
  - Jirachi.
  - Deoxys.
  - Groudon.
  - Kyogre.
  - Rayquaza.
  - Latios.
  - Latias.
  - Bonsly.
  - Munchlax.
  - Sudowoodo.
  - Snorlax.
- Renamed Thermikarp and Invisibas internally:
  - Cinnabar Magikarp.
  - Cinnabar Feebas.
- Kept their display names as Magikarp and Feebas-style names.
- Made Cinnabar Feebas Water/Dragon.
- Added Gallade.
- Made Gallade male-only evolution.
- Removed Gallade from Kanto mountain habitat and kept it non-native.
- Added Kabuknight evolution from Kabustar at level 40.
- Added Omatops evolution from Omato at level 40.
- Made Aerodactyl obtainable through Old Amber revival.
- Made the full Nido lines obtainable from wild encounters.
- Added Budew as a Viridian Forest wild encounter.
- Added Aipom as a wild encounter.
- Kept Ambipom from normal wild availability because it is treated as Apex.
- Removed Farfetch'd trade obtainability.
- Fixed Aipom habitat/area lookup.
- Moved Plusle and Minun out of non-native because they are treated as Kanto native.
- Moved Chingling/Chimecho, Bronzor/Bronzong, Duskull line, and Shuckle out of non-native as Kanto native.
- Reordered habitat entries for fossil lines around Omato/Omatops/Kabustar/Kabuknight.

## Pokemon Sprites, Icons, Palettes, And Footprints

- Added or replaced front/back sprites and shiny palettes for many custom and Gen 4 Pokemon.
- Added menu icons for many new species.
- Fixed menu icon palette assignments and removed one-off palettes where possible.
- Added official or corrected footprints for Lopunny and Ambipom.
- Fixed footprint slicing/cutting issues.
- Added or updated sprite/icon work for:
  - Annihilape.
  - Electivire.
  - Honchkrow.
  - Magmortar.
  - Magnezone.
  - Mismagius.
  - Rhyperior.
  - Weavile.
  - Combee.
  - Vespiquen.
  - Lickilicky.
  - Osscythe.
  - Starly.
  - Staravia.
  - Staraptor.
  - Happiny.
  - Bronzor.
  - Bronzong.
  - Budew.
  - Buneary.
  - Lopunny.
  - Aipom.
  - Ambipom.
  - Tangrowth.
  - Kricketot.
  - Kricketune.
  - Mime Jr.
  - Mime Sr.
- Shared palettes across evolution lines where appropriate:
  - Buneary/Lopunny.
  - Aipom/Ambipom.
  - Starly/Staravia/Staraptor.
  - Kricketot/Kricketune.
  - Sentret/Furret.
  - Bronzor/Bronzong.
  - Lickitung/Lickilicky.
  - Elekid/Electabuzz/Electivire.
  - Magnemite/Magneton/Magnezone where possible.
- Polished palettes toward a more FireRed-like look:
  - Kricketot/Kricketune warmer earthy reds.
  - Mismagius softer Misdreavus-like pink tips.
  - Weavile softened.
  - Combee/Vespiquen yellow saturation corrected.
  - Bronzor/Bronzong shared palette with Bronzong head fixed.
  - Annihilape checked for vanilla-like palette.
  - Omanyte/Omastar shells shifted toward concept-art beige.
  - Omato/Omatops shells shifted toward the Omanyte/Omastar beige without changing claws.
- Fixed Electivire/Tangrowth back sprite offsets and Tangrowth back palette issues.

## Trainer Classes, Sprites, And Naming

- Established trainer-class naming conventions:
  - Male/female variants use one base class with M/F.
  - Basic/advanced pairs keep separate names.
- Renamed Camper/Picnicker leftovers to Scout M/Scout F.
- Standardized Triathlete names:
  - Triathlete M Land.
  - Triathlete F Land.
  - Triathlete M Water.
  - Triathlete F Water.
  - Triathlete M Cycling.
  - Triathlete F Cycling.
- Added overworlds for all Triathlete variants.
- Added Cycling Triathlete M/F to Route 17.
- Renamed Cooltrainers to Ace Trainers.
- Standardized Battle Girl/Black Belt as Black Belt F/M, with RS variants renamed before later cleanup.
- Renamed Cue Ball concept to Roughneck.
- Set Aroma Lady as basic class and Pokemon Breeder as advanced pairing.
- Added land/water Tuber variants and checked swimmers.
- Removed many deprecated RS trainer front/overworld assets:
  - aroma_lady_rs.
  - beauty_rs.
  - bird_keeper_rs.
  - black_belt_f_rs.
  - black_belt_m_rs.
  - bug_catcher_rs.
  - fisherman_rs.
  - hiker_rs.
  - lass_rs.
  - pokemaniac_rs.
  - pokemon_ranger_f_rs.
  - pokemon_ranger_m_rs.
  - psychic_f_rs.
  - psychic_m_rs.
  - ruin_maniac_rs.
  - scout_f_rs.
  - scout_m_rs.
  - sis_and_bro_rs.
  - sr_and_jr.
  - swimmer_f_rs.
  - swimmer_m_rs.
  - tuber_f_rs.
  - tuber_m_rs.
  - youngster_rs.
- Removed unused front sprites:
  - dragon_tamer_front_pic.png.
  - interviewer_front_pic.png.
- Renamed champion_rival to rival_champion_front_pic.
- Renamed RS front sprite files so `_rs` comes after the species/class name.
- Updated currently placed trainers to use the correct front pics.
- Checked trainer names for uniqueness and rematch consistency.
- Made every non-key Kanto trainer rematchable, with exclusions:
  - S.S. Anne trainers.
  - Saffron Dojo trainers.
  - Silph Co Rocket trainers.
  - Elite Four.
- Fixed trainer placements so trainers are on walkable areas.
- Audited Route 1 and Route 2 trainer placement, movement, and sight ranges.
- Updated Experts as martial arts experts, higher level than Black Belts, with Fighting-type teams/dialogue.
- Added Experts to Saffron Dojo and Cinnabar Volcano.
- Ensured no trainer has Apex Pokemon in their roster.

## Trainer And NPC Overworlds

- Added or updated bespoke trainer overworlds for:
  - Bird Keeper.
  - Biker.
  - Roughneck/Cue Ball.
  - Engineer.
  - Rich Boy.
  - Burglar.
  - Ruin Maniac.
  - Kindler.
  - Hiker.
  - Expert M.
  - Expert F.
  - Black Belt.
  - Scientist.
  - Pokemaniac.
  - Swimmer F on land.
  - Tuber M.
  - Lass.
  - Gentleman.
  - Ariana.
  - Petrel.
  - Rocket Nurse.
  - Mewtwo.
- Fixed repeated overworld sheet issues where only the head/top half rendered.
- Converted vertical overworld sheets to horizontal where needed.
- Fixed Porymap missing new overworld sprites by correcting object event/palette/template references.
- Replaced or cleaned non-trainer NPC overworld names:
  - Removed mg_deliveryman.
  - Added `civ_` prefixes.
  - Renamed fat_man to civ_man_2.
  - Renamed balding_man to civ_man_3.

## Removed Minigames And Features

- Removed Pokemon Jump.
- Removed Dodrio Berry Picking and related minigame desk.
- Removed Berry Crush.
- Removed Mystery Gift.
- Removed most of the old Help System after Teachy TV/Help replacement.
- Removed unused help assets after confirming no remaining references.
- Kept Game Corner slots.

## Technical And Engine Work

- Stubbed `RunHelpSystemCallback` as part of removing the old help system and reducing EWRAM pressure.
- Fixed Bag/TM flow resets.
- Fixed radial menu sprite/palette corruption and weather tag collisions.
- Fixed map tile corruption after repeated start menu open/close.
- Fixed field effect palette/state issues after flashback transitions.
- Added reusable flashback prep/restore helpers after several failed camera/player visibility attempts.
- Added full-white overlay handling for clean flashback transitions.
- Added or updated special functions and event script helpers for:
  - Apex battles.
  - Rumour progression.
  - Immediate Apex reveal.
  - Logbook updates.
  - Flashbacks.
  - Bicycle rental.
  - League flow.
- Built and tested frequently with `make -j4`.
- Used emulator/manual visual checks heavily for UI, palette, and map issues.

## Current Recent Working Area

- Route 7 Elite Four aftermath and flashbacks.
- Celadon Cave / Tunnel creation and Lance flashback.
- Giovanni/Mewtwo story integration.
- Route 7 to Route 4 path of destruction.
- Mewtwo flashback assets:
  - `mewtwo_flashback_beam`.
  - `mewtwo_flashback_charge_circle`.
- Small sparkle palette reuse for flashback effects.
- Celadon Cave wild encounter setup using Mt. Moon and Cerulean Cave-style encounters.
- Reusable white-overlay flashback transitions and cleanup for player/NPC/field-effect state.

## Known Pinned / Remaining Work

- Playtest the full Route 7 to Celadon Cave to Route 4 story sequence in one continuous run.
- Finish the later Giovanni/Mewtwo sequence:
  - Post-Silph retreat.
  - Cerulean Cave confrontation.
  - Mewtwo encounter.
  - Giovanni retreat to Viridian Gym.
  - Viridian Gym resolution and Team Rocket disbanding.
- Finalize any last Celadon Cave/Tunnel map polish after playtesting:
  - Collision.
  - Encounter feel.
  - Exit/entrance framing.
  - Map section behavior.
- Do a final Apex encounter pass:
  - Rumour NPCs.
  - Spawn visibility.
  - Logbook state.
  - Unique roar text.
  - Battle setup.
- Finalize missing trainer overworlds and palette cleanup.
- Finalize trainer front sprite cleanup and class naming consistency.
- Do a complete Pokedex/habitat availability audit after removals.
- Do a final Bag/Pokedex/Pokemon/Card/Logbook/Help UI consistency pass.
- Do a full route progression playtest from all early route choices:
  - Viridian/Pewter path.
  - Viridian Channel/Celadon path.
  - Diglett/Vermilion path.
  - Ferry/Cinnabar path.
- Run a clean build and regression pass before release.
