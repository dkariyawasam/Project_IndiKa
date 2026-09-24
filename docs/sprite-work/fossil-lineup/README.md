# Fossil family comparison

Lineup renders current normal palettes with GBA 5-bit colour conversion, at uniform 3× scale. Kabustar/Kabuknight omitted because their front sprites remain placeholders.

Kabutops brown armour now uses Kabuto's exact shell ramp (normal indices 11–14). Pale body/blades and other palette entries unchanged; front/back pixel indices unchanged. Embedded PNG palettes and lineup updated. Main ROM rebuild passed.

Dart body consistency: user confirmed Omato and Omatops, not Omastar. Their normal body indices 4/5 now use Kinkabuto grey/ivory indices 8/14. Kabutops pure white index 1 now uses Kinkabuto ivory index 14; existing compatible grey shading retained. Shell colours, red eyes, pixel indices and Omastar blues unchanged. Embedded front/back palettes and lineup refreshed. Main ROM build passed. Previously noted incorrect Omato/Omatops back artwork still needs replacement.

Cream body follow-up: adopted the exact Omanyte/Omastar shell ramp for Omato/Omatops body indices 5/4 (highlight/mid-shadow), Kinkabuto pale indices 14/8/6 (highlight/mid/deep shadow), and Kabutops pale indices 1–5 (full five-shade ramp). Kinkabuto's shared pale entries also colour its blades and small highlights. All pixel indices and unrelated palette entries preserved; front/back embedded palettes synchronized where present. Main ROM build passed and lineup refreshed.

Omato/Omatops head shading: body shadow index 4 now uses the shared cream ramp's darker 172,156,115 shade; body main index 5 uses 230,222,189 instead of the brightest highlight. Existing bright highlight entries remain available. Preserved eye/claw/shell entries and all pixel indices. Updated front/back embedded palettes and lineup. Main ROM build passed.

Omatops shell shadow cleanup: legacy reddish-brown shade 3 now matches darkest shared shell shade 10 (98,82,49); deepest brown shade 2 changed to complementary 74,65,40. Black outlines and the established five-shade cream ramp remain unchanged. These shared legacy shade entries also affect small creases outside the shell. Pixel indices unchanged, embedded palettes and lineup refreshed, main ROM build passed.

Shell section seams: built-in image edit requested two short curved brown seams, beneath the upper rim and around the lower taper, with highlight breaks. Encoded those accents at native resolution using existing shadow index 10, changing 37 midtone shell pixels only. All other pixels and the palette preserved. Original backed up as omatops-before-section-seams.png; generated guide retained as omatops-section-seam-reference.png. Lineup refreshed and main ROM build passed.

User follow-up: changed only the 37 new seam pixels from brown index 10 to existing outline-black index 1. Highlight breaks and all other pixels preserved. Main ROM rebuilt and lineup refreshed.

Omatops side blades: built-in image edit requested short tapered blades with cream cutting edges and warm dark undersides at the widest rim, preserving the spinning-top form. Imported only the two side blade regions (x1–14 and x49–62, y27–34) into the original indexed front sprite. All pixels outside these regions verified unchanged. Existing palette retained. Original backed up as omatops-before-blades.png; generated source retained as omatops-blades-generated.png. Main ROM build passed; lineup refreshed. Back artwork remains the previously noted unrelated placeholder.

Spinning scythes revision: user requested hooked scythes rather than straight blades. Built-in edit produced opposed crescent sweeps (left upward, right downward), cream cutting edges and warm shadow faces. Imported only outer blade regions into original 64×64 sprite; head, shell seams and lower point preserved. Previous version retained as omatops-before-scythes.png. Main ROM build passed and lineup refreshed.

Both generated side-blade revisions reverted at user's request. Restored omatops-before-blades.png byte-for-byte, returning the original side projections while retaining earlier black shell seams and current palette. Lineup refreshed; main ROM rebuild passed.
