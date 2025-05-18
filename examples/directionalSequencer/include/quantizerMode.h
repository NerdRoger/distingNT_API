#pragma once

#include "modeBase.h"
#include "parameterDefinition.h"


struct QuantizerMode : ModeBase {
private:

	struct Control {
		uint8_t     ParameterIndex;
		const char* HelpText;
	};

	static constexpr int ActiveKeyColor = 6;
	static constexpr int SliderLineColor = 11;
	static constexpr int KeyBorderColor = 8;
	static constexpr float MaxSliderValue = 10.0f;

	static const Control Controls[];
	size_t SelectedControlIndex = 0;
	float SelectedControlIndexRaw = 0.0f;
	float SelectedControlValueRaw = 0.0f;

	const Control& FindControlByParameterIndex(uint8_t idx) const;
	void DrawParameter(uint8_t labelX, uint8_t editBoxX, uint8_t editBoxWidth, uint8_t y, const char* label, ParameterIndex paramIdx, uint8_t decimalPlaces, const char* suffix) const;
	void DrawParameters() const;

	void DrawWhiteKeySlider(uint8_t x, uint8_t y, ParameterIndex paramIdx) const;
	void DrawBlackKeySlider(uint8_t x, uint8_t y, ParameterIndex paramIdx) const;
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
