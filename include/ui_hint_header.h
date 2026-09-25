#ifndef GUARD_UI_HINT_HEADER_H
#define GUARD_UI_HINT_HEADER_H

#include "global.h"

extern const u16 gUiHintHeaderPalette[16];
u16 GetUiHintHeaderColor(void);
void ApplyUiHintHeaderTheme(u16 paletteEntry);
void ApplyUiDetailsFooterTheme(u16 paletteOffset);
void LoadUiHintHelpPalette(u16 paletteOffset);
void LoadUiHintHelpPaletteForFrame(u16 paletteOffset, u8 frameType);
extern const u8 gUiHintHeaderTextColors[3];
void DrawUiPageNotches(u8 windowId, u8 x, u8 count, u8 selected, u8 background);

void DrawUiHintHeader(u8 windowId, const u8 *text, u8 fillColor, u8 lineColor, u8 textY, bool8 putTilemap);

#endif // GUARD_UI_HINT_HEADER_H
