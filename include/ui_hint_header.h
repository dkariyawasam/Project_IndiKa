#ifndef GUARD_UI_HINT_HEADER_H
#define GUARD_UI_HINT_HEADER_H

#include "global.h"

extern const u16 gUiHintHeaderPalette[16];
extern const u8 gUiHintHeaderTextColors[3];

void DrawUiHintHeader(u8 windowId, const u8 *text, u8 fillColor, u8 lineColor, u8 textY, bool8 putTilemap);

#endif // GUARD_UI_HINT_HEADER_H
