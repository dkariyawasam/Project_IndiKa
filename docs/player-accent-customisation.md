# Player style and clothing colours

New games show one appearance window before naming the player:

- STYLE 1 / STYLE 2: original boy / girl sprites.
- COLOR 1: original red clothing accents, with blue and green hue shifts.
- COLOR 2: original blue trousers/skirt and other blue clothing, with red and green hue shifts. This selection also sets the corresponding UI frame (blue = 1, red = 2, green = 3).
- DONE: continue to naming.

Up/down select a row; left/right change it. A cycles a setting or confirms DONE. Both colours and the style preview update immediately, as does the frame theme. Skin, hair and sprite shapes remain unchanged. The normal Options screen still lets the player change their frame later independently of clothing.

SaveBlock2 uses existing padding: 0x90 version 3, 0x91 primary accent, 0x92 secondary accent. Following fields retain their offsets. Version-2 saves retain their primary choice and default blue secondary. Older/invalid versions use original colours. Existing saves are not prompted retroactively.

Primary palette slots: intro 27–31, front portrait 12–13, overworld/back/credits/map icon 11–12. Secondary slots: intro 17–21, front portrait 5–8, overworld/back/credits/map icon 5–7. RGB channel rotations preserve shade components. Each hook runs after loading original palettes and before field tint, with no additional object palette slots. Previews reload originals to avoid cumulative shifts.

Validation: main ROM build, production transform tests for both independent accents/legacy versions/invalid choices/palette boundaries; object palette lifetime and recovery regressions; all 14 player overworld sprite-index checks.

Live verification on the rebuilt ROM: combined menu and unobstructed STYLE 1 preview; switching to STYLE 2; independent first-colour blue selection; second-colour red and green with matching frame/dialogue themes; DONE continuing to naming with the selected appearance and green frame retained.
