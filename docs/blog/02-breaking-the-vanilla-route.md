---
title: "Breaking The Vanilla Route"
slug: "breaking-the-vanilla-route"
series: "Pokemon Expeditions: Kanto Devlog"
part: 2
status: "draft"
summary: "How the mod pushes players away from the classic badge-route mindset."
---

# Breaking The Vanilla Route

Most FireRed players know the opening by muscle memory:

Pallet. Viridian. Route 2. Viridian Forest. Pewter.

Pokemon Expeditions: Kanto keeps that path, but stops treating it as the only correct answer.

## The Problem

If a player has played FireRed before, they will naturally follow the old route even if the mod adds new options. That means the game has to actively teach them that Kanto has changed.

Not with a tutorial box. With the world itself.

[Screenshot: Route 2 rival encounter]

## The First Break

The first rival encounter now happens near Route 2 and reframes the early game. He mentions routes the player might not expect:

- Viridian Channel leads toward Celadon.
- Diglett Cave leads toward Vermilion.
- The ferry south of Pallet can carry the player toward Cinnabar.

The point is not to explain every story thread immediately. It is to make the player stop and think:

Wait, I do not have to go straight to Pewter?

## The Ferry Problem

Opening Kanto early creates a danger: softlocks.

If the player sails to Cinnabar before they have the usual mobility tools, whites out, or changes healing points, they can end up separated from the route they used to arrive.

That led to a full pass on:

- Ferry access.
- Healing house respawn points.
- Bike rental gates.
- Route connections.
- Guard house behavior.

The goal is simple: the player can explore out of order without the game trapping them for doing it.

## Route Connections Instead Of Gatehouses

Some gatehouses existed mostly because vanilla Kanto needed blockers. Once the blockers were removed, the buildings started feeling like leftover friction.

So some entrances became direct connections instead:

- Route 2 to Viridian Forest.
- Saffron entrances after the tea blockade cleanup.
- New pathing around Viridian Channel and Route 7.

That makes movement feel more natural, but it also means every connection strip has to line up tile-wise and palette-wise. Tiny map edge mistakes become very visible.

## Current Build Notes

Route 2's Viridian Forest gatehouses have now been removed entirely. The forest entrances behave like direct route connections instead of extra rooms, which better matches the new open-route philosophy.

Saffron's tea blockade has also been removed. The city now connects directly to the surrounding routes, and the connection strips have been retiled so the entrances no longer read as blocked.

The bike is now a purchased or rented travel tool rather than a voucher reward. The cycling-road guard houses can rent one, owned bikes bypass the rental scripts, and the Bike Voucher has been removed.

Route 7 has become the clearest expression of the new structure: it is no longer just a pass-through route, but the aftermath of Giovanni and Mewtwo tearing a path north toward Celadon Cave.

## Screenshot And Art Checklist

- [ ] Route 2 path choice.
- [ ] Pallet to Route 21 ferry sign.
- [ ] Bike rental guard house.
- [ ] Direct forest entrance.
- [ ] Saffron entrance without tea blockade.

## Closing Thought

The best non-linear design trick here is not adding a giant menu of destinations. It is making the player doubt their FireRed autopilot.
