#include <distingnt/api.h>
#include "userTriggersMode.h"

void UserTriggersMode::DrawIcon(int x, int y, int color) const {
	int dim = color * 0.4;
	// background trigger
	NT_drawShapeI(kNT_line, x + 3, y + 0, x + 3, y + 16, dim);
	NT_drawShapeI(kNT_line, x + 5, y + 0, x + 5, y + 16, dim);
	NT_drawShapeI(kNT_line, x + 3, y + 0, x + 5, y + 0,  dim);
	// foreground trigger
	NT_drawShapeI(kNT_line, x +  0, y + 16, x + 11, y + 16, color);
	NT_drawShapeI(kNT_line, x + 11, y + 16, x + 11, y +  0, color);
	NT_drawShapeI(kNT_line, x + 11, y +  0, x + 13, y +  0, color);
	NT_drawShapeI(kNT_line, x + 13, y +  0, x + 13, y + 16, color);
	NT_drawShapeI(kNT_line, x + 13, y + 16, x + 16, y + 16, color);
}

void UserTriggersMode::Draw() const {
	NT_drawText( ModeAreaX + 50, 30, "Triggers Mode Placeholder" );
}
