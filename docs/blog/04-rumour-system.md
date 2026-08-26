---
title: "The Rumour System"
slug: "rumour-system"
series: "Pokemon Expeditions: Kanto Devlog"
part: 4
status: "draft"
summary: "How NPC dialogue became a discovery system for Apex Pokemon."
---

# The Rumour System

The rumour system started from a very practical problem:

If Apex Pokemon are hidden, how does the player know where to look?

The answer was already in Pokemon's DNA. Talk to people.

## Three Rumours, One Discovery

Each Apex Pokemon has three rumours. Hearing one starts the relevant Logbook entry. Hearing all three confirms the lead and reveals the Apex in the world.

That creates a small investigation loop:

1. Hear something strange.
2. Find another witness.
3. Confirm the location.
4. Go investigate.

[Screenshot: Rumour recorded notification]

## Making Rumours Feel Like Events

Rumours are not just text. They now have a small feedback sequence:

- NPC dialogue plays.
- The dialogue closes properly.
- The player reacts with an exclamation mark.
- A "Rumour recorded" or "Rumour updated" message appears.
- The game waits before returning control.

That little beat matters. It tells the player they found something important without turning it into a giant tutorial.

## The Apex Log

The old Fame Checker was repurposed into the Apex Log.

Instead of celebrity gossip, it now shows:

- The Apex Pokemon silhouette.
- The NPCs who gave rumours.
- The rumour text after it is discovered.
- Progress toward finding the Apex.

Before a rumour is found, the relevant NPC appears as a blacked-out silhouette. Afterward, the full overworld sprite is shown.

## Thematic Rumours

Each rumour group was rewritten to match its Apex theme.

Examples:

- Annihilape rumours focus on anger and loss of control.
- Mime Sr rumours focus on an isolated community in Diglett Cave.
- Osscythe rumours focus on family and grief.
- Tangrowth rumours feel like local urban legends.
- Mewtwo rumours are tied to Giovanni's ambition.

## Current Build Notes

The old Fame Checker has been repurposed internally as the Apex Log. It is no longer a key item the player receives; it lives under the Logbook, where it can show the NPC witnesses tied to each Apex Pokemon.

Mewtwo now follows the same logic, but through Giovanni's story. The first entry comes from Giovanni at the broken vat in Pokemon Mansion, the second from Silph's president after the Silph Co confrontation, and the third from Lance after the Route 7 aftermath.

Rumours now refresh visible Apex objects immediately when the third rumour is recorded, so the player no longer has to leave and re-enter the map before an Apex appears.

## Screenshot And Art Checklist

- [ ] NPC before and after rumour discovery.
- [ ] Apex Log with silhouettes.
- [ ] Apex Log after all rumours.
- [ ] An NPC giving a thematically strong rumour.
- [ ] Apex overworld appearing after confirmation.

## Closing Thought

The rumour system makes NPCs matter again. They are not just flavor text. They are witnesses.
