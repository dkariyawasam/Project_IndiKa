# Playthrough resumed 30 September

Resumed using the current main ROM and a copy of the newest preserved cartridge save, `../september22/indigo-second-run-loss-earned-checkpoint.sav`. The later Golbat withdrawal mentioned in the journal was not present in the preserved save set. The original player save is untouched. Temporary emulator files had been cleared and were reconstructed.

The isolated CampaignPlaytest emulator uses `/tmp/kanto-release-september22/campaign.gba`. Refreshed the button-input harness addresses from the current ELF. No party, inventory, position or story-flag writes; losses are retained. Balance findings remain report-only.

Confirmed so far:
- Save loaded at Indigo Plateau with eight badges and healed earned party: Mewtwo 72, Venusaur 47, Budew 30, Ninetales 31, Jynx 31, Chimecho 34.
- Entered League arena through the north door; readiness prompt and battle transition worked.
- First resumed opponent is Lorelei, opening with Cloyster and Dewgong. All six player Pokémon normalised to level 50 with full HP.

Run remains in progress; no League completion claimed.

Lorelei attempt ended in defeat. Reached Cloyster, Dewgong, Lapras, Weavile and Jynx; not all six opponent slots were independently observed. No Bag items or injected party changes. The automatic input helper uses a simple attack heuristic, so this loss is not a controlled balance verdict. Budew remains an unevolved support member with poor attacking options; preparation is needed before another League completion attempt.

Normal blackout returned to Indigo Centre and restored all original levels and full HP: Mewtwo 72 (259), Venusaur 47 (143), Budew 30 (72), Ninetales 31 (95), Jynx 31 (87), Chimecho 34 (97). Field control returned. Opened the radial menu, selected L SAVE, confirmed Yes and returned to the field. Preserved the resulting cartridge save as `indigo-lorelei-loss-earned-checkpoint.sav`; original player save remains untouched.

League completion, remaining Apex encounters and final rival convergence are still pending. No game balance or game source changes were made in this resumed segment. Updated read-only/button-input harness is preserved under `harness/`; symbol addresses apply to this ROM build only.

## Golbat preparation and subsequent run

Used Someone's PC → Move Pokémon to replace Budew with Koga's earned Golbat 30; Budew was stored in Golbat's former Box 1 slot. The unrelated level-100 Gengar in storage was not used. Saved normally before entering; checkpoint `indigo-golbat-preparation-earned-checkpoint.sav`.

Read-only party move check: Mewtwo Swift/Recover/Safeguard/Psychic; Venusaur Sleep Powder/Razor Leaf/Retreat/Leech Seed; Golbat Supersonic/Bite/Wing Attack/Confuse Ray; Ninetales Ember/Quick Attack/Confuse Ray/Safeguard; Jynx Lovely Kiss/Powder Snow/Double Slap/Ice Punch; Chimecho Confusion/Uproar/Yawn/Psywave. Previous commentary mentioning Shadow Ball was incorrect: Mewtwo knows Psychic. The input helper now scores both enemy targets and uses Recover below half health; this changes only normal battle choices, not ROM balance or save data.

Won round one against Giovanni. Victory 1 of 4 message displayed; all six original levels and full HP restored (including Golbat 30, 85/85). Proceeded without leaving the arena. Round two opponent is Erika. Voluntary Venusaur → Golbat switch passed through the party menu, but Golbat was knocked out quickly. Mewtwo became paralysed; reserve move quality remains a preparation concern. Result follows below.

Erika defeated; victory 2 of 4 displayed. Ninetales and Chimecho cleared the late opponents after Mewtwo, Venusaur and Golbat fainted. This illustrates why the early struggle should not be treated as a final balance verdict. Continued same earned run; no resets or battle items.

Misty defeated; victory 3 of 4 displayed. Tested Perish Song voluntary switching through ordinary inputs. The helper initially tried a duplicate reserve and received the expected already-selected rejection; corrected its choices to distinct Ninetales/Jynx replacements. No ROM defect or state injection. Continued same run.

Fourth round was Lorelei and ended in defeat: this run finished at 3/4 pool victories (Giovanni, Erika, Misty). Cloyster, Dewgong and Weavile were defeated; reached Lapras/Piloswine before the remaining party fell. Jynx was observed in the preceding attempt, so all six intended Lorelei species were seen across the two attempts. Normal blackout restored Mewtwo to 72 immediately. No championship/Champion-door unlock is claimed.

Preparation finding: replacing Budew improves the roster, but the party still needs stronger coverage/support before the next attempt, particularly Ninetales retaining Ember and Chimecho retaining Confusion. The input helper is a limited heuristic, so this is not proof that trainer balance needs changing. No balance changes made.

Verified full recovery and original levels for all six after the loss, with Golbat still in the party. Saved normally at Indigo using radial L SAVE → Yes; preserved `indigo-three-wins-loss-earned-checkpoint.sav`. This is the latest earned continuation point. The user's main save was not modified.

## TM preparation and Rocket League

Travelled normally from Indigo through Victory Road in reverse, Route 23 (Cascade Board via Bag, then registered to Select and reused after island dismount), Route 22, Viridian, Route 2, Viridian Channel and Route 16 to Celadon. Previously unchallenged route trainers were fought with earned Mewtwo; these are progression checks, not normal-level balance evidence. The input helper needed updated live-object addresses and explicit turns on the staggered Channel bridge, not map edits.

Enabled 4× fast-forward on the isolated emulator for routine travel/battles; all actions remain normal button inputs. Bought 5,050 coins for 101,000: an unintended 50-coin purchase during menu testing is retained. The TM scripts retain the old prize-room names, but their active clerk is in Rocket League Lobby. The street-level prize room correctly sells held/battle items. Defeated the poster Grunt, revealed stairs, and entered Rocket League normally.

Purchased TM35 Flamethrower for 5,000 coins; taught Ninetales in place of Ember. Verified read-only move IDs 53/98/109/219 and empty TM pocket after consumption. Healed with Rocket League nurse and saved `flamethrower-earned-checkpoint.sav`. No ROM edits or injected inventory/party/story changes.

Started Rocket League to earn coins for Psychic. First two pool matches won; each awarded 1,000 coins and returned field control. Mewtwo reached 73 through earned EXP. Run in progress.

Third Rocket pool match won (Ariana); fourth ended in defeat against Petrel, with Weezing observed at level 40. Mewtwo exhausted Psychic/Swift PP, and the reserves could not finish the match. The button helper needed manual party selection because its display-slot mapping was unreliable; this is not evidence of a game defect. Loss retained, normal blackout/healing returned to Rocket Lobby. Three 1,000-coin wins retained.

Used the terminal's Rewards → Prize Board through ordinary inputs. Bought all three 250-coin upgrades, verifying x1.5, x2 and x3 messages. A fresh pool win then awarded 3,000 coins; balance became 5,300. Returned to the lobby voluntarily (unfinished challenge reset) and purchased TM29 Psychic for 5,000, leaving 300 coins. Taught Chimecho Psychic over Confusion. Read-only decrypted move verification: Ninetales 53/98/109/219; Chimecho 94/253/281/149. No injected items, flags or party changes.

Healed at the Rocket nurse and saved normally; observed “RED saved the game.” Preserved `psychic-flamethrower-earned-checkpoint.sav`, the newest earned continuation point. Both TM preparations are complete; next Indigo retry, Rocket Champion encounter and remaining endgame checks are still pending. No game source or balance edits in this segment.

## Prepared Indigo run — Champion defeated

Continued from the Psychic/Flamethrower checkpoint. Returned through Celadon, Route 16, Viridian Channel, Route 2, Viridian, Route 22, Route 23 and Victory Road using ordinary movement and the registered Cascade Board. Reboarded after the Route 23 island, defeated another route trainer, and traversed Victory Road without Strength. Healed at Indigo before entry. Corrected the external button helper to compare battle party IDs using the display-order mapping; no ROM/save injection.

Won all four pool matches: Erika, Bruno, the Psychic team (Xatu/Hypno/Bronzong/Chimecho/Claydol observed), and Surge. Confirmed 1/4, 2/4, 3/4 and all-four-victories messages. Ninetales used its improved attacking coverage; Chimecho's Psychic was visibly used during Surge. Party recovery worked between matches. This random pool did not include Lorelei, so her matchup with the prepared roster remains unverified.

The Champion door unlocked. Crossing the central readiness trigger still offers another pool-battle prompt, then repeats the Champion invitation; walking around that trigger reaches the north door normally. Entered the Champion room and saw the rival's introduction referencing Giovanni, Apex investigations and Gym Leaders. Won the level-50 double battle through normal inputs. Post-battle rival and Oak dialogue, Hall of Fame registration, credits and Continue all completed. Returned to Pallet Town with control restored.

Credits finding: the initial choice screen had no visible title or A/Select instructions. Read-only inspection confirmed credits state 5 (WAIT_TITLE_STAFF). Pressing A proceeded, and custom attribution pages and THE END rendered normally. Fixed `src/credits.c` to copy the opening title/choice window graphics to VRAM, matching the later credit-page update. Build passed (`/tmp/september30-credits-prompt-build.log`) and `git diff --check` passed. The fix is compiled into the main ROM but was not replayed live: the running isolated emulator completed its existing pre-fix ROM. Do not claim visual revalidation of the new opening prompt yet.

Preserved Hall of Fame autosave as `indigo-champion-earned-checkpoint.sav`. Continued normally, saved at Pallet, and preserved `pallet-post-champion-earned-checkpoint.sav` as the latest earned continuation point. Verified original levels and full HP: Mewtwo 73, Venusaur 47, Golbat 30, Ninetales 31, Jynx 31, Chimecho 35. User's main save untouched. Screenshots 06–16 cover pool wins, Champion scene, Hall of Fame, credits and return. No trainer balance changes.

Outstanding: live recheck of repaired credits prompt; Rocket Champion; remaining Apex encounters, final rival convergence, habitat/page reward checks, and normal-level balance review. Indigo completion is now verified; the broader postgame playthrough is not complete.

## Rocket Champion — earned completion

Continued from the Pallet post-Champion save. Walked to Celadon through Route 1, Viridian, Route 2, Viridian Channel and Route 16. Defeated a previously unchallenged Route 2 trainer normally. Used the Rainbow Key entrance to Celadon's Berry Patch, harvested its ripe Leppa Berry through Yes, and observed the regrowth message. Read-only bag check confirmed one Leppa Berry.

Healed in Rocket Lobby. Bought the x1.5 prize tier for 250 of the remaining 300 coins. Won three pool matches and used the harvested Leppa Berry on Mewtwo's Psychic through the normal Bag/party/move selection; the PP-restored message and item-use animation played. Won the fourth match, preserving the streak. All four pool rewards were 1,500 coins.

Entered Rocket Champion room with the earned eight-badge qualification. Giovanni introduction worked and the single battle was won. Mewtwo reached level 75 through earned experience. Honchkrow was observed live; the run did not capture a separate screenshot proving every opponent species. Post-battle trust dialogue, fixed 1,000-coin Champion reward and return to lobby all worked. The Champion reward is not multiplied, as specified by its script.

Found a battle-name presentation bug: the victory message displayed the player's rival name for Rocket Champion Giovanni. `battle_message.c` treated every CHAMPION-class trainer as the named rival. Restricted that substitution to the rival Champion portrait, preserving Giovanni's configured name. Main build passed (`/tmp/september30-rocket-name-build.log`). This fix, like the earlier credits opening prompt fix, is compiled but not live-replayed yet; the isolated emulator is still running the original session ROM. No balance changes.

Returning south from the lobby's north arrival crosses its registration trigger and repeats the full introduction; declined normally before proceeding to the nurse. Healed and saved through the radial menu; observed “RED saved the game.” Latest earned continuation point: `rocket-champion-earned-checkpoint.sav`. Party fully healed at original levels: Mewtwo 75, Venusaur 47, Golbat 30, Ninetales 31, Jynx 31, Chimecho 35. Main user save untouched. Both League titles have now been verified live. Remaining Apex encounters/rewards and any separate final rival meeting still need checking; do not infer completion from Champion dialogue alone.

## First post-League Apex checks

Read-only audit of the earned save found three accounts each for Tangrowth, Mewtwo and Mime Sr.; one each for Articuno and Osscythe; none for Zapdos, Moltres or Annihilape. Tangrowth, Osscythe, Mime Sr. and Annihilape fought flags were unset. Mewtwo's defeated flag was set. These were inspected, not modified.

Returned normally to Viridian Forest. Tangrowth was present in its clearing, with the level-50/one-chance warning. Attempted capture using Swift and Ultra Balls, but Tangrowth was defeated. This was an operator mistake, not a ROM defect: additional Swift input was chosen despite a read-only HP sample showing 10/163. Earlier commentary attributed this to menu confusion; the decisive error was failing to heed the low-HP reading. No rollback. Oak's Tangrowth follow-up and removal completed; capture is not claimed.

Traversed Diglett's Cave normally to Mime Sr.'s chamber. Its level-50 warning and battle appearance worked. One Swift reduced it to 8/122 HP; stopped attacks and confirmed the Bag screen before each throw. The second Ultra Ball caught Mime Sr. Capture EXP, Pokédex registration, declining a nickname, PC transfer (full party), Oak's follow-up and overworld object removal all completed. Screenshot `19-mime-sr-caught.png` records the capture, and `20-mime-sr-registration.png` records the dex page. Registration still says “UNKNOWN POKéMON”; review the intended category rather than silently inventing one.

Saved normally in Diglett's Cave B2F at (57,12), observed “RED saved the game,” and preserved `mime-sr-caught-earned-checkpoint.sav`. This is the latest continuation point. Mewtwo remains paralysed from Tangrowth, with 212 HP at the end of the capture. No game source/balance edits in this segment and no changes to the main user save. Remaining investigations and reward/evolution checks are still pending.

## Mime category and Osscythe capture

Changed Mime Sr.'s category from UNKNOWN to MIME as requested; the engine supplies POKéMON. Main ROM build passed (`/tmp/september30-mime-category-build.log`). Running isolated ROM remains the pre-fix build, so the new category is not yet visually revalidated.

Healed in Vermilion, then collected Osscythe's two missing accounts from Saffron's Cubone and Lavender's boy. The Cubone account remains available after Rocket leaves Saffron. With the existing Tower account, Osscythe appeared on Tower 4F. Its level-50 warning worked. Jynx used Lovely Kiss and Powder Snow, then fainted after Osscythe woke. Switched normally to Venusaur, used Sleep Powder, and caught Osscythe with the first Ultra Ball at 107/123 HP. Capture EXP, registration, declining nickname, PC transfer, Oak follow-up and object removal worked. Screenshot 21 records the capture.

Review finding, not changed: Osscythe's current species definition has all six base stats at 50, Normal/Normal typing, and its registration shows UNKNOWN POKéMON. These should be reviewed against its intended design. No balance edits made.

Saved normally on Tower 4F at (11,9); observed “RED saved the game.” Preserved `osscythe-caught-earned-checkpoint.sav`. Jynx is fainted; Venusaur has 113/143 HP. Main user save untouched. Annihilape and the three legendary birds remain pending.

## Annihilape — earned capture

Healed at Cerulean, then collected all three accounts normally: damaged-house Hiker, Route 4 woman and Mt. Moon 1F man. A previously unchallenged Route 4 trainer was defeated en route. Annihilape appeared in B2F after the final account; level-50 warning worked.

Venusaur's Sleep Powder did not leave it asleep. Two Razor Leafs reduced it to 111/172 HP; switched out at 13 HP. Three Ultra Balls and seven Great Balls failed. Mewtwo and Ninetales fainted during the attempt; Mewtwo's Recover could not keep up with the heavy attacks. Golbat held the field, and the eighth/final Great Ball caught Annihilape. No reload or injected resources. Screenshot 22 records capture. EXP, RAGE POKéMON registration, declining nickname, PC transfer, Oak follow-up and object removal all worked.

Saved normally at Mt. Moon B2F (34,7), observed save confirmation, and preserved `annihilape-caught-earned-checkpoint.sav`. This supersedes the Osscythe checkpoint for continuation. Ultra and Great Balls are exhausted; two Premier Balls remain. Mewtwo and Ninetales need revival/healing, Venusaur is at 13 HP. Legendary bird investigations remain outstanding; restock before attempting them. Main user save untouched.
