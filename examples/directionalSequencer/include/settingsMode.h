#pragma once

#include "modeBase.h"
#include "parameterDefinition.h"

struct SettingsMode : ModeBase {
private:

	struct Control {
		uint8_t     ParameterIndex;
		const char* HelpText;
	};

	static const Control GateSourceControl;
	static const Control GateAttenControl;
	static const Control MaxGateControl;
	static const Control HumanizeControl;
	static const Control VelocityAttenControl;
	static const Control VelocityOffsetControl;
	static const Control MoveNCellsControl;
	static const Control RestAfterNStepsControl;
	static const Control SkipAfterNStepsControl;
	static const Control ResetAfterNStepsControl;
	static const Control ResetWhenInactiveControl;

	const Control* SelectableControls[10] {
		&GateSourceControl,
		&GateAttenControl,
		&HumanizeControl,
		&VelocityAttenControl,
		&VelocityOffsetControl,
		&MoveNCellsControl, 
		&RestAfterNStepsControl,
		&SkipAfterNStepsControl,
		&ResetAfterNStepsControl,
		&ResetWhenInactiveControl
	};

	const Control* SelectedControl = SelectableControls[0];

	const Control& FindControlByParameterIndex(uint8_t idx) const;
	void DrawParameter(uint8_t labelX, uint8_t editBoxX, uint8_t editBoxWidth, uint8_t y, const char* label, ParameterDefinition::ParameterIndex paramIdx, uint8_t decimalPlaces, const char* suffix) const;
	void DrawParameters() const;
	
public:
	void DrawIcon(int x, int y, int color) const override;
	void Draw() const override;
};
