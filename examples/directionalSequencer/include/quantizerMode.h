#pragma once

#include "modeBase.h"
#include "parameterDefinition.h"

struct QuantizerMode : ModeBase {
private:

	struct Control {
		char    Name[20];
		uint8_t ParameterIndex;
		char    HelpText[45];
	};

	static constexpr int ActiveKeyColor = 6;
	static constexpr int SliderLineColor = 11;
	static constexpr int KeyBorderColor = 8;
	static constexpr float MaxSliderValue = 10.0f;

	using enum ParameterDefinition::ParameterIndex;
	static constexpr Control SelectableControls[] {
    { .Name = "Attenuate", .ParameterIndex = kParamAttenValue,        .HelpText = "  Attenuate the sequence value before quant" },
    { .Name = "Offset",    .ParameterIndex = kParamOffsetValue,       .HelpText = " Offset the sequence value before quantizing" },
    { .Name = "Transpose", .ParameterIndex = kParamTranspose,         .HelpText = "   Transpose the sequence after quantizing" },
    { .Name = "CKey",      .ParameterIndex = kParamQuantWeightC,      .HelpText = "  Adjust the attraction weighting of note C" },
    { .Name = "CSharpKey", .ParameterIndex = kParamQuantWeightCSharp, .HelpText = "  Adjust the attraction weighting of note C#" },
    { .Name = "DKey",      .ParameterIndex = kParamQuantWeightD,      .HelpText = "  Adjust the attraction weighting of note D" },
    { .Name = "DSharpKey", .ParameterIndex = kParamQuantWeightDSharp, .HelpText = "  Adjust the attraction weighting of note D#" },
    { .Name = "EKey",      .ParameterIndex = kParamQuantWeightE,      .HelpText = "  Adjust the attraction weighting of note E" },
    { .Name = "FKey",      .ParameterIndex = kParamQuantWeightF,      .HelpText = "  Adjust the attraction weighting of note F" },
    { .Name = "FSharpKey", .ParameterIndex = kParamQuantWeightFSharp, .HelpText = "  Adjust the attraction weighting of note F#" },
    { .Name = "GKey",      .ParameterIndex = kParamQuantWeightG,      .HelpText = "  Adjust the attraction weighting of note G" },
    { .Name = "GSharpKey", .ParameterIndex = kParamQuantWeightGSharp, .HelpText = "  Adjust the attraction weighting of note G#" },
    { .Name = "AKey",      .ParameterIndex = kParamQuantWeightA,      .HelpText = "  Adjust the attraction weighting of note A" },
    { .Name = "ASharpKey", .ParameterIndex = kParamQuantWeightASharp, .HelpText = "  Adjust the attraction weighting of note A#" },
    { .Name = "BKey",      .ParameterIndex = kParamQuantWeightB,      .HelpText = "  Adjust the attraction weighting of note B"	 },
	};
	
	float SelectedControlIndexRaw = 0.0f;
	float SelectedControlValueRaw = 0.0f;
	const Control* SelectedControl = &SelectableControls[0];

	static const Control& FindControlByParameterIndex(uint8_t idx);
	void DrawParameter(uint8_t labelX, uint8_t editBoxX, uint8_t editBoxWidth, uint8_t y, const char* label, ParameterDefinition::ParameterIndex paramIdx, uint8_t decimalPlaces, const char* suffix) const;
	void DrawParameters() const;

	void DrawWhiteKeySlider(uint8_t x, uint8_t y, ParameterDefinition::ParameterIndex paramIdx) const;
	void DrawBlackKeySlider(uint8_t x, uint8_t y, ParameterDefinition::ParameterIndex paramIdx) const;
	void DrawCFKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawDKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawEBKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawGKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawAKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawBlackKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawHelpSection() const;
	void DrawKeyboard(uint8_t x, uint8_t y) const;
	void DrawQuantizationResults() const;

public:
	void DrawIcon(int x, int y, int color) const override;
	void Draw() const override;

	void Encoder2ShortPress() override;
	void Pot2Turn(float val) override;
	void Pot3Turn(float val) override;
	void Pot3ShortPress() override;
	void FixupPotValues(_NT_float3& pots) override;

	void LoadControlForEditing();
	void Activate() override;
};
