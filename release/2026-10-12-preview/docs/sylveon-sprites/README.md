# Sylveon sprites

Adapted fan-made pixel sprites from [RHH pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/sylveon). Credit Rom Hacking Hideout and its sprite contributors; see the upstream [credits](https://github.com/rh-hideout/pokeemerald-expansion/blob/master/CREDITS.md). Original downloaded assets are preserved in `source/`. These are fan-made sprites, not official Nintendo 2D artwork or AI-generated artwork. Generation attempts produced no assets; this sourced set supersedes that incomplete attempt.

The first 64×64 front frame and the 64×64 cropped back are used without scaling, preserving the original pixel outlines. Normal and shiny palettes retain matching indices and use FRLG Chimecho pinks/blues and Eevee cream/shadow colours. The menu icon uses shared icon palette 0. The footprint is included. Overworld frames use the first six source frames, reordered 0,2,4,1,0,3,2,5,4 for the engine's nine-frame walking layout, with frame 0 as the extra fallback. Its palette is adapted separately to the same colours.

Battle tables, icons, footprint, coordinates and overworld graphics ID 218 are registered. The overworld sprite is available for future map use; no new map placement is added.

Main `pokefirered.gba` rebuilt successfully. Checked image dimensions, transparency index 0, matching shiny indices and visual previews. Existing frame-packing audit passes, but skips this new sprite; its nine 512-byte frame blocks were checked through the generated sheet preview. Live battle/map testing remains outstanding.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
