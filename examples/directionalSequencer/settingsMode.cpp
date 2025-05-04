#include <distingnt/api.h>
#include "settingsMode.h"

void SettingsMode::DrawIcon(int x, int y, int color) const {
	// faders
	NT_drawShapeI(kNT_box, x +  5, y +  2, x +  8, y +  4, color);
	NT_drawShapeI(kNT_box, x + 12, y +  7, x + 15, y +  9, color);
	NT_drawShapeI(kNT_box, x +  2, y + 12, x +  5, y + 14, color);
	// lines
	NT_drawShapeI(kNT_line, x +  0, y +  3, x +  5, y +  3, color);
	NT_drawShapeI(kNT_line, x +  8, y +  3, x + 16, y +  3, color);
	NT_drawShapeI(kNT_line, x +  0, y +  8, x + 12, y +  8, color);
	NT_drawShapeI(kNT_line, x + 15, y +  8, x + 16, y +  8, color);
	NT_drawShapeI(kNT_line, x +  0, y + 13, x +  2, y + 13, color);
	NT_drawShapeI(kNT_line, x +  5, y + 13, x + 16, y + 13, color);
}

void SettingsMode::Draw() const {
	NT_drawText( ModeAreaX + 50, 30, "Settings Mode Placeholder" );
}
