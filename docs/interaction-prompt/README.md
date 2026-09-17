# Contextual A prompt prototype

Implemented in `src/field_control_avatar.c`, called after overworld input processing.

- Appears immediately while stationary facing a scripted object or a direction-valid sign background event.
- Supports talking across a counter and tracks the target sprite's position and camera offset.
- Disappears during movement, button input, locked controls, fades and quest-log playback.
- Excludes hidden-item/secret background events, automatic doors and coordinate triggers.
- Uses a built-in menu A-button artwork centered in a 16×16 sprite and the darkest/lightest opaque entries from the existing player palette. No additional palette allocation; four sprite tiles and one sprite while visible.
- Reads interaction eligibility without invoking dispatchers that mutate selected-object, special-variable or signpost state.
- Finds its sprite by template so map and battle sprite resets cannot leave a stale sprite ID; destroys the sprite and frees its tiles when hidden.

Validation: main ROM build and `git diff --check` passed. No live visual validation yet. Check NPCs, counters, directional signs, hidden-item tiles, moving NPCs, menus, battle return, map transitions, and day/night contrast before considering this final. RAM-script overrides are outside the read-only object check.

## Invisible-image fix

Replaced the dummy end-only sprite animation with frame 0 followed by end. The old animation never queued the image upload to OBJ VRAM. Verified the compiled ROM template points to the new animation table and its first command loads frame 0. Rebuild passed; live visual confirmation remains pending.

## Position and timing adjustment

Lowered the NPC prompt by 8 pixels to sit closer to the visible head. Reduced the stationary delay from 18 to 4 frames (roughly 0.07 seconds at 60 fps). Sign positioning is unchanged.

## Menu artwork

Uses the first 8×12 icon from `gKeypadIconTiles`, the same graphic rendered for CHAR_A_BUTTON in menus. Preserves the artwork while remapping its two opaque colours to the existing player palette. NPC anchor lowered another 4 pixels; four-frame delay retained. Build verified; placement awaits in-game review.

Latest tuning: lowered the NPC anchor another 2 pixels (+6 relative to the sprite top), and reduced the delay from 4 frames to 1 frame.

Delay removed entirely: eligible prompts appear on the first stationary update.

## Expanded interactions

Now uses the same metatile script lookup as A-button dispatch, without changing facing variables or signpost UI state during preview. Covers PCs, maps, shelves, computers, TVs, readable scenery, and existing A-button water interactions. Existing object/sign coverage includes visible scripted items and notes. Hidden/secret background events remain excluded.

Build passed. Host comparison of original and refactored dispatch passed 1,024 synthetic behavior/direction cases, including matching signpost/facing effects and side-effect-free previews. This checks lookup refactoring, not live rendering or every map placement.

## Visible-top anchoring

Object prompts now scan the displayed 4bpp frame for its first opaque row, ignoring transparent top padding. The menu glyph bottom is positioned with exactly two empty pixel rows before the object’s visible top. Host checks cover every pixel position in 16/32px frame dimensions and gap arithmetic. Main ROM build passed; live visual confirmation pending.

## Background sign anchoring

Removed the old player-relative -24px offset for background interactions. The anchor now uses the target map tile top and scans the foreground metatile layer for its first opaque row (including tile flips), then leaves two empty rows below the button. This handles transparent padding on ordinary signs. Opaque/composite scenery may still need an explicit per-object anchor. Main ROM build passed; live sign placement remains to be checked.
