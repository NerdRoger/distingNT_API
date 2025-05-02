#include <distingnt/api.h>
#include <quantizerMode.h>

void QuantizerMode::DrawIcon(int x, int y, int color) {
	NT_drawShapeI(kNT_rectangle, x + 0, y + 0, x + 16, y + 16, color);
	NT_drawShapeI(kNT_line, x +  5, y + 0, x +  5, y + 16, 0);
	NT_drawShapeI(kNT_line, x + 11, y + 0, x + 11, y + 16, 0);
	NT_drawShapeI(kNT_rectangle, x +  3, y + 0, x +  6, y + 8, 0);
	NT_drawShapeI(kNT_rectangle, x + 10, y + 0, x + 13, y + 8, 0);
}

void QuantizerMode::Draw() {
	NT_drawText( ModeAreaX + 50, 30, "Quantizer Mode Placeholder" );
}
