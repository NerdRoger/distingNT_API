#include <string.h>
#include "quantizerView.h"
#include "weightedQuantizer.h"
#include "common.h"


const QuantizerView::Control QuantizerView::Controls[] = {
	{ kWQParamQuantWeightC,      "  Adjust the attraction weighting of note C"  },
	{ kWQParamQuantWeightCSharp, "  Adjust the attraction weighting of note C#" },
	{ kWQParamQuantWeightD,      "  Adjust the attraction weighting of note D"  },
	{ kWQParamQuantWeightDSharp, "  Adjust the attraction weighting of note D#" },
	{ kWQParamQuantWeightE,      "  Adjust the attraction weighting of note E"  },
	{ kWQParamQuantWeightF,      "  Adjust the attraction weighting of note F"  },
	{ kWQParamQuantWeightFSharp, "  Adjust the attraction weighting of note F#" },
	{ kWQParamQuantWeightG,      "  Adjust the attraction weighting of note G"  },
	{ kWQParamQuantWeightGSharp, "  Adjust the attraction weighting of note G#" },
	{ kWQParamQuantWeightA,      "  Adjust the attraction weighting of note A"  },
	{ kWQParamQuantWeightASharp, "  Adjust the attraction weighting of note A#" },
	{ kWQParamQuantWeightB,      "  Adjust the attraction weighting of note B"  },
};


void QuantizerView::Draw() const {
	DrawKeyboard(0, 0);
	DrawResults();
	DrawHelpSection();
}


void QuantizerView::DrawSlot(uint8_t x, uint8_t y, uint8_t slotNum) const {
	NT_drawShapeI(kNT_box, x, y, x + 14, y + 14, KeyBorderColor);
	uint8_t dx = 5;
	dx += slotNum == 0 ?  1 : 0;
	dx += slotNum == 9 ? -2 : 0;
	NT_drawText(x + dx, y + 11, "2", 8);
}


void QuantizerView::DrawWhiteKeySlider(uint8_t x, uint8_t y, int paramIdx) const {
	float scaling = CalculateScaling(AlgorithmInstance->parameters[paramIdx].scaling);
	auto val = AlgorithmInstance->v[paramIdx] / scaling;
	if (val > 0) {
		auto height = val * 49 / MaxSliderValue;
		auto dy = 49 - height;
		NT_drawShapeI(kNT_rectangle, x, y + dy, x + 15, y + 49, ActiveKeyColor);
		NT_drawShapeI(kNT_rectangle, x, y + dy, x + 15, y + dy, SliderLineColor);
	}
}


void QuantizerView::DrawBlackKeySlider(uint8_t x, uint8_t y, int paramIdx) const {
	float scaling = CalculateScaling(AlgorithmInstance->parameters[paramIdx].scaling);
	auto val = AlgorithmInstance->v[paramIdx] / scaling;
	NT_drawShapeI(kNT_rectangle, x - 1, y + 0, x + 9, y + 33, 0);
	if (val > 0) {
		auto height = val * 32 / MaxSliderValue;
		auto dy = 32 - height;
		NT_drawShapeI(kNT_rectangle, x + 0, y + dy, x + 8, y + 32, ActiveKeyColor - 4);
		NT_drawShapeI(kNT_rectangle, x + 0, y + dy, x + 8, y + dy, SliderLineColor);
	}
}


void QuantizerView::DrawCFKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y +  0 - expandBy, x +  9 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  9 + expandBy, y +  0 - expandBy, x +  9 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  9 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y +  0 - expandBy, color);
}


void QuantizerView::DrawDKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y +  0 - expandBy, x + 11 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y +  0 - expandBy, x + 11 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y +  0 - expandBy, color);
}


void QuantizerView::DrawEBKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  6 - expandBy, y +  0 - expandBy, x + 15 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y +  0 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  6 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  6 - expandBy, y + 34 - expandBy, x +  6 - expandBy, y +  0 - expandBy, color);
}


void QuantizerView::DrawGKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y +  0 - expandBy, x + 10 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 10 + expandBy, y +  0 - expandBy, x + 10 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 10 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y +  0 - expandBy, color);
}


void QuantizerView::DrawAKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  5 - expandBy, y +  0 - expandBy, x + 11 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y +  0 - expandBy, x + 11 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  5 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  5 - expandBy, y + 34 - expandBy, x +  5 - expandBy, y +  0 - expandBy, color);
}


void QuantizerView::DrawBlackKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_box, x + 0 - expandBy, y + 0 - expandBy, x + 8 + expandBy, y + 32 + expandBy, color);
}


void QuantizerView::DrawKeyboard(uint8_t x, uint8_t y) const {
	// draw the sliders as rectangles first...  we will come back and draw the borders at the end, so they are not drawn over
	// also draw white key sliders before black key sliders, since black ones overlap white ones
	DrawWhiteKeySlider(x +   0, y, kWQParamQuantWeightC);
	DrawWhiteKeySlider(x +  17, y, kWQParamQuantWeightD);
	DrawWhiteKeySlider(x +  34, y, kWQParamQuantWeightE);
	DrawWhiteKeySlider(x +  51, y, kWQParamQuantWeightF);
	DrawWhiteKeySlider(x +  68, y, kWQParamQuantWeightG);
	DrawWhiteKeySlider(x +  85, y, kWQParamQuantWeightA);
	DrawWhiteKeySlider(x + 102, y, kWQParamQuantWeightB);

	DrawBlackKeySlider(x + 11, y, kWQParamQuantWeightCSharp);
	DrawBlackKeySlider(x + 30, y, kWQParamQuantWeightDSharp);
	DrawBlackKeySlider(x + 62, y, kWQParamQuantWeightFSharp);
	DrawBlackKeySlider(x + 80, y, kWQParamQuantWeightGSharp);
	DrawBlackKeySlider(x + 98, y, kWQParamQuantWeightASharp);

	DrawCFKeyBorder(x +   0, y, 0, KeyBorderColor);
	DrawDKeyBorder( x +  17, y, 0, KeyBorderColor);
	DrawEBKeyBorder(x +  34, y, 0, KeyBorderColor);
	DrawCFKeyBorder(x +  51, y, 0, KeyBorderColor);
	DrawGKeyBorder( x +  68, y, 0, KeyBorderColor);
	DrawAKeyBorder( x +  85, y, 0, KeyBorderColor);
	DrawEBKeyBorder(x + 102, y, 0, KeyBorderColor);

	DrawBlackKeyBorder(x + 11, y, 0, KeyBorderColor - 4);
	DrawBlackKeyBorder(x + 30, y, 0, KeyBorderColor - 4);
	DrawBlackKeyBorder(x + 62, y, 0, KeyBorderColor - 4);
	DrawBlackKeyBorder(x + 80, y, 0, KeyBorderColor - 4);
	DrawBlackKeyBorder(x + 98, y, 0, KeyBorderColor - 4);

	if (Editable) {
		if (Controls[SelectedControlIndex].ParameterIndex >= kWQParamQuantWeightC) {
			switch (Controls[SelectedControlIndex].ParameterIndex)
			{
				case kWQParamQuantWeightC:
					DrawCFKeyBorder(x + 0, y, -1, 15);
					DrawCFKeyBorder(x + 0, y,  0, 15);
					break;
				case kWQParamQuantWeightCSharp:
					DrawBlackKeyBorder(x + 11, y, -1, 15);
					DrawBlackKeyBorder(x + 11, y,  0, 15);
					break;
				case kWQParamQuantWeightD:
					DrawDKeyBorder(x + 17, y, -1, 15);
					DrawDKeyBorder(x + 17, y,  0, 15);
					break;
				case kWQParamQuantWeightDSharp:
					DrawBlackKeyBorder(x + 30, y, -1, 15);
					DrawBlackKeyBorder(x + 30, y,  0, 15);
					break;
				case kWQParamQuantWeightE:
					DrawEBKeyBorder(x + 34, y, -1, 15);
					DrawEBKeyBorder(x + 34, y,  0, 15);
					break;
				case kWQParamQuantWeightF:
					DrawCFKeyBorder(x + 51, y, -1, 15);
					DrawCFKeyBorder(x + 51, y,  0, 15);
					break;
				case kWQParamQuantWeightFSharp:
					DrawBlackKeyBorder(x + 62, y, -1, 15);
					DrawBlackKeyBorder(x + 62, y,  0, 15);
					break;
				case kWQParamQuantWeightG:
					DrawGKeyBorder(x + 68, y, -1, 15);
					DrawGKeyBorder(x + 68, y,  0, 15);
					break;
				case kWQParamQuantWeightGSharp:
					DrawBlackKeyBorder(x + 80, y, -1, 15);
					DrawBlackKeyBorder(x + 80, y,  0, 15);
					break;
				case kWQParamQuantWeightA:
					DrawAKeyBorder(x + 85, y, -1, 15);
					DrawAKeyBorder(x + 85, y,  0, 15);
					break;
				case kWQParamQuantWeightASharp:
					DrawBlackKeyBorder(x + 98, y, -1, 15);
					DrawBlackKeyBorder(x + 98, y,  0, 15);
					break;
				case kWQParamQuantWeightB:
					DrawEBKeyBorder(x + 102, y, -1, 15);
					DrawEBKeyBorder(x + 102, y,  0, 15);
					break;
				default:
					break;
			}
		}
	}
}


void QuantizerView::DrawHelpSection() const {
	// clear the area we're about to draw in, as we are drawing on top of the grid
	NT_drawShapeI(kNT_rectangle, 0, 50, 255, 63, 0);

	// draw the transient help text if present
	if (!AlgorithmInstance->HelpText.Draw()) {
		if (Editable) {
			NT_drawText(167, 58, "Push: Lock", 15, kNT_textLeft, kNT_textTiny);
		} else {
			NT_drawText(163, 58, "Push: Unlock", 15, kNT_textLeft, kNT_textTiny);
		}
		NT_drawText(2, 64, "Select Mode", 15, kNT_textLeft, kNT_textTiny);
		NT_drawText(105, 64, "Select Value", 15, kNT_textLeft, kNT_textTiny);
		NT_drawText(219, 64, "Set Value", 15, kNT_textLeft, kNT_textTiny);
	}
	NT_drawShapeI(kNT_line, 0, 50, 255, 50, 15);
}


void QuantizerView::DrawArrow(uint8_t x, uint8_t y, uint8_t color) const {
	NT_drawShapeI(kNT_line, x, y - 4, x + 4, y - 4, color);
	NT_drawShapeI(kNT_line, x + 4, y - 4, x + 2, y - 6, color);
	NT_drawShapeI(kNT_line, x + 4, y - 4, x + 2, y - 2, color);
}


void QuantizerView::DrawResult(uint8_t x, uint8_t y, const char* label, const char* result, const char* transposedResult, uint8_t color) const {
	NT_drawText(x, y, label, 8);
	NT_drawText(x + 28, y, result, 8);
	if (strcmp(result, transposedResult) != 0) {
		DrawArrow(x + 42, y, 8);
		NT_drawText(x + 49, y, transposedResult, 8);
	}
}


void QuantizerView::DrawResults() const {
	switch (AlgorithmInstance->NumChannels)	{
		// these cases explicity fall-through
		case 8:
			DrawResult(192, 46, "Ch 8:", "G", "B", 8);
		case 7:
			DrawResult(192, 34, "Ch 7:", "F", "F#", 8);
		case 6:
			DrawResult(192, 22, "Ch 6:", "F#", "F#", 8);
		case 5:
			DrawResult(192, 10, "Ch 5:", "C", "D#", 8);
		case 4:
			DrawResult(123, 46, "Ch 4:", "C#", "D", 8);
		case 3:
			DrawResult(123, 34, "Ch 3:", "B", "B", 8);
		case 2:
			DrawResult(123, 22, "Ch 2:", "G", "A", 8);
		case 1:
			DrawResult(123, 10, "Ch 1:", "C#", "F#", 8);
		default:
			break;
	}
}


void QuantizerView::Pot1Turn(float val) {
	// if (Editable) {
	// 	AlgorithmInstance->Input.UpdateValueWithPot(1, val, SelectedControlIndexRaw, 0, ARRAY_SIZE(Controls) - 0.001f);
	// 	auto old = SelectedControlIndex;
	// 	SelectedControlIndex = static_cast<int>(SelectedControlIndexRaw);
	// 	LoadControlForEditing();
	// 	if (SelectedControlIndex != old) {
	// 		AlgorithmInstance->HelpText.DisplayHelpText(Controls[SelectedControlIndex].HelpText);
	// 	}
	// }
}