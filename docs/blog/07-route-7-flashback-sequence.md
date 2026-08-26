---
title: "The Route 7 Flashback Sequence"
slug: "route-7-flashback-sequence"
series: "Pokemon Expeditions: Kanto Devlog"
part: 7
status: "draft"
summary: "Building a memory sequence where the player reconstructs Mewtwo's rampage."
---

# The Route 7 Flashback Sequence

Route 7 became one of the biggest storytelling set pieces in the mod.

The player does not watch Giovanni and Mewtwo defeat the Elite Four in real time. They arrive afterward, walk through the damage, and hear what happened from the people who survived it.

## The Beam Cut

The map itself tells the first part of the story.

A path has been carved through the trees, turning a once-blocked stretch of land into a scar running north. Before Silph Co is cleared, the trees still cover the area. Afterward, the blast path is revealed.

[Screenshot: Route 7 before and after the beam cut]

## Four Witnesses

Each Elite Four member gives a flashback:

- Lorelei sees Mewtwo fire the first devastating beam.
- Bruno is thrown aside and slammed into the rock.
- Agatha sees the control over Mewtwo begin to fail.
- Lance witnesses the moment Mewtwo goes fully rogue in Celadon Cave.

Each flashback adds a new piece of the event.

## The Visual Language

The flashbacks use:

- White fades instead of black fades.
- Greyscale tinting.
- Slowed music.
- Screen shake.
- Dust effects.
- A bespoke Mewtwo beam and charge circle.
- NPC-only flashback versions of the characters.

The goal is to make the scene feel like memory, not a normal cutscene.

## The Technical Rabbit Hole

The tricky part was not just making a flashback. It was making the game recover afterward.

Problems included:

- The player turning invisible after the flashback.
- Tall grass field effects lingering as black blocks.
- NPCs staying in the wrong visibility state.
- Effects not receiving greyscale.
- White fades briefly revealing the wrong scene.

The final solution uses reusable transition helpers, white overlay coverage, careful field effect cleanup, and dedicated flashback object slots.

## Current Build Notes

The four flashbacks now form a sequence:

- Lorelei shows Giovanni trying to command Mewtwo's beam attack.
- Bruno shows Mewtwo slamming him aside with an impact sound, dust, and screen shake.
- Agatha shows the control over Mewtwo starting to fail.
- Lance, inside Celadon Cave, shows Mewtwo fully breaking loose while Giovanni gives chase.

All four use the same white-flash transition language, greyscale memory tint, and post-flashback dialogue structure. The player and present-day NPCs are kept out of the staged flashback space, then restored cleanly afterward.

The beam-cut path now uses rough terrain rather than grass, and Celadon Cave connects Route 7 to Route 4 as the wound Mewtwo leaves behind.

## Screenshot And Art Checklist

- [ ] Route 7 beam cut.
- [ ] Lorelei flashback beam charge.
- [ ] Bruno impact moment.
- [ ] Agatha control-waning moment.
- [ ] Lance in Celadon Cave.

## Closing Thought

This sequence is exactly the kind of thing ROM hacking rewards: a small map change, a few sprites, and a lot of careful timing can turn an old route into a story event.
