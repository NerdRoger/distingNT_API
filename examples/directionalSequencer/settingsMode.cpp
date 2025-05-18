// #include <cstddef>
// #include <distingnt/api.h>
// #include "settingsMode.h"
// #include "common.h"
// #include "directionalSequencer.h"

// // TODO:  remove this hack
// int paramIdx = 0;
// float paramVal = 0.0f;
// uint32_t paramChangeCount;


// using enum ParameterIndex;
// const SettingsMode::Control SettingsMode::GateSourceControl        = { kParamGateLengthSource,    "  Derive gate from max gate length or clock" };
// const SettingsMode::Control SettingsMode::GateAttenControl         = { kParamGateLengthAttenuate, "  Attenuate the length of all gates at once" };
// const SettingsMode::Control SettingsMode::MaxGateControl           = { kParamMaxGateLength,       "      The max length of the gate, in ms" };
// const SettingsMode::Control SettingsMode::HumanizeControl          = { kParamHumanizeValue,       "Humanize gate position + length, and velocity" };
// const SettingsMode::Control SettingsMode::VelocityAttenControl     = { kParamVelocityAttenuate,   " Attenuate the velocity of all steps at once" };
// const SettingsMode::Control SettingsMode::VelocityOffsetControl    = { kParamVelocityOffset,      "   Offset the velocity of all steps at once" };
// const SettingsMode::Control SettingsMode::MoveNCellsControl        = { kParamMoveNCells,          "  Advance this many cells in given direction" };
// const SettingsMode::Control SettingsMode::RestAfterNStepsControl   = { kParamRestAfterNSteps,     "   Rest for one step after playing N steps" };
// const SettingsMode::Control SettingsMode::SkipAfterNStepsControl   = { kParamSkipAfterNSteps,     "   Skip over one step after playing N steps" };
// const SettingsMode::Control SettingsMode::ResetAfterNStepsControl  = { kParamResetAfterNSteps,    "     Reset sequencer after playing N steps" };
// const SettingsMode::Control SettingsMode::ResetWhenInactiveControl = { kParamResetWhenInactive,   " Reset sequencer if no gate after 10 seconds" };


// const SettingsMode::Control& SettingsMode::FindControlByParameterIndex(uint8_t idx) const {
// 	for (const auto& ctrl : SelectableControls) {
// 		if (ctrl->ParameterIndex == idx) {
// 			return *ctrl;
// 		}
// 	}
// 	// this should not happen, but we gotta satisfy the compiler of that
// 	static const Control dummy = { 0xFF, "Invalid parameter index" };
// 	return dummy;
// }


// void SettingsMode::DrawIcon(int x, int y, int color) const {
// 	// faders
// 	NT_drawShapeI(kNT_box, x +  5, y +  2, x +  8, y +  4, color);
// 	NT_drawShapeI(kNT_box, x + 12, y +  7, x + 15, y +  9, color);
// 	NT_drawShapeI(kNT_box, x +  2, y + 12, x +  5, y + 14, color);
// 	// lines
// 	NT_drawShapeI(kNT_line, x +  0, y +  3, x +  5, y +  3, color);
// 	NT_drawShapeI(kNT_line, x +  8, y +  3, x + 16, y +  3, color);
// 	NT_drawShapeI(kNT_line, x +  0, y +  8, x + 12, y +  8, color);
// 	NT_drawShapeI(kNT_line, x + 15, y +  8, x + 16, y +  8, color);
// 	NT_drawShapeI(kNT_line, x +  0, y + 13, x +  2, y + 13, color);
// 	NT_drawShapeI(kNT_line, x +  5, y + 13, x + 16, y + 13, color);
// }


// void SettingsMode::Draw() const {


// 	NT_drawShapeI(kNT_rectangle, 0, 0, 50, 50, 0);

// 	NT_intToString(&NumToStrBuf[0], paramIdx);
// 	NT_drawText(0, 10, NumToStrBuf, 15);

// 	NT_floatToString(&NumToStrBuf[0], paramVal, 3);
// 	NT_drawText(0, 20, NumToStrBuf, 15);

// 	NT_intToString(&NumToStrBuf[0], paramChangeCount);
// 	NT_drawText(0, 30, NumToStrBuf, 15);


// 	DrawParameters();
// 	DrawHelpSection();
// }


// void SettingsMode::DrawParameter(uint8_t labelX, uint8_t editBoxX, uint8_t editBoxWidth, uint8_t y, const char* label, ParameterIndex paramIdx, uint8_t decimalPlaces, const char* suffix) const {
// 	auto& ctrl = FindControlByParameterIndex(paramIdx);
// 	auto selected = SelectedControl == &ctrl;
// 	NT_drawText(labelX, y + 8, label, 8);

// 	// special case where we need to override the displayed text
// 	if (paramIdx == kParamGateLengthSource) {
// 		const char* txt = AlgorithmInstance->v[paramIdx] == 0 ? "Max Gt" : "Clock";
// 		DrawEditBox(editBoxX, y, editBoxWidth, txt, selected, Editable);
// 		return;
// 	}

// 	auto& param = ParameterDefinition::Parameters[paramIdx];
// 	// enum parameters
// 	if (param.unit == kNT_unitEnum) {
// 		auto val = param.enumStrings[AlgorithmInstance->v[paramIdx]];
// 		DrawEditBox(editBoxX, y, editBoxWidth, val, selected, Editable);
// 		return;
// 	}

// 	float scaling = CalculateScaling(param.scaling);
// 	auto val = AlgorithmInstance->v[paramIdx] / scaling;
// 	if (scaling == 1) {
// 		NT_intToString(&NumToStrBuf[0], val);
// 	} else {
// 		NT_floatToString(&NumToStrBuf[0], val, decimalPlaces);
// 	}
// 	AddSuffixToBuf(suffix);
// 	DrawEditBox(editBoxX, y, editBoxWidth, &NumToStrBuf[0], selected, Editable);
// };


// void SettingsMode::DrawParameters() const {
// 	using enum ParameterIndex;
// 	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 1, "Gate Source", kParamGateLengthSource, 0, "");

// 	if (SelectableControls[1] == &GateAttenControl) {
// 		DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 11, "Gate Atten", kParamGateLengthAttenuate, 1, "%");
// 	} else {
// 		DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 11, "Max Gate", kParamMaxGateLength, 0, "ms");  // this is line 85
// 	}

// 	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 21, "Humanize", kParamHumanizeValue, 1, "%");
// 	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 31, "Velo. Atten", kParamVelocityAttenuate, 1, "%");
// 	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 41, "Velo. Offset", kParamVelocityOffset, 0, "");

// 	DrawParameter(ModeAreaX + 107, ModeAreaX + 175, 26,  1, "Move",         kParamMoveNCells,        0, "");
// 	DrawParameter(ModeAreaX + 107, ModeAreaX + 175, 26, 11, "Rest After",   kParamRestAfterNSteps,   0, "");
// 	DrawParameter(ModeAreaX + 107, ModeAreaX + 175, 26, 21, "Skip After",   kParamSkipAfterNSteps,   0, "");
// 	DrawParameter(ModeAreaX + 107, ModeAreaX + 175, 26, 31, "Reset After",  kParamResetAfterNSteps,  0, "");
// 	DrawParameter(ModeAreaX + 107, ModeAreaX + 175, 26, 41, "Inact. Reset", kParamResetWhenInactive, 0, "");
// }


// void SettingsMode::DrawHelpSection() const {
// 	// clear the area we're about to draw in, as we are drawing on top of the grid
// 	NT_drawShapeI(kNT_rectangle, 0, 50, 255, 63, 0);

// 	// draw the transient help text if present
// 	if (!AlgorithmInstance->HelpText.Draw()) {
// 		if (Editable) {
// 			NT_drawText(167, 58, "Push: Lock", 15, kNT_textLeft, kNT_textTiny);
// 		} else {
// 			NT_drawText(163, 58, "Push: Unlock", 15, kNT_textLeft, kNT_textTiny);
// 		}
// 		NT_drawText(2, 64, "Select Mode", 15, kNT_textLeft, kNT_textTiny);
// 		NT_drawText(105, 64, "Select Value", 15, kNT_textLeft, kNT_textTiny);
// 		NT_drawText(219, 64, "Set Value", 15, kNT_textLeft, kNT_textTiny);
// 	}
// 	NT_drawShapeI(kNT_line, 0, 50, 255, 50, 15);
// }


// void SettingsMode::Encoder2ShortPress() {
// 	Editable = !Editable;
// }


// void SettingsMode::Pot2Turn(float val) {
// 	if (Editable) {
// 		AlgorithmInstance->Input.UpdateValueWithPot(1, val, SelectedControlIndexRaw, 0, ARRAY_SIZE(SelectableControls) - 0.001f);
// 		auto old = SelectedControl;
// 		SelectedControl = SelectableControls[static_cast<int>(SelectedControlIndexRaw)];
// 		LoadControlForEditing();
// 		if (SelectedControl != old) {
// 			AlgorithmInstance->HelpText.DisplayHelpText(SelectedControl->HelpText);
// 		}
// 	}
// }


// void SettingsMode::Pot3Turn(float val) {
// 	if (Editable) {
// 		auto alg = NT_algorithmIndex(AlgorithmInstance);
// 		auto parameterIndex = SelectedControl->ParameterIndex;
// 		auto& param = ParameterDefinition::Parameters[parameterIndex];
// 		bool isEnum = param.unit == kNT_unitEnum;
// 		auto min = param.min;
// 		auto max = param.max + (isEnum ? 0.99f : 0);
// 		AlgorithmInstance->Input.UpdateValueWithPot(2, val, SelectedControlValueRaw, min, max);
// 		NT_setParameterFromUi(alg, parameterIndex + NT_parameterOffset(), SelectedControlValueRaw);
// 		AlgorithmInstance->HelpText.DisplayHelpText(SelectedControl->HelpText);
// 	}
// }


// void SettingsMode::Pot3ShortPress() {
// 	if (Editable) {
// 		auto alg = NT_algorithmIndex(AlgorithmInstance);
// 		auto parameterIndex = SelectedControl->ParameterIndex;
// 		auto& param = ParameterDefinition::Parameters[parameterIndex];
// 		// since we are dealing with unscaled numbers here, epsilon is always 0.5
// 		SelectedControlValueRaw = param.def + 0.5;
// 		NT_setParameterFromUi(alg, parameterIndex + NT_parameterOffset(), SelectedControlValueRaw);
// 		AlgorithmInstance->HelpText.DisplayHelpText(SelectedControl->HelpText);
// 	}
// }


// void SettingsMode::FixupPotValues(_NT_float3& pots) {
// 	// calculate p2
// 	pots[1] = SelectedControlIndexRaw / ARRAY_SIZE(SelectableControls);

// 	// calculate p3
// 	auto parameterIndex = SelectedControl->ParameterIndex;
// 	auto& param = ParameterDefinition::Parameters[parameterIndex];
// 	auto val = AlgorithmInstance->v[parameterIndex];
// 	bool isEnum = param.unit == kNT_unitEnum;
// 	auto min = param.min;
// 	auto max = param.max + (isEnum ? 0 : 0.99f);
// 	auto range = max - min;
// 	pots[2] = (val - min) / range;
// 	// we need to reload our control for editing because the underlying parameter value could have changed while we were "sleeping"
// 	// this works because this event is fired when returning to our algorithm from other disting screens
// 	LoadControlForEditing();
// }


// void SettingsMode::ParameterChanged(int paramIndex) {
// 	paramIdx = paramIndex;
// 	auto val = AlgorithmInstance->v[paramIndex];
// 	paramVal = val;
// 	paramChangeCount++;


// 	if (paramIndex == kParamGateLengthSource) {
// 		FixupParameters();
// 	}


// }


// void SettingsMode::LoadControlForEditing() {
// 	auto parameterIndex = SelectedControl->ParameterIndex;
// 	auto val = AlgorithmInstance->v[parameterIndex];
// 	// since we are dealing with unscaled numbers here, epsilon is always 0.5
// 	SelectedControlValueRaw = val + 0.5f;
// }


// void SettingsMode::Activate() {
// 	LoadControlForEditing();
// }


// void SettingsMode::FixupParameters() {
// 	auto selected = (SelectedControl == &GateAttenControl) || (SelectedControl == &MaxGateControl);
// 	auto gateSource = AlgorithmInstance->v[kParamGateLengthSource];
// 	if (gateSource == 0 && SelectableControls[1] != &MaxGateControl) {
// 		SelectableControls[1] = &MaxGateControl;
// 		if (selected) {
// 			SelectedControl = &MaxGateControl;
// 			LoadControlForEditing();
// 		}
// 	} else if (gateSource == 1 && SelectableControls[1] != &GateAttenControl) {
// 		SelectableControls[1] = &GateAttenControl;
// 		if (selected) {
// 			SelectedControl = &GateAttenControl;
// 			LoadControlForEditing();
// 		}
// 	}
// }