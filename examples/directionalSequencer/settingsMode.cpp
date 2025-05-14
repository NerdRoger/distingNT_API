#include <cstddef>
#include <distingnt/api.h>
#include "settingsMode.h"
#include "common.h"
#include "directionalSequencer.h"


using enum ParameterDefinition::ParameterIndex;
const SettingsMode::Control SettingsMode::GateSourceControl        = { kParamGateLengthSource,    "  Derive gate from max gate length or clock" };
const SettingsMode::Control SettingsMode::GateAttenControl         = { kParamGateLengthAttenuate, "  Attenuate the length of all gates at once" };
const SettingsMode::Control SettingsMode::MaxGateControl           = { kParamMaxGateLength,       "      The max length of the gate, in ms" };
const SettingsMode::Control SettingsMode::HumanizeControl          = { kParamHumanizeValue,       "Humanize gate position + length, and velocity" };
const SettingsMode::Control SettingsMode::VelocityAttenControl     = { kParamVelocityAttenuate,   " Attenuate the velocity of all steps at once" };
const SettingsMode::Control SettingsMode::VelocityOffsetControl    = { kParamVelocityOffset,      "   Offset the velocity of all steps at once" };
const SettingsMode::Control SettingsMode::MoveNCellsControl        = { kParamMoveNCells,          "  Advance this many cells in given direction" };
const SettingsMode::Control SettingsMode::RestAfterNStepsControl   = { kParamRestAfterNSteps,     "   Rest for one step after playing N steps" };
const SettingsMode::Control SettingsMode::SkipAfterNStepsControl   = { kParamSkipAfterNSteps,     "   Skip over one step after playing N steps" };
const SettingsMode::Control SettingsMode::ResetAfterNStepsControl  = { kParamResetAfterNSteps,    "     Reset sequencer after playing N steps" };
const SettingsMode::Control SettingsMode::ResetWhenInactiveControl = { kParamResetWhenInactive,   " Reset sequencer if no gate after 10 seconds" };


const SettingsMode::Control& SettingsMode::FindControlByParameterIndex(uint8_t idx) const {
	for (const auto& ctrl : SelectableControls) {
		if (ctrl->ParameterIndex == idx) {
			return *ctrl;
		}
	}
	// this should not happen, but we gotta satisfy the compiler of that
	static const Control dummy = { 0xFF, "Invalid parameter index" };
	return dummy;
}


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
	DrawParameters();
}


void SettingsMode::DrawParameter(uint8_t labelX, uint8_t editBoxX, uint8_t editBoxWidth, uint8_t y, const char* label, ParameterDefinition::ParameterIndex paramIdx, uint8_t decimalPlaces, const char* suffix) const {
	auto& ctrl = FindControlByParameterIndex(paramIdx);
	auto selected = SelectedControl == &ctrl;
	NT_drawText(labelX, y + 8, label, 8);

	// special case where we need to override the displayed text
	if (paramIdx == kParamGateLengthSource) {
		const char* txt = AlgorithmInstance->v[paramIdx] == 0 ? "Max Gt" : "Clock";
		DrawEditBox(editBoxX, y, editBoxWidth, txt, selected, Editable);
		return;
	}

	float scaling = CalculateScaling(ParameterDefinition::Parameters[paramIdx].scaling);
	auto val = AlgorithmInstance->v[paramIdx] / scaling;
	if (scaling == 1) {
		NT_intToString(&NumToStrBuf[0], val);
	} else {
		NT_floatToString(&NumToStrBuf[0], val, decimalPlaces);
	}
	AddSuffixToBuf(suffix);
	DrawEditBox(editBoxX, y, editBoxWidth, &NumToStrBuf[0], selected, Editable);
};


void SettingsMode::DrawParameters() const {
	using enum ParameterDefinition::ParameterIndex;
	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 1, "Gate Source", kParamGateLengthSource, 0, "");

	if (SelectableControls[1] == &GateAttenControl) {
		DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 11, "Gate Atten", kParamGateLengthAttenuate, 1, "%");
	} else {
		DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 11, "Max Gate", kParamMaxGateLength, 0, "ms");
	}

	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 21, "Humanize", kParamHumanizeValue, 1, "%");
	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 31, "Velo. Atten", kParamVelocityAttenuate, 1, "%");
	DrawParameter(ModeAreaX, ModeAreaX + 68, 32, 41, "Velo. Offset", kParamVelocityOffset, 0, "");





/*
	Label = "Move",        
	Label = "Rest After",  
	Label = "Skip After",  
	Label = "Reset After", 
	Label = "Inact. Reset",
*/




}
