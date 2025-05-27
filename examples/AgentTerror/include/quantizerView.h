#pragma once
#include <distingnt/api.h>
#include <stddef.h>
#include "ownedBase.h"


struct WeightedQuantizer;


struct QuantizerView : OwnedBase<WeightedQuantizer> {
private:

	struct Control {
		uint8_t     ParameterIndex;
		const char* HelpText;
	};

	static constexpr int ActiveKeyColor = 6;
	static constexpr int SliderLineColor = 15;
	static constexpr int KeyBorderColor = 8;
	static constexpr float MaxSliderValue = 10.0f;

	bool Editable = true;

	static const Control Controls[];
	size_t SelectedControlIndex = 0;
	float SelectedControlIndexRaw = 0.0f;
	float SelectedControlValueRaw = 0.0f;

	void DrawWhiteKeySlider(uint8_t x, uint8_t y, int paramIdx) const;
	void DrawBlackKeySlider(uint8_t x, uint8_t y, int paramIdx) const;
	void DrawCFKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawDKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawEBKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawGKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawAKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawBlackKeyBorder(uint8_t x, uint8_t y, uint8_t expandBy, int color) const;
	void DrawHelpSection() const;
	void DrawKeyboard(uint8_t x, uint8_t y) const;
	void DrawSlot(uint8_t x, uint8_t y, uint8_t slotNum) const;
	void DrawArrow(uint8_t x, uint8_t y, uint8_t color) const;
	void DrawResult(uint8_t x, uint8_t y, const char* label, const char* result, const char* transposedResult, uint8_t color) const;
	void DrawResults() const;

public:
	void Draw() const;

	void Pot1Turn(float val);
	void Pot3Turn(float val);


};