# Running area audit

Outdoor map types and underground dungeons already permit running. Per the requested indoor exceptions, Pokémon Tower 1F–4F, both Celadon rooftops and Celadon Berry Patch retain `allow_running: false`. Ordinary indoor spaces and tile-specific movement restrictions are unchanged.

The seven flags from the earlier pass were restored. Main ROM rebuilt; no live traversal performed.

## Existing-save running fix

Route 3 and Route 4 both allow running and contain no MB_RUNNING_DISALLOWED tiles. Running uses the same input path for both player characters. Removed the legacy FLAG_SYS_B_DASH requirement from that path: existing saves no longer depend on having visited the newer first-bedroom scene. Holding B now runs wherever map/terrain rules permit; indoor exceptions remain. This identifies and removes a possible save-state blocker, without claiming the user’s current flag was read.
