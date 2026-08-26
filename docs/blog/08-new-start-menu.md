---
title: "The New Start Menu"
slug: "new-start-menu"
series: "Pokemon Expeditions: Kanto Devlog"
part: 8
status: "draft"
summary: "Replacing FireRed's list menu with a radial field aide toolkit."
---

# The New Start Menu

The old FireRed start menu is practical, but it feels like a list.

Pokemon Expeditions needed something closer to a field aide toolkit.

So the start menu became radial.

## The Shape

The radial menu opens around the player, with icons for:

- Pokedex
- Pokemon
- Bag
- Card
- Map
- Settings
- Help
- Logbook

The selected icon bobs, the backdrop scales in, and the center label updates with the current entry.

[Screenshot: radial start menu open]

## Why Radial?

A radial menu makes the player's tools feel physical and immediate. It also helps the mod's identity: this is not just FireRed with more options. This is a field aide interface.

The menu now acts as a hub for the systems the mod cares about most.

## The Visual Pass

The radial menu went through a lot of tuning:

- Icon positions were adjusted pixel by pixel.
- Save and Settings labels moved into the shared header language.
- The center label was refined to avoid tile glitches.
- The backdrop was recolored to fit the UI.
- Icons were packed into a shared sheet.
- Palette tags were moved away from weather sprite tags.

## The Control Pass

Navigation was cleaned up so directions feel intentional.

Examples:

- Map and Settings sit opposite each other.
- Pokedex connects to the upper tools.
- Help and Logbook sit in the lower tool group.
- The D-pad glyph changes based on available directions.

## Current Build Notes

The radial menu now has eight entries: Pokedex, Pokemon, Bag, Card, Map, Settings, Help, and Logbook.

The icons are packed into one sheet with a shared palette, and the directional glyph changes depending on which neighbouring entries are available. `L SAVE` now lives in the common blue header instead of being a separate floating label.

The menu still opens with its backdrop scaling into place, then the icons appear once the backdrop has settled. The save overwrite warning has also been removed so saving feels less fussy.

## Screenshot And Art Checklist

- [ ] Closed overworld before opening menu.
- [ ] Radial menu open with Pokedex selected.
- [ ] Icon sheet.
- [ ] D-pad glyph variants.
- [ ] Menu opening animation.

## Closing Thought

The start menu is one of the first things players touch over and over. Making it feel like this mod's menu, not just FireRed's menu, was worth the tiny pixel fights.
