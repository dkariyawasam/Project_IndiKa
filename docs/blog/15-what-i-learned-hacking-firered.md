---
title: "What I Learned Hacking FireRed"
slug: "what-i-learned-hacking-firered"
series: "Pokemon Expeditions: Kanto Devlog"
part: 15
status: "draft"
summary: "A reflection on palettes, tilemaps, scripting, iteration, and the joy of tiny fixes."
---

# What I Learned Hacking FireRed

This project has been a long chain of small discoveries.

Some were design discoveries. Some were technical. Some were just learning that one tile being wrong can ruin your entire afternoon.

## Palettes Are Everything

A lot of bugs that looked like broken graphics were actually palette problems:

- Sprites inheriting weather palettes.
- Transparent colours becoming opaque.
- Bridge tiles using the wrong palette from neighboring maps.
- Menu icons fighting over limited palette slots.
- Field effects turning black after a flashback.

On the GBA, colour is not just colour. It is state.

## Tilemaps Are Ruthless

A graphic can be perfect in a PNG and still be wrong in-game.

The title screen, bag menu, logbook, bridge connections, and trainer card all had moments where the art was fine but the tiles were not.

That taught the real rule:

The asset is only half the image. The tilemap is the other half.

## Scripts Need Rhythm

Cutscenes live or die on tiny timing details.

An exclamation mark needs a pause. A rival turn needs a delay. Giovanni needs to wait before walking. A flashback needs the white fade to cover the repositioning completely.

The scripting is not just logic. It is performance.

## Reuse Is Harder Than It Sounds

The Route 7 flashbacks forced a cleanup of reusable helpers:

- White transition helpers.
- Greyscale setup and teardown.
- Flashback object positioning.
- Music speed changes.
- Field effect cleanup.
- Cutscene recovery.

The first working version is rarely the clean version.

## Design Keeps Pulling Code Into Shape

Most technical changes came from a design need:

- The radial menu needed icon packing.
- Apex rumours needed counters and live object refresh.
- Bike rental needed guard-house routing.
- Giovanni's story needed new flags and quest state.
- The bag needed new pockets.
- The Pokedex needed cleaner habitat presentation.

Good design pressure is useful. It tells the code what it wants to become.

## Screenshot And Art Checklist

- [ ] Before and after screenshots from several systems.
- [ ] A funny broken tile/palette screenshot.
- [ ] A clean final UI screenshot.
- [ ] A cutscene frame.
- [ ] A sprite sheet before and after.

## Closing Thought

ROM hacking FireRed is a conversation with constraints. Sometimes the game says no. Sometimes it says yes, but only if you move everything one pixel down.

That is part of the charm.
