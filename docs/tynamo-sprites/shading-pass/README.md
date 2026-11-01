# Tynamo line shading pass

Validation: identical transparency masks, 64×64 dimensions, indexed palette values below 16, main ROM build. No live battle test performed.

## Tynamo user update — 29 October 2026

The user updated both active Tynamo sprites directly. Both are valid 64×64 indexed images and match the runtime normal palette exactly. No pixel edits were performed during integration. Snapshots and hashes are retained in `tynamo-user-update.json`. The historical shading converter now skips Tynamo. This records a user revision, not an independent claim that every earlier generated shading pixel was replaced. Eelektrik and Eelektross remain pending review.

## Eelektrik and Eelektross user updates — 29 October 2026

The user updated both front and back sprites for Eelektrik and Eelektross directly. All four images are valid 64×64 indexed assets with transparent index zero and exact runtime normal-palette matches. Integration preserves all pixels and palettes unchanged. Snapshots and hashes are retained in the corresponding user-update JSON files. The historical shading converter now skips all three species. These records establish user revisions, not an independent determination that every earlier generated shading pixel has been replaced.
