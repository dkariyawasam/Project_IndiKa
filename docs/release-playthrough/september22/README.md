# Continuous playthrough resumed 22 September

IN PROGRESS. Loaded the 18 September earned campaign save (Pewter Gym, zero badges, Bulbasaur 9/Hoppip 7/Weedle 8) against the main ROM after the evolution-link fixes. Dedicated emulator and save in `/tmp/kanto-release-september22`; no party, inventory, flag or position writes. Movement/battle harness uses button inputs and read-only memory. No rollback for battle outcomes.

[Balance findings for review](balance-review.md) — report-only; no trainer level or team changes.

## Confirmed so far

- Route 3 rival Gym conversation triggers on the approach to Mt. Moon; completed without battle.
- Route 4 Timmy battle won; Bulbasaur learned Vine Whip in place of Growl and reached level 11.
- Kinkabuto wild encounter displays the known question-mark battle sprite. Battle itself completed normally.
- Mt. Moon travel battles reduced Bulbasaur to 3 HP. Four subsequent wild encounters escaped with Retreat, HP unchanged.
- Brock's B1F conversation points to exposed rocks. Found Helix fragment at the nearby rock through normal interaction. Brock accepted it, gave Dart fragment and returned to Pewter Gym.
- Route 4 healing house restored the earned party to full HP.
- Route 3 Greg's level-18 Machop caused Bulbasaur to faint. Weedle poisoned and defeated it; battle won without rollback. Weedle evolved into Kakuna. Party afterward: Bulbasaur 12 (fainted), Hoppip 7, Kakuna 9.
- Pewter Pokémon Centre healed the party.
- Gym aide recognizes Brock's return and advises Water, Grass and Ground moves; dialogue explicitly connects fossil hunting to Brock's return.

Latest earned save checkpoint: healed at Indigo Plateau Pokémon Centre (indigo-arrival-earned-checkpoint.sav). Eight badges, all three rival Apex reflections and Route 23 rival completed, Victory Road traversed after the user-approved removal of Strength puzzles. Both Leagues and remaining Apex/gift-evolution checks remain pending. All losses are retained; balance changes remain report-only.

## Boulder Badge

Brock's double battle won on the first attempt. Bulbasaur finished level 13 at 26 HP; Hoppip fainted and Kakuna replaced it. Badge/Cinnabar Lab access reward delivered. Follow-up explicitly points to ancient waters beneath Rock Tunnel for the next evolution lesson. No injected party or retries.

## Cerulean Gym softlock found

After travelling normally to Cerulean and healing, first Gym entry locked controls indefinitely before the aide dialogue. Read-only runtime state: player (8,16), aide local ID 2 at (6,14), initial (7,14), movement action 0x12 (walk left), flags 0x103 (active, single movement active, frozen). The aide used WALK_LEFT_AND_RIGHT; the entrance script uses lockall, which freezes NPCs immediately. Its following scripted movement could not complete. Changed the aide to FACE_DOWN so her scripted approach starts from the expected stationary position.

Recovery requires reloading the last normal cartridge save (Brock victory), then replaying the travel with ordinary inputs. This is a bug recovery, not a rollback to improve a battle result. The original attempt earned level-15 Bulbasaur with Sleep Powder and reached Cerulean; its outcomes remain in the logs.

## Fix verified and Route 9 recovery

Rebuilt the main ROM, replayed the journey from the normal Brock save, and saved at Cerulean Pokémon Centre. The fixed aide approached, explained Misty's location and released controls normally.

Route 9 Scout Isaac was defeated; Bulbasaur evolved into Ivysaur at level 16. The following Black Belt F battle (Cora; legacy script label Conner) with a level-22 Mankey defeated the three-Pokémon party. Blackout returned to Cerulean Pokémon Centre and restored the party normally; no reset was used to erase the loss. This exposes a party-strength gap on this path, but is not yet evidence for a balance change: the supporting Pokémon are only levels 7 and 9, and the basic battle pilot does not use status tactics intelligently.

Cora's full roster is Mankey 22 and Machop 25. On the normal retry, Leech Seed enabled Ivysaur to defeat Mankey, then Machop knocked it out. Nearby Scout Isaac's Mankey is level 12. Record this local difficulty contrast for balancing review; no trainer data changed.

## Northern route and capture coverage

Nugget Bridge's first trainer clearly introduces the five-trainer challenge. Ivysaur reached level 17 during that battle, but the level-20 Qwilfish defeated the remaining party. Recovery again worked without a reset. The run is now strengthening the supporting party before progressing. Caught Wurmple 8 on Route 5 using Sleep Powder and two Poké Balls; registration, declining a nickname and return to the overworld worked.

Harness limitation: initial damage-only move selection underused Ivysaur's status moves. Updated the button-input pilot to use Leech Seed against healthy non-Grass trainer opponents. This is a testing strategy change, not a ROM or party-state edit.

## Capture experience did not trigger evolution

Live reproduction: caught Minun 7 on Route 5 with Kakuna as the active Pokémon. Capture experience raised Kakuna from level 9 to 10, but after declining Minun's nickname the game returned directly to the map. Read-only party inspection confirmed Kakuna at level 10.

Cause: `FreeResetData_ReturnToOvOrDoEvolutions` allowed evolution checks only for `B_OUTCOME_WON`, despite the project awarding experience on captures. Updated it to also allow `B_OUTCOME_CAUGHT`; losses, escapes and other results still bypass evolution. Main ROM build passed. Saved the earned five-Pokémon party, healed, then reloaded the new ROM. Live post-fix verification is pending.

Post-fix live verification passed: captured Ariados 23 (party reached six), then Wurmple 8. The latter capture raised Kakuna to level 11; Wurmple was sent to BOX1, and Kakuna immediately evolved into Beedrill. Screenshot `capture-evolution-fixed.png` records the success message. All captures and experience were earned through normal inputs.

## Route 9 rematch roster used on first encounter

Ruin Maniac Jovan at (14,22), legacy trainer ID `TRAINER_YOUNGSTER_EDDIE`, uses `sParty_RematchMtMoonRuinManiac` on its first battle: Clefairy 31, Onix 33, Amunyte 35. This is much higher than Scout Isaac 12 and Cora 22/25 on the same route to Misty's trial. The earned party eventually won: Ariados defeated Clefairy but fainted against Onix; Ivysaur finished the battle and reached level 19. Amunyte still shows its known placeholder battle sprite. The user requested that balance findings be recorded for review. Trainer levels and teams remain unchanged.

Cora was defeated on the subsequent attempt: Ivysaur used Leech Seed, then Beedrill finished Machop after Ivysaur fainted. Beedrill reached level 12. The party was then intercepted by Hiker Brice (Geodude 20) on the route toward the Route 10 healing house while three members had already fainted.

## Route 10 reached

Brice defeated on the healed attempt; Ivysaur reached level 20. Route 10 Tuber Dillon also defeated. Reached Route 10 healing house, accepted a rest and confirmed the six-Pokémon party fully healed. Saved normally there (`route10-healing-house-checkpoint.sav`). Entering Rock Tunnel next.

## Rock Tunnel and healing-house dialogue

First Rock Tunnel trainer defeated, with Ariados finishing the Fighting opponents and reaching level 24. On B1F, Ruin Maniac Winston's level-25 Onix knocked Ariados out with a critical Rock attack while Ivysaur was fainted; the remaining low-level party lost. Recovery correctly placed the player at (4,5) directly in front of the Route 10 house healer. However, her first line incorrectly said "MOM" and "Welcome home": the house recovery script reused the mother's text. Added a separate generic house-healer recovery line. Live verification of the new line is pending; the position was verified on the original recovery.

Continued from the house save on the rebuilt ROM. Winston defeated with Ivysaur; the subsequent Fighting trainer defeated with Ariados (level 25, learned Leech Life in place of Constrict). Used the two remaining Potions during the latter battle. Collected the B1F Revive at (2,37) and used it on Ivysaur; item presentation and recovery worked. Next Hiker defeated; Ivysaur reached level 22 and replaced Vine Whip with Razor Leaf. Reached the northwest 1F ladder via B1F (33,3).

Testing controls now support normal forced replacement after fainting, and use Retreat/Run for incidental wild encounters. Live NPC positions are read to avoid navigation stalls when a trainer has moved from its map-template coordinates. No game-state writes are used. After the dialogue-only build, ROM data addresses used by the pilot were refreshed; a stale address briefly caused repeated Sleep Powder selection against Winston's Onix, a harness issue rather than a game issue.

## Misty trial completed

Reached B2F via the northwest 1F ladder and central B1F ladder. Misty's introduction explains the link between underground currents, generators and the Power Plant's surges. Tested the first wrong direction: she returned, explained the looping current and allowed another choice. Correct directions and both generator handoffs worked; the second interaction explicitly explains that she can hear the player through the wall. Final dialogue states that the plant is fixed, asks the player to return to the Gym and introduces the upcoming evolution lesson. Misty departed and controls returned normally. Read-only runtime check of `VAR_MISTY_TRIAL_STATE` confirmed 9 (the compiled SaveBlock1 vars offset is 0x1010; the source comment 0x1000 is stale).

Exited B2F through the Power Plant, collected the Max Potion and reached Route 10 on foot. Returning to the healing house and Cerulean for Misty's Gym follow-up.

## Cascade Badge

Returned to Cerulean normally after healing. The aide recognized Misty’s return and recommended Grass and Electric attacks. Misty’s double battle was won on the first attempt: BEACON (Chinchou 17), PUDDLES (Poliwhirl 20), and TWINKLE (Staryu 23). Ivysaur 25 and Ariados 26 survived. Cascade Badge and Cascade Board were awarded, with instructions for using the board at water. Misty clearly requested Gyarados for her held-item evolution trade; declined for now. Saved normally (`misty-earned-checkpoint.sav`). Two badges earned; the full campaign remains in progress.

## Southern route difficulty

Healed in Cerulean, purchased 12 Great Balls (plus the normal Premier Ball bonus) with earned money. An attempted Super Potion purchase did not complete; the missing stock was discovered during Erika’s third attempt. Route 5’s Bug Catcher and subsequent route trainer were defeated normally. Psychic Preston at the southern junction uses the shared `sParty_GymAdjacentPsychicB`: Drowzee 30, Kadabra 32, Xatu 34. This is another early-route level spike, encountered while travelling toward Vermilion with two badges. No balance data changed.

Preston defeated the party; normal recovery returned to Cerulean and healed it. Continued without resetting. The southern edge of Route 5 permits a route around his sightline to the Underground Path entrance, so this is not a hard progression block. Defeated additional Bug/Picnicker trainers en route; tunnel entry worked. A later party-selection stall was in the test harness: voluntary battle selection uses action 0 as well as action 1. Updated the input handler; no corresponding game change.

## Vermilion and Surge trial

Traversed the Underground Path and Route 6 through normal encounters. Porygon and transformed Ditto rendered correctly in the tunnel battle. Healed at Vermilion. Gym aide correctly approached, directed the player to Diglett’s Cave, returned and released control.

Triggered the rival’s Route 11 scene outside the cave: he names Mime Sr., mentions invisible walls and missing Mr. Mime, and directs the player to compare three local accounts in the Logbook. The reflection emphasizes evolution without a Trainer. No rival battle; departure and control release worked. The cave entrance can be reached from the west before crossing the rival’s trigger tiles; noted as an optional encounter/presentation detail, not a confirmed defect.

Found Surge in the cave. His tutor selection correctly marked only Minun eligible in this party. Minun learned Volt Tackle, Surge explained the move and invited the player to Vermilion Gym, then departed normally.

## Thunder Badge

The returning-Surge aide dialogue explicitly recommends Ground types, explains Electric immunity and Ground effectiveness, warns about paralysis, and states the doubles requirement. Surge defeated on the first attempt using the earned Ivysaur 28/Ariados 27 pair. Thunder Badge and Thunder Pass delivered; follow-up explains expanded Mart stock and Raichu/Electabuzz-with-Electirizer trade. Entered then cancelled the trade selection successfully; no Pokémon exchanged. Saved normally (`surge-earned-checkpoint.sav`). Three badges; the campaign remains in progress.

## Celadon rival and Erika’s forest challenge

Celadon rival battle triggered immediately on the Gym approach and was won. Follow-up clearly contrasts trainer-assisted evolution with wild evolution and explains that Gym teams adjust to badge count. The carried-over September save already has Erika’s forest introduction (trial state 2), so the Celadon aide is correctly absent pending the field challenge; this entrance introduction was not replayed.

Collected all three Mime Sr. accounts, including Chaplin’s post-battle conversation. Logbook feedback states the three-account requirement and permanent consequence of fleeing. The active confirmed Mime Sr. lead and its witness dossier open correctly. Its Apex silhouette remains unrecorded. Traversed Diglett’s Cave normally to Route 2.

Caught Bellsprout 7 in Viridian Forest (second Great Ball, following Sleep Powder). Pokédex registration worked. Took its Energy Root through the party Item menu. Showing the root to Erika triggered the aide’s arrival, type advice, and explicit official-challenge explanation. Erika won the first battle; normal blackout returned to Celadon. Her 3-badge roster is Victreebel 29, Tangela 24, Vileplume 29, Exeggcute 32. The party had poor offensive coverage and suffered sleep/setup; no balance change made.

Deposited Hoppip, Wurmple and Beedrill normally in the PC before the forest task. After the loss, caught Ninetales 27 on Route 8 (Sleep Powder, one Great Ball); registration worked. Healed and moved it to the lead position alongside Ariados. Preparing a retry using Ember/Safeguard; ordinary button inputs only.

## Rainbow Badge

Second attempt lost, but trial state correctly restored the aide in the forest and offered a direct retry. Third attempt won with Ninetales/Ariados: used the collected Max Potion on Ninetales and manually focused both attackers on the Swords Dance Tangela. The pilot previously focused the other enemy and allowed Tangela to build up; these losses are not by themselves evidence for weakening Erika. Read-only battle HP and party-menu HP agreed.

Victory explicitly confirms the witnessed official Gym challenge, awards the Rainbow Badge and Rainbow Key, points to the Celadon friendship reward, and explains Berry Patch access. Erika and aide departed, controls returned. Saved normally (`erika-earned-checkpoint.sav`). Four badges; reward follow-up and later campaign content remain.

## Route 6 trainer blocks the northern opening

After Psychic Lucian’s approach/battle (won), he remained at (9,6), directly south of Route 6’s only traversable opening on row 5, at (9,5). The player at (10,6) could no longer reach Saffron by that route. Screenshot `route6-trainer-chokepoint.png` and live object inspection confirm the position. His original (6,6) location and LOOK_AROUND/range-4 behavior allow this sideways approach. Changed only his movement type to FACE_DOWN so his approach cannot occupy the northern choke. Main build passed; live post-fix verification pending. Took the ordinary Underground Path detour without resetting.

Reloaded the rebuilt ROM from the normal Celadon save; addresses used by the read-only harness were rechecked and unchanged. Walked Route 6 from (10,6) through (11,0) into Saffron successfully. Lucian was already defeated in this save, so this confirms restored traversal, not a fresh battle-approach replay.

Erika’s Celadon gift follow-up passed: received Budew 30 with the standard name; dialogue explicitly says strong friendship plus a daytime level-up evolves it to Roselia. The gift evolution itself remains to be completed in this run.

## Sabrina lead and Route 8

Saffron Gym aide introduction directs the player to Pokémon Tower and releases control. Route 8 Psychic Johan intercepted the crossing. The battle pilot repeatedly selected a disabled Quick Attack; this was a harness limitation, corrected by excluding the current disabled move from its scoring. No game balance or battle mechanics changed.

Johan and the subsequent Route 8 Scout were defeated without a reset. Lavender Healing House restored the party normally. Pokémon Tower rival scene passed: references Osscythe, recalls Mime Sr., proposes habitat as a theory, and directs the player to accounts/Logbook. No rival battle, and departure/control release worked.

Sabrina tutor correctly marked the current six Pokémon ineligible; cancelling provided the explicit Gastly hint and released control. Caught Chingling 6 (one Great Ball), registered it, and sent it to BOX1. Returned normally to Saffron, healed, deposited Minun and withdrew that Chingling, then saved. Existing BOX1 Gengar 100 was observed but not withdrawn or used. Current party: Ninetales 29, Ariados 30, Bellsprout 12, Ivysaur 31, Budew 30, Chingling 6.

Usability review: a full-party player catching a tutor-compatible Pokémon at the tower must return to a PC; Lavender Healing House has none. This is a pacing observation, not a confirmed defect.

## Marsh Badge

Purchased 10 Hyper Potions and 3 Revives at Saffron Mart, with quantity/cost confirmation. Chingling 6 successfully learned Dream Eater from Sabrina. Aide arrival, explicit witness explanation, Bug/Ghost/Dark advice, Dark immunity warning, and challenge prompt all passed.

Won Sabrina’s 4-badge challenge on the first attempt with Ninetales/Ariados, using six Hyper Potions (one on the wrong target due to pilot selection). Used Safeguard and focused the second enemy position. No trainer teams or levels changed. Victory awarded the Marsh Badge, directed the player back to Saffron Gym, removed Sabrina/aide, and released control. Saved normally (`sabrina-earned-checkpoint.sav`). Five badges; gift follow-up and remaining campaign still pending.

Sabrina’s Gym follow-up passed through the normal teleport-pad route. With a full party, her Chingling gift transferred to BOX1 using its standard name; follow-up explains strong friendship and a nighttime level-up. Gift evolution remains pending. The tower Channeler’s Osscythe account was also recorded, with the permanent-flee warning displayed.

## Coastal rental and Route 12 patrol clearance

The Route 12 gate correctly offered a ¥500 rental, explained coastal-route use/return, charged the fee and placed the player outside on the bike. A Route 12 fishing battle was won.

The navigator initially attempted a tile-elevation mismatch at the pier; its movement rules now check elevations. Separately, Gia and Jes patrol the two lanes at x=8 and x=9, y=53–56 and repeatedly blocked forward passage. Moved Gia’s starting x from 9 to 13 on the adjacent pier, preserving her vertical patrol, team and level. Build passed (`/tmp/route12-patrol-clearance-build.log`). Reloaded the normal saved game; map re-entry is required to refresh saved live object positions. Live traversal verification follows below.

After normal map exit/re-entry, live objects confirmed Gia patrolling x=13 and Jes x=8. Traversed the x=9 lane south from (9,53) to (9,59), back north, and south again successfully. Both patrols remain active. Main ROM includes the verified clearance fix.

The run continues south on the coastal rental. Chip, Pia, Cora and Maris battles were won; Ninetales fainted naturally and Ariados carried subsequent fights, reaching level 33. Budew moved to the lead for normal friendship-evolution experience testing. No balance edits.

Vera’s first attempt was lost (Pelipper 31, Starmie 33, Lanturn 35). Budew fainted against Pelipper; the party was already depleted from travel, including a fainted Ninetales and low attack PP on Ariados. Ariados reached 34 and learned Pin Missile over String Shot during the battle, but the remaining party lost. Normal blackout returned to Saffron and healed everyone. No rollback. Retrying with Ivysaur leading and full PP. This outcome is recorded for review, not used to alter balance.

Vera retry won with full PP, Ivysaur leading, and two Hyper Potions. Ivysaur reached 32 and evolved into Venusaur normally after battle. Cleared Route 12 and entered Route 13. The rental was renewed after blackout; the gate explanation and transition worked again.

## Fuchsia checkpoint

Cleared the traversed Route 13/14/15 trainer encounters and reached Fuchsia. Used the remaining two Hyper Potions and the collected Energy Root during this stretch; three Revives remain unused. The party was healed and saved normally (`fuchsia-earned-checkpoint.sav`): Venusaur 34, Ariados 34, Budew 30, Ninetales 30, gifted Chingling 30. Budew and Chingling friendship evolutions remain pending.

Fuchsia Gym’s Janine introduction triggered at the entrance. Her purple-haired sprite rendered correctly, and the dialogue explicitly directs the player to Cinnabar Volcano for Koga’s volcanic-rock task. Screenshot `fuchsia-janine-introduction.png`.

Purchased 20 Super Potions, 10 Full Heals, 10 Ultra Balls and 5 Max Repels in Fuchsia; remaining money ¥5,988. Route 19 shore trainer defeated. The Route 19 ferry is initially hidden by design (source verified); activated the earned Cascade Board from the Bag and entered the water normally, with control restored. Southern traversal is in progress.

## Seafoam crossing

Reached Route 20 by normal surfing; sea-route battles won, Venusaur reached level 35. Entered Seafoam from the east entrance, descended from 1F (28,19) to B2F (25,19), crossed to B2F (17,9), and returned to western 1F (10,6). Exited the western entrance successfully without Strength or boulders. The western witness delivered the lava-to-cold rumour, recorded it, explained the three-account Logbook requirement and permanent flee consequence, then released control. Articuno discovery/battle remains pending.

Reached Cinnabar via Route 20, healed at its Pokémon Centre and saved normally (`cinnabar-earned-checkpoint.sav`). Five badges remain the current count. No trainer balance changes made. Koga’s volcano sequence is next.

Koga’s 1F introduction and Left Corridor 2F checkpoint both progressed and released control. Reached the Right Corridor 3F fountain: Janine arrived, the three characters freed the lavic stone, Koga explained her role as official witness, and Janine provided Psychic/Ground/Flying-immunity/antidote advice. Accepted the field battle at five badges; opponent opening pair Crobat 38 and Arbok 39, player Venusaur 36 and Ariados 34. Battle result pending.

Koga first attempt lost after four Super Potions. Crobat defeated; remaining observed opponents Muk 40 and Arbok 39. Normal blackout returned to Cinnabar Pokémon Centre. No rollback. Koga reward/return-to-gym transition is still unverified in this continuous run; the trial cutscene and witness advice were verified before the loss. See balance review for party/controller limitations.

## Resumed after Bronzong outline repair

Reloaded the rebuilt main ROM from the earned post-Koga-loss save; no progress rollback. Cinnabar rival chat played and directed attention to the Mansion laboratory. Navigated to Mansion 3F, operated its statue switch normally, and defeated Scientist Lenox. Rival No response played its refusal/retreat scene; the navigation helper immediately re-approached afterward, so this was stopped before accepting Yes. The accepted rival battle was lost to opening Pidgeot 36. Normal blackout healed the party at Cinnabar. This party has substantial Flying weaknesses and insufficient coverage; boss retries are paused for normal capture/preparation.

Wording review: Mansion rival asks whether the player is ready or turning back, but does not explicitly state that Yes starts a battle. No dialogue or balance edits made in this continuation.

Cinnabar Route 21 ferry → Seafoam arrived at Route 20 and released control normally. Entered Seafoam east entrance. Moved gifted Chingling 30 to lead using the party menu and used purchased Max Repels to filter low-level encounters while searching for Jynx 30–33. First two Repel durations produced no Jynx; third search underway. No encounter-table/RNG/save-memory edits.

Found Jynx 30 during the fifth purchased Max Repel. Used Chingling’s Yawn, one Super Potion to keep it healthy through the sleep setup, and one Ultra Ball; caught successfully into the sixth party slot at full HP. Five Max Repels spent, 15 Super Potions and 9 Ultra Balls remain. Captured using normal menu inputs; no encounter/party/level changes injected. Returning to Cinnabar to heal and retry with Ice coverage.

Returned from Seafoam by ferry, healed, put the earned Jynx in the lead and saved (`jynx-earned-checkpoint.sav`). Mansion statue switches remained consistent on re-entry; toggled the 2F and 3F statues normally to reach the rival again.

Mansion rival retry won: Pidgeot 36, Raticate 34, Kadabra 35, Charizard 38. Jynx’s Lovely Kiss/Ice Punch handled Pidgeot; Venusaur used Sleep Powder/Leech Seed/Razor Leaf against the middle opponents; Ninetales finished seeded Charizard with Confuse Ray/Quick Attack. Four Super Potions used, no party members fainted, no Revives used. Venusaur reached 37. Post-battle dialogue points below and the rival departure is being checked. Manual menu control was used because the older pilot could race menu inputs; that helper issue is not classified as a ROM defect.

Mansion rival departure released control. Took the 3F drop into 1F and descended normally to B1F. Scientist Ted (including Electrode 29 and Weezing) and Burglar Lewis (Growlithe/Ponyta 34) were defeated; Jynx fainted in Ted’s battle and Ariados in Lewis’s. No extra healing items used in those battles. Read the September 1 diary from the north side of its table; its Mewtwo-power warning displayed correctly.

Giovanni basement scene passed from the x=7 trigger: overheard perfect-power speech, movement beside the player, explicit Silph/Saffron/Master Ball lead, departure and control release. Mewtwo rumour recorded with three-account explanation and permanent-flee warning. Five badges remain; normal save checkpoint follows. The darkness limits distant visibility but no palette corruption was visible in the inspected scene.

Saved normally after Giovanni (`mansion-giovanni-earned-checkpoint.sav`), returned through Mansion and healed at Cinnabar Pokémon Centre. Cinnabar→Pallet ferry worked, landing at Route 21 North with control restored. Northern landing trainer defeated, then crossed into Pallet normally. Following the explicit Saffron lead overland; no flag/position edits.

Reached Saffron through Pallet, Route 1, Viridian, Route 2, the full Diglett’s Cave crossing, Vermilion and Route 6. No traversal lock found; Arena Trap Diglett encounters were fought normally. Healed in Saffron and moved gifted Chingling to lead. Silph’s 5F elevator selection and 5F↔9F teleporter worked; Rocket takeover was visible. Defeated the Card Key corridor Grunt, obtained Card Key normally. Chingling reached 31 but did not evolve: read-only inspection subsequently showed night and friendship 218, below 220. This is not classified as an evolution defect.

Found a dialogue consistency issue in live Silph 9F: the female Rocket still introduced herself as one of the four “ROCKET BROTHERS.” Updated the linked 6F/7F/8F/9F group references to “ROCKET SIBLINGS,” preserving all battle data and intent. Main ROM build passed (`/tmp/silph-rocket-siblings-build.log`); dedicated emulator reload/live verification pending next normal save.

Silph 9F rest spot healed all six Pokémon and released control. Saved normally (`silph-card-key-earned-checkpoint.sav`), reloaded the rebuilt main ROM, and verified the 9F Rocket’s corrected “My siblings will avenge me!” follow-up live. Main/test ROM symbols used by the harness remained unchanged. Current healed party at that save: Chingling 31, Ariados 35, Budew 30, Ninetales 30, Venusaur 39, Jynx 31. Card Key retained and previously opened door stayed open. Continuing through 3F toward the rival.

Silph 3F Grunt defeated; opened the 3F Card Key door and used the teleporter to 7F. Rival was present after the Mansion prerequisites. Declined his challenge: player stepped back to (3,4), rival returned to (0,7), control released. Re-approached and accepted. Won the five-Pokémon fight on first attempt: Pidgeot 37, Exeggcute 38, Alakazam 35, Charizard 40, Gyarados 35 (observed send-out order). Three Super Potions and two Full Heals used; no Revives. Ariados fainted; Chingling entered already fainted. Ninetales reached 31, Venusaur 40. Post-battle dialogue reinforces the control/Master Ball motive; rival left and released control. 7F→11F teleporter works.

Clarified both Mansion and Silph Yes/No prompts to explicitly say “battle me”; build passed (`/tmp/rival-battle-prompts-build.log`), live ROM reload pending next save. The old Lapras-gift script has no active employee object on 7F, so no Lapras gift is counted as obtainable in this run.

Harness corrections: disabled Leech Seed was still selected by a support-move override; fixed that helper condition and resumed normally. The pilot also accepted Fury Swipes over Ariados’s first move at level 35, reducing its coverage; this is a run limitation, not a ROM defect or a deliberate balance change.

Defeated the final 11F guard, opened the boardroom door, returned to 9F rest spot and healed all six. Saved (`silph-rival-earned-checkpoint.sav`) and reloaded the current main ROM containing the explicit rival battle prompts. The rebuild moved gBattleMoves/gTypeEffectiveness by 16 bytes; updated the read-only battle helper to the verified ELF addresses before continuing. Rival stayed gone and doors stayed open across reload. Giovanni’s scene began from the left trigger with full HP: Venusaur 40, Ariados 35, Budew 30, Ninetales 31, Jynx 31, Chingling 31. Battle result pending.

Silph Giovanni won on the first attempt: Persian 40, Kangaskhan 41, Nidoqueen 42, Nidoking 42, Rhydon 43, Gyarados 43. Venusaur alone fought, reaching 42; no items, switches or faints. Declined Growth at 41 through the normal move-learning prompt. Sleep Powder/Leech Seed/Razor Leaf succeeded with favourable sleep durations and critical hits. Giovanni explicitly named Route 7, departed with the Rockets, and released control. President gave the Master Ball and updated the Mewtwo rumour; his dialogue distinguishes capture from trust. Exited by the normal teleporter/elevator route, healed at Saffron and saved silph-giovanni-earned-checkpoint.sav.

Arrived on Route 7 from Saffron after the signal event. Lorelei appeared at the southern aftermath site; her complete flashback dialogue played and returned control with ordinary field colours restored. Continuing north through the witnesses.

Route 7 Bruno and Agatha flashbacks completed and restored control. Agatha explicitly directs the player to Lance in the cave opened by the blast. Entered Celadon Cave from Route 7, watched Lance’s flashback (grayscale scene observed), returned to normal colours/control, then talked to him again: short Cerulean Cave lead only, no restart. Exited north to Route 4 and reached Cerulean Cave using the Cascade Board normally.

Found and fixed a Nugget Bridge reward defect: walked past three moving trainers, defeated Cale and Nadia, and the host nonetheless congratulated the player for five victories. Both host interaction and approach triggers now require all five trainer defeat flags, otherwise explaining that trainers beside the bridge must also be challenged. Build passed (/tmp/nugget-bridge-reward-build.log). Existing run received the premature Nugget because the old script was already active; retained that outcome, no rollback. Fresh unclaimed-reward live verification remains pending. Saved normally at Cerulean Cave entrance (cerulean-cave-entry-earned-checkpoint.sav); current main reload follows. Venusaur is 43.

Cerulean Cave 2F: picked up the Full Restore in the entry corridor, then defeated Ruin Maniac Foster. Live movement confirmed his original (27,16) position still blocks the one-tile corridor after defeat; no side tile bypass exists. Moved him to the nearby two-by-two landing at (22,9), facing down, preserving his trainer/team data. Build/live return traversal pending. The navigation helper also retained removed item objects as obstacles; corrected its live-object occupancy refresh (harness issue, not ROM issue).

Foster relocation build passed and live traversal passed after saving on 1F and reloading the main ROM: returned to 2F, walked through the former blocked (27,16) corridor and reached the next ladder. Continued through the ordinary ladder maze into B1F without injected movement or flags.

Mewtwo capture branch passed: Giovanni’s approach scene explicitly explains his failed control and releases the path. Readiness prompt displays level 70 and permanent loss/flee consequences; chose No and verified Mewtwo remains with control released. Re-interacted, accepted, used the earned Master Ball, captured Mewtwo 70 into Box 1 (full party), declined nickname, and registered its Pokédex entry. Venusaur gained 2,200 capture EXP. Oak’s aftermath played, Mewtwo disappeared, Giovanni explained his change of view and explicitly named Viridian Gym, then departed and released control. Saved normally (mewtwo-earned-checkpoint.sav). Kept Mewtwo in the PC so later Gym balance checks use the existing earned party. This verifies capture/story progression, not a fair level-70 fight or the alternate flee/loss branches.

Returned from Cerulean Cave normally, healed in Cerulean, and bought 10 Super Potions, 3 Escape Ropes and 15 Repels (remaining money 10,012). The premature bridge Nugget was not sold. Routes 4→3→Pewter→2→Viridian are traversable westbound. Defeated Scout Nancy on Route 2 en route. Viridian Gym opens after the Mewtwo resolution; aide delivers Water/Grass/Ice advice and Ground immunity to Electric. Party changed through the menu to lead Venusaur 43 and Chingling 31, with Chingling’s Levitate useful against Ground moves. Gym challenge pending.

Viridian Gym first attempt lost to the opening Sandslash (SPADE) 47 and Dugtrio (TRIO) 47. Team: Venusaur 43, Chingling 31, Budew 30, Ninetales 31, Jynx 31, Ariados 35. Used the found Full Restore, no other healing items. Sandslash used Swords Dance/Slash, Dugtrio Slash/Earthquake; Giovanni used a Hyper Potion. Three Sleep Powder misses, a critical Slash and a late targeting error contributed. Normal blackout returned to Cerulean; paid 2,752, no rollback. Retry deferred until earlier unfinished Gym checks and party preparation. Gym path works; navigation helper needed to avoid blindly walking onto arrow tiles (not a ROM defect).

After the Viridian loss, normal Centre recovery healed the party. Chingling friendship was 223 by read-only inspection (above 220); Budew 232. Led with Chingling and defeated the previously bypassed Swimmer Nell on Nugget Bridge, reaching level 32 at 39/79 HP with no items. Level-up occurred during daytime, so no Chimecho evolution is expected yet. Returning to heal before the remaining bridge trainers.

Remaining bridge encounters completed: Kindra defeated by Chingling, Becky (Pelipper 31, Starmie 33, Lanturn 35) defeated with Chingling/Venusaur. No items or faints in Becky’s battle. Chingling used Yawn/Confusion against Pelipper, then Yawn against Starmie before switching to Venusaur. Venusaur overcame Confuse Ray and finished Starmie; critical Razor Leaf knocked out Lanturn. Chingling reached 33 and Venusaur 44. Night friendship evolution succeeded: Chingling → Chimecho, offered Double-Edge, declined it normally to retain support moves, returned to the overworld.

Live visual mismatch found: Scout Kindra was using Swimmer F’s overworld despite Scout dialogue and battle portrait. Changed only her object graphics to existing OBJ_EVENT_GFX_SCOUT_F; four dynamic palette tags in the bridge group, with Triathlete using the standard blue slot. Build passed (/tmp/route24-kindra-sprite-build.log); live reload/appearance check follows the next earned save.

Saved healed Chimecho party normally (chimecho-earned-checkpoint.sav), loaded the current main ROM, and revisited Route 24. Kindra now has the green Scout overworld; nearby Swimmer, Tuber and Fisher remained correctly coloured in the inspected crowded night view (route24-kindra-palette-check.png). Main ROM and dedicated test ROM hashes match. New bridge reward gating is built but still requires a separate fresh unclaimed-reward scenario; this save had already received the old premature reward.

Koga second attempt lost after two Super Potions, both used on Chimecho. Full-health party on entry: Chimecho 33, Venusaur 44, Budew 30, Ninetales 31, Jynx 31, Ariados 35. Defeated Arbok 39 and Crobat 38; Muk 40 and Weezing 41 remained, Koffing not reached. Retry recognised the completed stone trial and repeated Janine’s Psychic/Ground advice without replaying her arrival. Normal blackout returned to Cerulean and healed the party; no rollback. This party needs stronger supporting moves/levels before another attempt.

After recording both Koga losses, defeated Breeder Tess on Route 25 normally with Chimecho. To continue story verification without a long grinding detour, withdrew the legitimately caught Mewtwo 70 and deposited Ariados using the PC. The pre-existing Gengar 100 remains unused. Subsequent Mewtwo-assisted wins test progression/dialogue, not normal-level balance. Healed and saved at Cinnabar (koga-mewtwo-party-earned-checkpoint.sav).

Fixed Koga/Janine retry positions: on re-entry after the stone trial, they reverted to their original map coordinates, while the departure smoke uses their post-trial positions. The transition script now restores Koga (4,15) and Janine (5,15) only after the stone is pulled. Main build passed (/tmp/koga-retry-positions-build.log), then reloaded from the earned Cinnabar save. Live return confirmed both corrected positions and accessible retry dialogue (koga-retry-positions.png). Main/test ROM hashes match.

Koga third attempt won with earned Mewtwo 70 / Venusaur 44 leading; no items or faints. All five opponents cleared. This is a story verification result, not normal-level balance evidence. Soul Badge (sixth badge) and Soul Pass awarded; dialogue explicitly sends the player to Fuchsia Gym. Captured departure smoke over the corrected Koga/Janine positions (koga-departure-smoke.png), both disappear and controls release. Saved normally (koga-victory-earned-checkpoint.sav).

Blaine defeated at six badges with Mewtwo 70 / Venusaur 44→45 leading; Venusaur fainted and Chimecho replaced it. No battle items used. Seven badges now. Aide advised Water and Burn Heals; Blaine’s dialogue connects Fire to invention and explicitly directs the player to his Porygon machine. Incorrect generator answer (allow random noise) rejected cleanly, released controls, and permitted retry. Correct Yes/No/Yes parameters generated Porygon 26; full party sent it to Box 1. The input helper entered nickname mode because its stop text did not match; accepted a blank input to retain PORYGON, no rename. Repeat interaction says the pattern is archived and gives no further Pokémon (porygon-generator-repeat.png).

Found a general multichoice input bug while taking the Cinnabar ferry: A closing the final question page immediately selected PALLET TOWN, so the destination menu never remained visible. The same B press closed the question and cancelled the menu. The normal multichoice handler had a zero-frame opening delay; added a two-frame input guard so it cannot reuse the opening frame’s input. Main build passed (/tmp/multichoice-opening-input-build.log), reloaded from a normal dock save (ferry-menu-earned-checkpoint.sav). Live verified visible destination list, B cancellation/control release, reopening, and selecting SEAFOAM ISLAND (ferry-choice-menu-fixed.png). The accidental Pallet trip was retained and sailed back normally.

After the multichoice fix, selected Seafoam Island and arrived there normally; selected Fuchsia City from the second ferry menu and arrived at Route 19 with control released. Both non-default destination choices passed live.

Fuchsia Gym follow-up passed: Janine is present with her intended sprite, acknowledges witnessing the trial, and directs the player to Koga. Koga gives Golbat 30, sent to Box 1 because the party is full; declined nickname normally. Read-only boxed-data verification confirms Golbat friendship 200 and 27,000 EXP (level 30), and Porygon 17,576 EXP (level 26). Koga explicitly explains friendship plus level-up, then repeats the Crobat/Pokédex completion reminder without giving another Golbat. Gift evolution remains pending.

Saved healed seven-badge party at Fuchsia (koga-golbat-earned-checkpoint.sav). Route 18 Triathlete Rina defeated en route west. Route 18 rental guard correctly offered the 500-fee bike rental, accepted Yes, explained return conditions, and placed the player outside riding normally. Continuing north via Cycling Road.

Cycling Road northbound completed with the earned Mewtwo: Wilton, Orson, Toby and Dax defeated; no balance conclusion from these assisted wins. Found stale VAR_RESULT branches following checkflag in all four bike-rental gates. Replaced them with direct flag branches; main build passed. Live Route 16 verification: rented once for 500, re-entered and spoke to the guard while renting (correct active-rental reminder, no new fee prompt), then exited east. Return dialogue and walk to the guard played, bike was removed, player warped outside, and controls released. Saved normally (bike-return-earned-checkpoint.sav). Seven badges; Giovanni remains the final Gym.

Giovanni Gym retry won with earned Mewtwo 70→71; Venusaur 45 fainted, Chimecho 33→34 replaced it. No battle items used. All six opponents cleared, 20,000 prize money, eighth badge awarded. Introduction references ruins, industry and Mewtwo; conclusion connects control versus trust and Team Rocket’s new direction. Explicit next-route guidance: west from Viridian to Route 22, north through Route 23 and Victory Road to Indigo. Giovanni disappears and controls release. Saved normally (eight-badges-earned-checkpoint.sav). This remains assisted story verification, not normal-level balance validation.

Viridian Forest rival reflection completed as the third Apex chat (Route 11 and Tower already recorded). It names MIME SR., OSSCYTHE and TANGROWTH together, explains the habitat hypothesis, contrasts it with training, and mentions sharing notes with Oak. No battle; rival disappears and controls release. Tangrowth itself is not currently revealed in the clearing; its local-account investigation remains pending.

All eight Route 22/23 badge checks passed, including Cascade Board field animation and water crossing. Route 23 rival first attempt lost from travel attrition (recorded in balance review), normal recovery/save retained. Returned with full health and PP; rival still present and retry works. Second attempt won with earned Mewtwo 71→72, no items or faints. Follow-up references the first lab battle and sends the player through Victory Road to the League. Rival disappears, controls release. Saved at Victory Road entrance (route23-rival-victory-earned-checkpoint.sav).

Victory Road’s entrance still required Strength (TM54, 3,500 Game Corner coins). User chose to remove that requirement. Removed ten pushable-boulder objects, four switch triggers/tiles and their barrier scripts across three floors; passages are open on map load, and Route 23 no longer resets the removed puzzles. Main build passed (/tmp/victory-road-no-strength-build.log), reloaded the earned cave save. Live traversed 1F→2F→3F, walked the former 3F barrier (victory-road-3f-open-path.png), returned through the exit stairs and reached Indigo Plateau. No Strength move or field flag used. Defeated Naomi, Rolando, Daisuke and Nelson en route; Mewtwo/Venusaur used normal party switching. Two Super Potions used on Mewtwo. Healed and saved normally at Indigo; main/test ROM match.

Indigo League first run: briefing explains doubles, both sides at 50, original levels restored, full recovery, no Bag items, and reset on leaving/loss. Arena decline steps back/release works; north door denies entry before four wins. Misty then Sabrina defeated; each records correct win count and restores original party levels/order/full HP. Bag attempt blocked with “Items can’t be used now.” Bruno defeated this party in round three; normal blackout to Indigo, 6,000 loss payment, original levels and full HP restored. Saved normally (indigo-first-run-loss-earned-checkpoint.sav). Perish Song switching error contributed to the Misty struggle; wins/losses are functional checks, not a controlled balance assessment.

Indigo second run: Surge defeated, lost to Brock in round two. Third run: lost to Lorelei in round one. Both normal 6,000-loss blackouts retained. Terminal records live verification shows three battle victories, three defeats, zero championships and zero streak (indigo-records-three-runs.png). Current challenge correctly says no active challenge, last run 0/4, ended in defeat. Improved only the button-input pilot’s doubles target selection after it repeatedly ignored dangerous opponents on one side. Game balance untouched. Team preparation through earned Pokémon is next before another completion attempt.

Withdrew Koga’s Golbat 30 through the normal PC interface, swapping Budew into Box 1 slot 10. The old test Gengar was untouched. Healed and saved at Indigo (indigo-golbat-earned-checkpoint.sav). Remaining League completion and Apex encounters are still pending.
