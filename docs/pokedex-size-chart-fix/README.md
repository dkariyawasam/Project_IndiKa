# Added-species Pokédex size-chart calibration

37 active added species with actual battle sprites were recalibrated against a 1.5 m reference player. Existing vanilla entries, placeholder entries, sprite pixels, and listed height/weight values were not changed.

The chart uses inverse affine scales: larger values draw smaller silhouettes. The new values use each actual sprite's occupied dimensions, retain a shared scale in metres for Pokémon and player, fit the pair into the panel, and align their bottoms at y126. RED and LEAF baseline differences were checked. Tynamo is measured along its horizontal body length; other species use their posed vertical extent, so curled bodies, wings and long appendages remain visual approximations rather than anatomical measurements.

`calibration.json` records the original and new fields, the 37 updated species, and 36 skipped question-mark placeholders. `comparison-*.png` shows static before/after reconstructions with both player silhouettes. These are not emulator captures. `calibrate.py` records the calibration procedure.

Validation passed for scale ranges, vertical panel bounds, matching baselines for both player sprites, active Pokédex consistency, and the main ROM build. No live emulator test performed. Custom species' listed heights have not been independently reviewed; this pass makes their chart follow the existing height data.
