#include <cstddef>
#include <distingnt/api.h>
#include <string.h>
#include "common.h"
#include "quantizerMode.h"
#include "directionalSequencer.h"


const QuantizerMode::Control QuantizerMode::Controls[] = {
	{ kParamAttenValue,        "  Attenuate the sequence value before quant"  },
	{ kParamOffsetValue,       " Offset the sequence value before quantizing" },
	{ kParamTranspose,         "   Transpose the sequence after quantizing"   },
	{ kParamQuantWeightC,      "  Adjust the attraction weighting of note C"  },
	{ kParamQuantWeightCSharp, "  Adjust the attraction weighting of note C#" },
	{ kParamQuantWeightD,      "  Adjust the attraction weighting of note D"  },
	{ kParamQuantWeightDSharp, "  Adjust the attraction weighting of note D#" },
	{ kParamQuantWeightE,      "  Adjust the attraction weighting of note E"  },
	{ kParamQuantWeightF,      "  Adjust the attraction weighting of note F"  },
	{ kParamQuantWeightFSharp, "  Adjust the attraction weighting of note F#" },
	{ kParamQuantWeightG,      "  Adjust the attraction weighting of note G"  },
	{ kParamQuantWeightGSharp, "  Adjust the attraction weighting of note G#" },
	{ kParamQuantWeightA,      "  Adjust the attraction weighting of note A"  },
	{ kParamQuantWeightASharp, "  Adjust the attraction weighting of note A#" },
	{ kParamQuantWeightB,      "  Adjust the attraction weighting of note B"  },
};


const QuantizerMode::Control& QuantizerMode::FindControlByParameterIndex(uint8_t idx) const {
	for (const auto& ctrl : Controls) {
		if (ctrl.ParameterIndex == idx) {
			return ctrl;
		}
	}
	// this should not happen, but we gotta satisfy the compiler of that
	static constexpr Control dummy = { 0xFF, "Invalid parameter index" };
	return dummy;
}


void QuantizerMode::DrawIcon(int x, int y, int color) const {
	NT_drawShapeI(kNT_rectangle, x + 0, y + 0, x + 16, y + 16, color);
	NT_drawShapeI(kNT_line, x +  5, y + 0, x +  5, y + 16, 0);
	NT_drawShapeI(kNT_line, x + 11, y + 0, x + 11, y + 16, 0);
	NT_drawShapeI(kNT_rectangle, x +  3, y + 0, x +  6, y + 8, 0);
	NT_drawShapeI(kNT_rectangle, x + 10, y + 0, x + 13, y + 8, 0);
}


void QuantizerMode::Draw() const {
	DrawParameters();
	DrawKeyboard(ModeAreaX + 83, 0);
	DrawQuantizationResults();
	DrawHelpSection();
}


void QuantizerMode::DrawParameter(uint8_t labelX, uint8_t editBoxX, uint8_t editBoxWidth, uint8_t y, const char* label, int paramIdx, uint8_t decimalPlaces, const char* suffix) const {
	NT_drawText(labelX, y + 8, label, 8);
	float scaling = CalculateScaling(AlgorithmInstance->parameters[paramIdx].scaling);
	auto val = AlgorithmInstance->v[paramIdx] / scaling;
	auto& ctrl = FindControlByParameterIndex(paramIdx);
	auto selected = &Controls[SelectedControlIndex] == &ctrl;

	if (scaling == 1) {
		NT_intToString(&NumToStrBuf[0], val);
	} else {
		NT_floatToString(&NumToStrBuf[0], val, decimalPlaces);
	}

	AddSuffixToBuf(suffix);
	DrawEditBox(editBoxX, y, editBoxWidth, &NumToStrBuf[0], selected, Editable);
}


void QuantizerMode::DrawParameters() const {
	DrawParameter(ModeAreaX, ModeAreaX + 39, 39, 1,  "Atten",  kParamAttenValue,  1, "%");
	DrawParameter(ModeAreaX, ModeAreaX + 39, 39, 11, "Offset", kParamOffsetValue, 3, "V");
	DrawParameter(ModeAreaX, ModeAreaX + 33, 45, 21, "Trans",  kParamTranspose,   0, " semi");
}


void QuantizerMode::DrawWhiteKeySlider(uint8_t x, uint8_t y, int paramIdx) const {
	float scaling = CalculateScaling(AlgorithmInstance->parameters[paramIdx].scaling);
	auto val = AlgorithmInstance->v[paramIdx] / scaling;
	if (val > 0) {
		auto height = val * 49 / MaxSliderValue;
		auto dy = 49 - height;
		NT_drawShapeI(kNT_rectangle, x, y + dy, x + 15, y + 49, ActiveKeyColor);
		NT_drawShapeI(kNT_rectangle, x, y + dy, x + 15, y + dy, SliderLineColor);
	}
}


void QuantizerMode::DrawBlackKeySlider(uint8_t x, uint8_t y, int paramIdx) const {
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


void QuantizerMode::DrawCFKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y +  0 - expandBy, x +  9 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  9 + expandBy, y +  0 - expandBy, x +  9 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  9 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y +  0 - expandBy, color);
}


void QuantizerMode::DrawDKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y +  0 - expandBy, x + 11 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y +  0 - expandBy, x + 11 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y +  0 - expandBy, color);
}


void QuantizerMode::DrawEBKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  6 - expandBy, y +  0 - expandBy, x + 15 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y +  0 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  6 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  6 - expandBy, y + 34 - expandBy, x +  6 - expandBy, y +  0 - expandBy, color);
}


void QuantizerMode::DrawGKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y +  0 - expandBy, x + 10 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 10 + expandBy, y +  0 - expandBy, x + 10 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 10 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  4 - expandBy, y + 34 - expandBy, x +  4 - expandBy, y +  0 - expandBy, color);
}


void QuantizerMode::DrawAKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_line, x +  5 - expandBy, y +  0 - expandBy, x + 11 + expandBy, y +  0 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y +  0 - expandBy, x + 11 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 11 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 34 - expandBy, x + 15 + expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x + 15 + expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 49 + expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 49 + expandBy, x +  0 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  0 - expandBy, y + 34 - expandBy, x +  5 - expandBy, y + 34 - expandBy, color);
	NT_drawShapeI(kNT_line, x +  5 - expandBy, y + 34 - expandBy, x +  5 - expandBy, y +  0 - expandBy, color);
}


void QuantizerMode::DrawBlackKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const {
	NT_drawShapeI(kNT_box, x + 0 - expandBy, y + 0 - expandBy, x + 8 + expandBy, y + 32 + expandBy, color);
}


void QuantizerMode::DrawKeyboard(uint8_t x, uint8_t y) const {
	// draw the sliders as rectangles first...  we will come back and draw the borders at the end, so they are not drawn over
	// also draw white key sliders before black key sliders, since black ones overlap white ones
	DrawWhiteKeySlider(x +   0, y, kParamQuantWeightC);
	DrawWhiteKeySlider(x +  17, y, kParamQuantWeightD);
	DrawWhiteKeySlider(x +  34, y, kParamQuantWeightE);
	DrawWhiteKeySlider(x +  51, y, kParamQuantWeightF);
	DrawWhiteKeySlider(x +  68, y, kParamQuantWeightG);
	DrawWhiteKeySlider(x +  85, y, kParamQuantWeightA);
	DrawWhiteKeySlider(x + 102, y, kParamQuantWeightB);

	DrawBlackKeySlider(x + 11, y, kParamQuantWeightCSharp);
	DrawBlackKeySlider(x + 30, y, kParamQuantWeightDSharp);
	DrawBlackKeySlider(x + 62, y, kParamQuantWeightFSharp);
	DrawBlackKeySlider(x + 80, y, kParamQuantWeightGSharp);
	DrawBlackKeySlider(x + 98, y, kParamQuantWeightASharp);

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
		if (Controls[SelectedControlIndex].ParameterIndex >= kParamQuantWeightC) {
			switch (Controls[SelectedControlIndex].ParameterIndex)
			{
				case kParamQuantWeightC:
					DrawCFKeyBorder(x + 0, y, -1, 15);
					DrawCFKeyBorder(x + 0, y,  0, 15);
					break;
				case kParamQuantWeightCSharp:
					DrawBlackKeyBorder(x + 11, y, -1, 15);
					DrawBlackKeyBorder(x + 11, y,  0, 15);
					break;
				case kParamQuantWeightD:
					DrawDKeyBorder(x + 17, y, -1, 15);
					DrawDKeyBorder(x + 17, y,  0, 15);
					break;
				case kParamQuantWeightDSharp:
					DrawBlackKeyBorder(x + 30, y, -1, 15);
					DrawBlackKeyBorder(x + 30, y,  0, 15);
					break;
				case kParamQuantWeightE:
					DrawEBKeyBorder(x + 34, y, -1, 15);
					DrawEBKeyBorder(x + 34, y,  0, 15);
					break;
				case kParamQuantWeightF:
					DrawCFKeyBorder(x + 51, y, -1, 15);
					DrawCFKeyBorder(x + 51, y,  0, 15);
					break;
				case kParamQuantWeightFSharp:
					DrawBlackKeyBorder(x + 62, y, -1, 15);
					DrawBlackKeyBorder(x + 62, y,  0, 15);
					break;
				case kParamQuantWeightG:
					DrawGKeyBorder(x + 68, y, -1, 15);
					DrawGKeyBorder(x + 68, y,  0, 15);
					break;
				case kParamQuantWeightGSharp:
					DrawBlackKeyBorder(x + 80, y, -1, 15);
					DrawBlackKeyBorder(x + 80, y,  0, 15);
					break;
				case kParamQuantWeightA:
					DrawAKeyBorder(x + 85, y, -1, 15);
					DrawAKeyBorder(x + 85, y,  0, 15);
					break;
				case kParamQuantWeightASharp:
					DrawBlackKeyBorder(x + 98, y, -1, 15);
					DrawBlackKeyBorder(x + 98, y,  0, 15);
					break;
				case kParamQuantWeightB:
					DrawEBKeyBorder(x + 102, y, -1, 15);
					DrawEBKeyBorder(x + 102, y,  0, 15);
					break;
				default:
					break;
			}
		}
	}
}


void QuantizerMode::DrawHelpSection() const {
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


void QuantizerMode::Encoder2ShortPress() {
	Editable = !Editable;
}


void QuantizerMode::DrawQuantizationResults() const {
	// TODO:  this whole mode goes away once we seperate quantizer from sequencer
	// auto result = AlgorithmInstance->Quant.LastResult;
	// auto x = ModeAreaX + 33;

	// if (strcmp(result.QuantizedNoteName, "") != 0) {
	// 	if (strcmp(result.QuantizedNoteName, result.FinalNoteName) != 0) {
	// 		NT_drawText(x - 20, 44, result.QuantizedNoteName, 8);
	// 		NT_drawText(x, 44, "=>", 8);
	// 		NT_drawText(x + 20, 44, result.FinalNoteName, 8);
	// 	} else {
	// 		NT_drawText(x, 44, result.QuantizedNoteName, 8);
	// 	}
	// }
}


void QuantizerMode::Pot2Turn(float val) {
	if (Editable) {
		AlgorithmInstance->UpdateValueWithPot(1, val, SelectedControlIndexRaw, 0, ARRAY_SIZE(Controls) - 0.001f);
		auto old = SelectedControlIndex;
		SelectedControlIndex = static_cast<int>(SelectedControlIndexRaw);
		LoadControlForEditing();
		if (SelectedControlIndex != old) {
			AlgorithmInstance->HelpText.DisplayHelpText(Controls[SelectedControlIndex].HelpText);
		}
	}
}


void QuantizerMode::Pot3Turn(float val) {
	if (Editable) {
		auto alg = NT_algorithmIndex(AlgorithmInstance);
		auto parameterIndex = Controls[SelectedControlIndex].ParameterIndex;
		auto& param = AlgorithmInstance->parameters[parameterIndex];
		bool isEnum = param.unit == kNT_unitEnum;
		auto min = param.min;
		auto max = param.max + (isEnum ? 0.99f : 0);
		AlgorithmInstance->UpdateValueWithPot(2, val, SelectedControlValueRaw, min, max);
		NT_setParameterFromUi(alg, parameterIndex + NT_parameterOffset(), SelectedControlValueRaw);
		AlgorithmInstance->HelpText.DisplayHelpText(Controls[SelectedControlIndex].HelpText);
	}
}


void QuantizerMode::Pot3ShortPress() {
	if (Editable) {
		auto alg = NT_algorithmIndex(AlgorithmInstance);
		auto parameterIndex = Controls[SelectedControlIndex].ParameterIndex;
		auto& param = AlgorithmInstance->parameters[parameterIndex];
		// since we are dealing with unscaled numbers here, epsilon is always 0.5
		SelectedControlValueRaw = param.def + 0.5;
		NT_setParameterFromUi(alg, parameterIndex + NT_parameterOffset(), SelectedControlValueRaw);
		AlgorithmInstance->HelpText.DisplayHelpText(Controls[SelectedControlIndex].HelpText);
	}
}


void QuantizerMode::FixupPotValues(_NT_float3& pots) {
	// calculate p2
	pots[1] = SelectedControlIndexRaw / ARRAY_SIZE(Controls);

	// calculate p3
	auto parameterIndex = Controls[SelectedControlIndex].ParameterIndex;
	auto& param = AlgorithmInstance->parameters[parameterIndex];
	auto val = AlgorithmInstance->v[parameterIndex];
	bool isEnum = param.unit == kNT_unitEnum;
	auto min = param.min;
	auto max = param.max + (isEnum ? 0 : 0.99f);
	auto range = max - min;
	pots[2] = (val - min) / range;
	// we need to reload our control for editing because the underlying parameter value could have changed while we were "sleeping"
	// this works because this event is fired when returning to our algorithm from other disting screens
	LoadControlForEditing();
}


void QuantizerMode::LoadControlForEditing() {
	auto parameterIndex = Controls[SelectedControlIndex].ParameterIndex;
	auto val = AlgorithmInstance->v[parameterIndex];
	// since we are dealing with unscaled numbers here, epsilon is always 0.5
	SelectedControlValueRaw = val + 0.5f;
}


void QuantizerMode::Activate() {
	LoadControlForEditing();
}